// Header
#include "QP.hpp"

// Boost
#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH

// QP
#include "Translate.hpp"

namespace QP {

Solution minimize(const QuadraticForm& q, const std::vector<Constraint>& constraints) {
  Translation t = translate(q, constraints);

  quadprogpp::Vector<double> x;

  double cost = quadprogpp::solve_quadprog(t.G, t.G0, t.CE, t.CE0, t.CI, t.CI0, x);

  std::map<int, double> values;
  for(int i = 0; i < t.n; ++i) {
    assert(t.variables.right.find(i) != t.variables.right.end());
    values[t.variables.right.find(i)->second] = x[i];
  }

  return Solution(cost + t.baseCost, values);
}

Solution maximize(const QuadraticForm& q, const std::vector<Constraint>& constraints) {
  return minimize(-q, constraints);
}

} // Namespace
