#ifndef QP_Problem_hpp
#define QP_Problem_hpp

// Boost
#include <boost/bimap.hpp>

// QP
#include "Objective.hpp"
#include "Constraint.hpp"

namespace QP {

class Problem {
public:
  Problem(const std::vector<Objective>&, const std::vector<Constraint>&);
  double solve() const;

private:
  std::vector<Objective> m_objectives;
  std::vector<Constraint> m_constraints;
  typedef boost::bimap<Variable::ResolvablePtr, int> Variables;
  Variables m_variables;
  int m_equalityConstraints;
  int m_inequalityConstraints;
};

} // Namespace

#endif // Include guard
