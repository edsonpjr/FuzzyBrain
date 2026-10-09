#include "candidate.hpp"

Candidate::Candidate(std::string text) : text_(std::move(text)) {}

auto Candidate::get_text() const -> std::string {
    return text_;
}
