#include <clocale>
#include <iostream>
#include <format>
#include <limits>
#include <climits>

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif

#include "ui_console.hpp"

ConsoleUI::ConsoleUI() {
    utf8_language_settings();
}

void ConsoleUI::utf8_language_settings() {
    std::setlocale(LC_ALL, "pt_BR.UTF-8");

#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
}

void ConsoleUI::print_message_new_line(const std::string_view& message) {
    std::cout << "\n" << message;
}

void ConsoleUI::print_empty_line() {
    std::cout << "\n";
}

void ConsoleUI::print_message(const std::string_view& message) {
    std::cout << message;
}

void ConsoleUI::print_wellcome_message() {
    print_message_new_line(ConsoleUI::welcome_message);
}

auto ConsoleUI::print_session_control() -> int {
    int choice = 0;
    print_message_new_line(ConsoleUI::session_control_message);
    bool valid_input = false;

    while (!valid_input) {
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(LLONG_MAX, '\n');

            print_message(ConsoleUI::invalid_input_message);
            continue;
        }

        switch (choice) {
            case 1:
            case 2:
                print_message_new_line(ConsoleUI::starting_new_session_message);
                print_empty_line();
                valid_input = true;
                break;

            case 3:
                print_message_new_line(ConsoleUI::exiting_program_message);
                valid_input = true;
                break;

            default:
                print_message(ConsoleUI::invalid_input_message);
                continue;
        }

    }
    return choice;
}

void ConsoleUI::print_load_questions_message(const ConsoleUI::LoadedQuestionsParams& params) {
    ConsoleUI::print_message_new_line(
        std::format(
            ConsoleUI::load_questions_message, 
            params.questions_count, 
            params.candidates_count));
    ConsoleUI::print_message_new_line("");
}

auto ConsoleUI::read_answer() -> double {
    double answer = -1.0;

    print_message_new_line(ConsoleUI::input_request_message);

    while (true) {
        std::cin >> answer;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(LLONG_MAX, '\n');

            print_message(ConsoleUI::invalid_input_message);
            continue;
        }

        if (answer < 0.0 || answer > 1.0) {
            print_message(ConsoleUI::invalid_input_message);
            continue;
        }

        return answer;
    }
}

void ConsoleUI::print_suggestion_message(const std::string_view& candidate_name) {
    print_message_new_line(std::format(ConsoleUI::suggestion_message, candidate_name));
}

void ConsoleUI::print_candidate_probalility_debug(const std::string_view& candidate_name, const double probability) {
    const auto probability_percent = probability * percent_mulitplier;
    const auto candidate_info = std::format(ConsoleUI::candidate_probability_debug_message, candidate_name, probability_percent);
    print_message_new_line(candidate_info);
}

void ConsoleUI::print_strong_candidate_found(const bool pluralize) {
    const auto strong_candidate_found = std::format(ConsoleUI::strong_candidate_found_debug,
                                                   pluralize ? "s" : "", pluralize ? "s" : "");
    print_message_new_line(strong_candidate_found);
}

void ConsoleUI::print_strong_candidate_debug(const std::string_view& candidate_name, 
                                             const double probability) {
    const auto probability_percent = probability * percent_mulitplier;
    const auto strong_candidate_info = std::format(ConsoleUI::strong_candidate_probability_debug_format,
                                                    candidate_name, probability_percent);
    print_message_new_line(strong_candidate_info);
}

void ConsoleUI::print_success_message(std::string_view candidate_name) {
    print_message_new_line(std::format(ConsoleUI::success_message, candidate_name));
    print_empty_line();
}

void ConsoleUI::print_give_up_message() {
    print_message_new_line(ConsoleUI::give_up_message);
    print_empty_line();
}

auto ConsoleUI::read_yes_no() -> bool {
    std::string input;
    while (true) {
        std::cin >> input;
        if (input == "s" || input == "S") {
            return true;
        }
        if (input == "n" || input == "N") {
            return false;
        }
        print_message(ConsoleUI::invalid_yes_no_message);
    }
}
