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

Objective Objective::Difference(const LinearForm& v1, const LinearForm& v2, double target, double weight) {
  return Minimize((v1 - v2 - target) * (v1 - v2 - target), weight);
}

const QuadraticForm& Objective::getQuadraticForm() const {
  return m_quadraticForm;
}

} // Namespace
