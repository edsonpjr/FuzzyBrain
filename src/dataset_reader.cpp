#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>

#include "dataset_reader.hpp"
#include "engine.hpp"

namespace fs = std::filesystem;

using std::string;
using std::vector;

auto DatasetReader::load_file(const fs::path& file_path) -> std::vector<QuestionRow> {
    vector<QuestionRow> matrix;
    auto full_file_path = file_path;
    auto file = get_file_stream(full_file_path);

    try {
        std::string line;

        // Read question count
        if (!read_line(file,line)) {
            return {};
        }
        int question_count = std::stoi(line);
        matrix.reserve(static_cast<size_t>(question_count));

        // Read questions
        questions_.clear();
        questions_.reserve(static_cast<size_t>(question_count));
        for (int i = 0; i < question_count; ++i) {
            if (!read_line(file, line)) {
                return {};
            }
            questions_.emplace_back(line);
        }

        // Read candidates count
        if (!read_line(file, line)) {
            return {};
        }
        int candidates_count = std::stoi(line);
        candidates_.clear();
        candidates_.reserve(static_cast<size_t>(candidates_count));
        for (int i = 0; i < candidates_count; ++i) {
            if (!read_line(file, line)) {
                return {};
            }
            candidates_.emplace_back(line);
        }

        // For each question, read association line
        for (const auto& question : questions_) {
            if (!read_line(file, line)) {
                return {};
            }

            auto association_values = parse_association_line(line, candidates_.size());
            if (association_values.size() != candidates_.size()) {
                std::cerr << "Error: association values count does not match candidates count for file: "
                          << fs::absolute(full_file_path) << '\n';
                return {};
            }

            std::vector<CandidateRelation> candidate_relations;
            candidate_relations.reserve(candidates_.size());
            for (size_t i = 0; i < candidates_.size(); ++i) {
                candidate_relations.push_back({
                    .candidate = candidates_.at(i),
                    .relation = Relation(association_values.at(i))
                });
            }

            matrix.push_back({
                .question = question,
                .candidate_relations = std::move(candidate_relations)
            });
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: Failed to load file: " << fs::absolute(full_file_path) << '\n'
                  << "Reason: " << e.what() << '\n';
        return {};
    }

    return matrix;
}

auto DatasetReader::set_file_full_path(fs::path& file_path) -> void {
    if (file_path.is_relative()) {
        fs::path exe_path = fs::current_path();

        if (fs::exists(exe_path / DatasetReader::resources_path / file_path)) {
            file_path = exe_path / DatasetReader::resources_path / file_path;
        } else if (fs::exists(exe_path / "build" / DatasetReader::resources_path / file_path)) {
            file_path = exe_path / "build" / DatasetReader::resources_path / file_path;
        } else {
            file_path = fs::path(DatasetReader::resources_path) / file_path;
        }
    }
}

auto DatasetReader::get_file_stream(fs::path& file_path) -> std::ifstream {
    set_file_full_path(file_path);

    if (!fs::exists(file_path)) {
        std::cerr << "Error: File not found: " << fs::absolute(file_path) << '\n';
        return {};
    }

    std::ifstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file: " << fs::absolute(file_path) << '\n';
        return {};
    }

    // Only throw on serious I/O errors
    file.exceptions(std::ifstream::badbit);

    return file;
}

auto DatasetReader::read_line(std::ifstream& file, std::string& out) -> bool {
    if (!std::getline(file, out)) {
        return false;
    }
    if (!out.empty() && out.back() == '\r') {
        out.pop_back();
    }
    return !out.empty();
}

auto DatasetReader::parse_association_line(const std::string& line, size_t expected_count) -> std::vector<double> {
    std::vector<double> association_values;
    association_values.reserve(expected_count);

    std::istringstream stream_line(line);
    double value = 0.0;
    while (stream_line >> value) {
        association_values.push_back(value  );
    }

    return association_values;
}
