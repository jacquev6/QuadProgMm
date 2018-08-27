#ifndef QP_QP_hpp
#define QP_QP_hpp

#include "Objective.hpp"
#include "Constraint.hpp"

namespace QP {

double solve(const std::vector<Objective>&, const std::vector<Constraint>& = std::vector<Constraint>());

} // Namespace

#endif // Include guard
