#ifndef QP_QP_hpp
#define QP_QP_hpp

#include "Forms.hpp"

namespace QP {

class Solution {
  public:
    double getCost() const {return cost;}
    double get(const Variable& v) const {
      return values.find(v.id)->second; // @todo Don't dereference values.end()
    }

  public:
    Solution(double cost_, const std::map<int, double>& values_) :
      cost(cost_),
      values(values_)
    {
    }

  private:
    const double cost;
    std::map<int, double> values;
};

Solution minimize(const QuadraticForm&, const std::vector<Constraint>& = std::vector<Constraint>());

Solution maximize(const QuadraticForm&, const std::vector<Constraint>& = std::vector<Constraint>());

} // Namespace

#endif // Include guard
