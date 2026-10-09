#include "relation.hpp"

Relation::Relation(const double& membership_degree) : membership_degree_(membership_degree) {}

auto Relation::get_membership_degree() const -> double {
    return membership_degree_;
}
