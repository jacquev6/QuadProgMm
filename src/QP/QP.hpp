#ifndef QP_QP_hpp
#define QP_QP_hpp

// Standard library
#include <vector>

// QP
#include "Forms.hpp"

namespace QP {
  class Solution {
    public:
      double getCost() const {
        return cost;
      }

      // Behavior is undefined when calling with a variable that was not part of the problem
      double get(const Variable& v) const {
        return values.find(v)->second;
      }

    public:
      Solution(double cost_, const std::map<Variable, double>& values_) :
        cost(cost_),
        values(values_)
      {
      }

    private:
      const double cost;
      std::map<Variable, double> values;
  };

  Solution minimize(const QuadraticForm&, const std::vector<Constraint>& = {});

  Solution maximize(const QuadraticForm&, const std::vector<Constraint>& = {});
}

#endif // Include guard
