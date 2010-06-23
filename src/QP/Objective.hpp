#ifndef QP_Objective_hpp
#define QP_Objective_hpp

// Standard library
#include <vector>

// QP
#include "QuadraticForm.hpp"

namespace QP {

class Objective {
public:
    static Objective Minimize( const QuadraticForm&, double weight = 1 );
    static Objective Maximize( const QuadraticForm&, double weight = 1 );
    static Objective Value( const LinearForm&, double target, double weight = 1 );
    static Objective Difference( const LinearForm&, const LinearForm&, double target, double weight = 1 );

private:
    Objective( const QuadraticForm& );

public:
    const QuadraticForm& getQuadraticForm() const;

private:
    QuadraticForm m_quadraticForm;
};

typedef std::vector< Objective > Objectives;

} // Namespace

#endif // Include guard
