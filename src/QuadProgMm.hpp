#ifndef QuadProgMm_QuadProgMm_hpp
#define QuadProgMm_QuadProgMm_hpp

// Standard library
#include <vector>

// QuadProgMm
#include "Forms.hpp"

//! The namespace
namespace QuadProgMm {
  //! The result of an optimization
  /*! Returned by minimize and maximize */
  struct Solution {
    //! Get the optimal value of the QuadraticForm
    double getCost() const {return cost;}

    //! Get the value a variable must take to reach the optimum
    // Behavior is undefined when calling with a variable that was not part of the problem
    double get(const Variable& v) const {return values.find(v)->second;}

    public:
      Solution(double cost_, const std::map<Variable, double>& values_) : cost(cost_), values(values_) {}

    private:
      const double cost;
      std::map<Variable, double> values;
  };

  //! Minimize a QuadraticForm subject to Constraints
  Solution minimize(const QuadraticForm&, const std::vector<Constraint>& = std::vector<Constraint>());

  //! Maximize a QuadraticForm subject to Constraints
  Solution maximize(const QuadraticForm&, const std::vector<Constraint>& = std::vector<Constraint>());
}

#endif // Include guard
