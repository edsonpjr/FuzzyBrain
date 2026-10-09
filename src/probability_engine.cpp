#include "probability_engine.hpp"
#include <algorithm>
#include <ranges>
#include <cmath>
#include <memory>
#include <random>
#include <ui_console.hpp>

void ProbabilityEngine::initialize(const std::vector<QuestionRow>& matrix, bool debug_mode) {
    debug_mode_ = debug_mode;
    matrix_ = &matrix;
    hypotheses_.clear();
    asked_questions_.clear();

    if (matrix.empty() || matrix.front().candidate_relations.empty()) {
        return;
    }

    const auto& candidate_relations = matrix.front().candidate_relations;

    hypotheses_.reserve(candidate_relations.size());
    for (size_t candidate_index = 0; candidate_index < candidate_relations.size();
         ++candidate_index) {
        hypotheses_.emplace(candidate_index, Hypothesis{
                .candidate = std::make_unique<Candidate>(
                    candidate_relations.at(candidate_index).candidate
                ),
                .probability = 0.0,
                .consecutive_zero_rounds = 0
            });
    }

    reset_to_uniform_distribution();
}

auto ProbabilityEngine::get_next_question_index() const -> std::optional<std::size_t> {
    if (!strong_candidates_.indexed_hypotheses.empty()) {
        return select_question_by_outliers();
    }
    return select_question_by_weighted_variance();
}

void ProbabilityEngine::register_question_asked(size_t question_index) {
    asked_questions_.insert(question_index);
}

auto ProbabilityEngine::get_hypotheses() const -> const std::unordered_map<size_t, ProbabilityEngine::Hypothesis>& {
    return hypotheses_;
}

void ProbabilityEngine::update_probabilities(const ProbabilityParameters& params) {
    if (matrix_ == nullptr || hypotheses_.empty()) {
        return;
    }

    const auto& candidate_relations = matrix_->at(params.question_index).candidate_relations;
    double sum_of_probabilities = 0.0;

    for (auto& [candidate_index, hypothesis] : hypotheses_) {
        const double theoretical_value =
            candidate_relations.at(candidate_index).relation.get_membership_degree();
        const double similarity = 1.0 - std::abs(theoretical_value - params.user_answer);
        double updated_probability = hypothesis.probability * similarity;

        if (updated_probability <= epsilon) {
            ++hypothesis.consecutive_zero_rounds;
            if (hypothesis.consecutive_zero_rounds >= max_consecutive_zero_rounds) {
                updated_probability = 0.0;  // definitive elimination
            } else {
                updated_probability = epsilon;  // temporary survival
            }
        } else {
            hypothesis.consecutive_zero_rounds = 0;
        }

        hypothesis.probability = updated_probability;
        sum_of_probabilities += updated_probability;
    }

    if (sum_of_probabilities <= 0.0) {
        reset_to_uniform_distribution();  // fallback: uniform distribution
        return;
    }

    for (auto& [candidate_index, hypothesis] : hypotheses_) {
        hypothesis.probability /= sum_of_probabilities;
    }

    update_strong_candidates();
}

auto ProbabilityEngine::has_candidate_suggestion() const -> bool {

    const auto strong_candidates_count = strong_candidates_.indexed_hypotheses.size();
    bool has_strong_candidate = false;

    if (strong_candidates_count == 1) {
        const auto [index, hypothesis] = strong_candidates_.indexed_hypotheses.front();
        if (debug_mode_) {
            ConsoleUI::print_strong_candidate_found(false);
            ConsoleUI::print_strong_candidate_debug(hypothesis->candidate->get_text(),
                                                    hypothesis->probability);
            ConsoleUI::print_empty_line();
        }
        has_strong_candidate = hypothesis->probability >= minimum_probability_suggestion;
    }

    if (strong_candidates_count > 1) {
        const auto [top_index, hypothesis] = strong_candidates_.indexed_hypotheses.front();
        const double top_probability = hypothesis->probability;
        double second_probability = 0.0;

        if (debug_mode_) {
            ConsoleUI::print_strong_candidate_found(true);
            ConsoleUI::print_strong_candidate_debug(hypothesis->candidate->get_text(), 
                                                    top_probability);
        }
        for (size_t index = 0; index < strong_candidates_count; ++index) {
            const auto& [current_candidate_index, current_hypothesis] =
                strong_candidates_.indexed_hypotheses.at(index);

            if (current_candidate_index == top_index) {
                continue;
            }

            const auto& current_probability = current_hypothesis->probability;
            second_probability = std::max(current_probability, second_probability);

            if (debug_mode_) {
                ConsoleUI::print_strong_candidate_debug(current_hypothesis->candidate->get_text(), 
                                                        current_probability);
            }
        }

        if (debug_mode_) {
            ConsoleUI::print_empty_line();
        }

        if ((top_probability >= minimum_probability_suggestion) &&
            ((top_probability - second_probability) >= minimum_probability_difference)) {
            has_strong_candidate = true;
        }
    }

    if (strong_candidates_.rounds_with_same_candidates >= max_rounds_with_same_candidates) {
        has_strong_candidate = true;
    }

    return has_strong_candidate;
}

