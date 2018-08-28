#ifndef QP_QP_hpp
#define QP_QP_hpp

#include "Objective.hpp"
#include "Constraint.hpp"

namespace QP {

class Solution {
  public:
    double getCost() const {return cost;}
    double get(const Variable& v) const {return v.getValue();}

  public:
    Solution(double cost_) : cost(cost_) {}

  private:
    const double cost;
};

Solution solve(const std::vector<Objective>&, const std::vector<Constraint>& = std::vector<Constraint>());

} // Namespace

#endif // Include guard
