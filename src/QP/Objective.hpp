#ifndef QP_Objective_hpp
#define QP_Objective_hpp

// Standard library
#include <vector>

// QP
#include "QuadraticForm.hpp"

namespace QP {

class Objective {
public:
  // @todo Simplify: remove weights
  static Objective Minimize(const QuadraticForm&, double weight = 1);
  static Objective Maximize(const QuadraticForm&, double weight = 1);
  // @todo Remove?
  static Objective Value(const LinearForm&, double target, double weight = 1);

private:
  Objective(const QuadraticForm&);

public:  // @todo Why public?
  const QuadraticForm& getQuadraticForm() const;

private:
  QuadraticForm m_quadraticForm;
};

} // Namespace

#endif // Include guard
