#include "question.hpp"

Question::Question(std::string& text) : text_(std::move(text)) { }

auto Question::get_text() const -> std::string {
    return text_;
}
