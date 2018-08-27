// Header
#include "Objective.hpp"

namespace QP {

Objective::Objective(const QuadraticForm& q) :
  m_quadraticForm(q)
{
}

Objective Objective::Minimize(const QuadraticForm& q, double weight) {
  return Objective(weight * q);
}

Objective Objective::Maximize(const QuadraticForm& q, double weight) {
  return Objective(-weight * q);
}

Objective Objective::Value(const LinearForm& v, double target, double weight) {
  return Minimize((v - target) * (v - target), weight);
}

const QuadraticForm& Objective::getQuadraticForm() const {
  return m_quadraticForm;
}

} // Namespace
