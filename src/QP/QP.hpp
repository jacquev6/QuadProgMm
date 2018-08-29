#ifndef QP_QP_hpp
#define QP_QP_hpp

// Boost
#include <boost/optional.hpp>

// QP
#include "Forms.hpp"

namespace QP {

class Solution {
  public:
    double getCost() const {
      return cost;
    }

    boost::optional<double> get(const Variable& v) const {
      const std::map<Variable, double>::const_iterator it = values.find(v);
      if (it == values.end()) {
        return boost::optional<double>();
      } else {
        return it->second;
      }
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

Solution minimize(const QuadraticForm&, const std::vector<Constraint>& = std::vector<Constraint>());

Solution maximize(const QuadraticForm&, const std::vector<Constraint>& = std::vector<Constraint>());

} // Namespace

#endif // Include guard
