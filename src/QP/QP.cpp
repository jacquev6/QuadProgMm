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
    double cost = quadprogpp::solve_quadprog(t.G, t.g0, t.CE, t.ce0, t.CI, t.ci0, x);

    std::map<Variable, double> values;
    {
      int index = 0;
      foreach(Variable v, t.variables) {
        values[v] = x[index];
        ++index;
      }
    }

    return Solution(cost + t.g00, values);
  }

  Solution maximize(const QuadraticForm& q, const std::vector<Constraint>& constraints) {
    return minimize(-q, constraints);
  }
}
