#include <algorithm>

#include "session_controller.hpp"
#include "ui_console.hpp"

SessionController::SessionController(ProbabilityEngine& probability_engine,
                                     std::vector<QuestionRow>& matrix, bool debug_mode)
    : probability_engine_(&probability_engine), matrix_(&matrix), debug_mode_(debug_mode) {}

void SessionController::run() {
    auto has_candidate_suggestion = false;
    while (!has_candidate_suggestion) {
        const auto question_index = probability_engine_->get_next_question_index();

        if (!question_index.has_value()) {
            ConsoleUI::print_give_up_message();
            break;
        }

        const std::string question_text = matrix_->at(question_index.value()).question.get_text();
        ConsoleUI::print_message_new_line(question_text);

        const double answer = ConsoleUI::read_answer();

        const ProbabilityEngine::ProbabilityParameters params{
            .question_index = question_index.value(),
            .user_answer = answer
        };

        probability_engine_->update_probabilities(params);
        probability_engine_->register_question_asked(question_index.value());
        
        if (debug_mode_) {
            print_debug_candidates_info(*probability_engine_);
        }
        
        has_candidate_suggestion = probability_engine_->has_candidate_suggestion();
        if (has_candidate_suggestion) {
            const std::string top_candidate = probability_engine_->get_top_candidate_text();
            ConsoleUI::print_suggestion_message(top_candidate);

            if (ConsoleUI::read_yes_no()) {
                ConsoleUI::print_success_message(top_candidate);
                break;
            } 
            
            probability_engine_->eliminate_top_candidate();
            has_candidate_suggestion = false;
        }
    }
}

void SessionController::print_debug_candidates_info(const ProbabilityEngine& probability_engine) {
    const auto& hypotheses_map = probability_engine.get_hypotheses();
    using MapElement = std::pair<const size_t, ProbabilityEngine::Hypothesis>;
    std::vector<std::reference_wrapper<const MapElement>> ranking(hypotheses_map.begin(),
                                                                  hypotheses_map.end());
    std::ranges::sort(ranking, [](const MapElement& first, const MapElement& second) -> bool {
            return first.second.probability < second.second.probability;
        });

    for (auto const& hypothesis : ranking) {
        const auto& candidate = hypothesis.get().second;
        ConsoleUI::print_candidate_probalility_debug(candidate.candidate->get_text(),
                                                     candidate.probability);
    }
    ConsoleUI::print_empty_line();
}