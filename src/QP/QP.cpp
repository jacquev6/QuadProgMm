// Header
#include "QP.hpp"

// QP
#include "Problem.hpp"

namespace QP {

double solve(const std::vector<Objective>& objectives, const std::vector<Constraint>& constraints) {
  return Problem(objectives, constraints).solve();
}

} // Namespace