void ProbabilityEngine::update_strong_candidates() {
    std::vector<size_t> previous_indices;
    previous_indices.reserve(strong_candidates_.indexed_hypotheses.size());
    for (const auto& entry : strong_candidates_.indexed_hypotheses) {
        previous_indices.push_back(entry.first);
    }

    strong_candidates_ = {};
    if (hypotheses_.empty()) {
        return;
    }

    double sum = 0.0;
    for (const auto& [candidate_index, hypothesis] : hypotheses_) {
        sum += hypothesis.probability;
    }
    const double mean = sum / static_cast<double>(hypotheses_.size());

    double sum_of_squares = 0.0;
    for (const auto& [candidate_index, hypothesis] : hypotheses_) {
        const double difference = hypothesis.probability - mean;
        sum_of_squares += difference * difference;
    }
    const double standard_deviation =
        std::sqrt(sum_of_squares / static_cast<double>(hypotheses_.size()));

    if (standard_deviation <= 0.0) {
        return;
    }

    const double outlier_threshold = mean + (outlier_std_dev_weight * standard_deviation);

    strong_candidates_.mean = mean;
    strong_candidates_.threshold = outlier_threshold;

    for (const auto& [candidate_index, hypothesis] : hypotheses_) {
        if (hypothesis.probability > outlier_threshold) {
            strong_candidates_.indexed_hypotheses.emplace_back(candidate_index, &hypothesis);
        }
    }

    std::vector<size_t> current_indices;
    current_indices.reserve(strong_candidates_.indexed_hypotheses.size());
    for (const auto& entry : strong_candidates_.indexed_hypotheses) {
        current_indices.push_back(entry.first);
    }

    auto indices_equal_as_multiset = [](
        const std::vector<size_t>& right, 
        const std::vector<size_t>& left) -> bool {
            if (right.size() != left.size()) {
                return false;
            }
        std::vector<size_t> right_copy = right;
        std::vector<size_t> left_copy = left;
        std::ranges::sort(right_copy);
        std::ranges::sort(left_copy);
        return right_copy == left_copy;
    };

    if (!previous_indices.empty() && indices_equal_as_multiset(previous_indices, current_indices)) {
        strong_candidates_.rounds_with_same_candidates += 1;
    } else {
        strong_candidates_.rounds_with_same_candidates = 0;
    }

    std::ranges::sort(strong_candidates_.indexed_hypotheses,
        [](const auto& left_hypothesis, const auto& right_hypothesis) -> bool {
            return left_hypothesis.second->probability > right_hypothesis.second->probability;
        });
}

auto ProbabilityEngine::get_top_candidate_text() const -> std::string {
    if (hypotheses_.empty()) {
        return {};
    }

    size_t top_index = hypotheses_.begin()->first;
    double top_prob = -1.0;
    for (const auto& [candidate_index, hypothesis] : hypotheses_) {
        if (hypothesis.probability > top_prob) {
            top_prob = hypothesis.probability;
            top_index = candidate_index;
        }
    }
    return hypotheses_.at(top_index).candidate->get_text();
}

void ProbabilityEngine::eliminate_top_candidate() {
    if (hypotheses_.empty()) {
        update_strong_candidates();
        return;
    }

    size_t top_index = hypotheses_.begin()->first;
    double top_prob = -1.0;
    for (const auto& [candidate_index, hypothesis] : hypotheses_) {
        if (hypothesis.probability > top_prob) {
            top_prob = hypothesis.probability;
            top_index = candidate_index;
        }
    }

    hypotheses_.at(top_index).probability = 0.0;
    hypotheses_.at(top_index).consecutive_zero_rounds = max_consecutive_zero_rounds;

    double sum = 0.0;
    for (const auto& [candidate_index, hypothesis] : hypotheses_) {
        sum += hypothesis.probability;
    }

    if (sum <= 0.0) {
        reset_to_uniform_distribution();
        return;
    }

    for (auto& [candidate_index, hypothesis] : hypotheses_) {
        hypothesis.probability /= sum;
    }
    update_strong_candidates();
}

