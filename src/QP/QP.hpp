#ifndef QP_QP_hpp
#define QP_QP_hpp

#include "Forms.hpp"

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

Solution minimize(const QuadraticForm&, const std::vector<Constraint>& = std::vector<Constraint>());

Solution maximize(const QuadraticForm&, const std::vector<Constraint>& = std::vector<Constraint>());

} // Namespace

#endif // Include guard
