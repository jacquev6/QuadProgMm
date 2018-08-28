// Header
#include "Constraint.hpp"

namespace QP {

Constraint::Constraint(const LinearForm& l, bool isEquality) :
  m_linearForm(l),
  m_isEquality(isEquality)
{
}

Constraint operator == (const LinearForm& l, const LinearForm& r) {
  return Constraint(l - r, true);
}

Constraint operator >= (const LinearForm& l, const LinearForm& r) {
  return Constraint(l - r, false);
}

Constraint operator <= (const LinearForm& l, const LinearForm& r) {
  return Constraint(r - l, false);
}

const LinearForm& Constraint::getLinearForm() const {
  return m_linearForm;
}

bool Constraint::isEquality() const {
  return m_isEquality;
}

} // Namespace
