#ifndef QuadProgMm_QuadProgMm_hpp
#define QuadProgMm_QuadProgMm_hpp

// Standard library
#include <vector>

// QuadProgMm
#include "Forms.hpp"

namespace QuadProgMm {
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
