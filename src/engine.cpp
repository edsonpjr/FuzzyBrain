#include "engine.hpp"

#include "session_controller.hpp"

Engine::Engine(bool debug_mode) : debug_mode_(debug_mode) {
    matrix_ = reader_.load_file(dataset_file_name);
    probability_engine_.initialize(matrix_, debug_mode);
}

void Engine::start() {
    if (debug_mode_) {
        ConsoleUI::print_load_questions_message({
            .questions_count = matrix_.size(), 
            .candidates_count = matrix_.at(0).candidate_relations.size()
        });
    }
    SessionController session_controller(probability_engine_, matrix_, debug_mode_);
    session_controller.run();
}