void ProbabilityEngine::reset_to_uniform_distribution() {
    if (hypotheses_.empty()) {
        return;
    }

    const double uniform_probability = 1.0 / static_cast<double>(hypotheses_.size());
    for (auto& [candidate_index, hypothesis] : hypotheses_) {
        hypothesis.probability = uniform_probability;
        hypothesis.consecutive_zero_rounds = 0;
    }

    update_strong_candidates();
}

auto ProbabilityEngine::select_question_by_weighted_variance() const -> std::optional<std::size_t> {
    size_t chosen_index = -1;
    std::vector<std::pair<size_t, double>> questions_index_variance;

    if (matrix_ == nullptr) {
        return chosen_index;
    }

    for (size_t question_index = 0; question_index < matrix_->size(); ++question_index) {
        if (asked_questions_.contains(question_index)) {
            continue;
        }

        const auto& candidate_relations = matrix_->at(question_index).candidate_relations;
        const size_t total_candidates = candidate_relations.size();
        if (total_candidates == 0) {
            continue;
        }

        double sum = 0.0;
        for (const auto& candidate_relation : candidate_relations) {
            sum += candidate_relation.relation.get_membership_degree();
        }
        const double mean = sum / static_cast<double>(total_candidates);

        double sum_of_squares_with_variance = 0.0;
        for (size_t candidate_index = 0; candidate_index < total_candidates; ++candidate_index) {
            const double membership_degree =
                candidate_relations.at(candidate_index).relation.get_membership_degree();
            const double difference = membership_degree - mean;
            sum_of_squares_with_variance += (difference * difference) *
                                           hypotheses_.at(candidate_index).probability * variance_weight;
        }
        const double variance_avg = sum_of_squares_with_variance / static_cast<double>(total_candidates);

        questions_index_variance.emplace_back(question_index, variance_avg);
    }

    std::ranges::sort(questions_index_variance,
        [](const auto& left_question, const auto& right_question) -> bool {
            return left_question.second > right_question.second;
        });

    size_t drawn_number_position = 0;
    if (asked_questions_.empty()) {
        size_t max_drawn =
            std::min(questions_index_variance.size(), static_cast<size_t>(max_questions_drawn));

        std::random_device random_device;
        std::mt19937 random_generator(random_device());
        std::uniform_int_distribution<> distrib(0, static_cast<int>(max_drawn) - 1);
        drawn_number_position = static_cast<size_t>(distrib(random_generator));
    }

    chosen_index = questions_index_variance.at(drawn_number_position).first;

    return chosen_index;
}

auto ProbabilityEngine::select_question_by_outliers() const -> std::optional<std::size_t> {
    size_t best_index = -1;

    if (matrix_ == nullptr) {
        return best_index;
    }

    double highest_variance = -1.0;

    for (size_t question_index = 0; question_index < matrix_->size(); ++question_index) {
        if (asked_questions_.contains(question_index)) {
            continue;
        }

        std::vector<std::pair<const size_t, const CandidateRelation*>> candidate_relations_outliers;
        candidate_relations_outliers.reserve(strong_candidates_.indexed_hypotheses.size());
        
        const auto& more_strong_hypothesis = strong_candidates_.indexed_hypotheses.front();

        for (const auto& [index, hypothesis] : strong_candidates_.indexed_hypotheses) {
            const auto& candidate_relation =
                matrix_->at(question_index).candidate_relations.at(index);
            candidate_relations_outliers.emplace_back(index, &candidate_relation);
        }

        const size_t total_candidates = candidate_relations_outliers.size();
        if (total_candidates == 0) {
            continue;
        }

        double sum = 0.0;
        for (const auto& candidate_relation : candidate_relations_outliers) {
            sum += candidate_relation.second->relation.get_membership_degree();
        }
        const double mean = sum / static_cast<double>(total_candidates);

        double sum_of_squares_with_variance = 0.0;
        for (size_t strong_candidate_index = 0; strong_candidate_index < total_candidates; ++strong_candidate_index) {
            auto [candidate_index, candidate_relation_ptr] =
                candidate_relations_outliers.at(strong_candidate_index);
            const double membership_degree = candidate_relation_ptr->relation.get_membership_degree();

            const double factor_to_apply_more_stongger =
                more_strong_hypothesis.first == candidate_index ? 2.0 : 1.0;

            const double difference = membership_degree - mean;
            sum_of_squares_with_variance += (difference * difference) *
                                            factor_to_apply_more_stongger *
                                            hypotheses_.at(candidate_index).probability *
                                            variance_weight;
        }
        const double variance_avg =
            sum_of_squares_with_variance / static_cast<double>(total_candidates);

        if (variance_avg > highest_variance) {
            highest_variance = variance_avg;
            best_index = question_index;
        }
    }

    return best_index;
}