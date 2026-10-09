#include <iostream>
#include <limits>

#include "engine.hpp"

auto main() -> int {
    ConsoleUI{};   
    auto choice = 0;
    while (true) {
        ConsoleUI::print_wellcome_message();
        auto debug_mode = false;
        choice = ConsoleUI::print_session_control();
        if (choice == 1) {
            Engine engine(debug_mode);
            engine.start();
        } else if (choice == 2) {
            debug_mode = true;
            Engine engine(debug_mode);
            engine.start();
        } else {
            break;
        }
    }
    return 0;
}