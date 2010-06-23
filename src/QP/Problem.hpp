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
    Problem( const Objectives&, const Constraints& = Constraints() );
    double solve() const;

private:
    Objectives m_objectives;
    Constraints m_constraints;
    typedef boost::bimap< Variable::ResolvablePtr, int > Variables;
    Variables m_variables;
    int m_equalityConstraints;
    int m_inequalityConstraints;
};

} // Namespace

#endif // Include guard
