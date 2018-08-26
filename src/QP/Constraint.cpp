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

Constraint Constraint::Value(const LinearForm& l, double value) {
  return l == value;
}

Constraint Constraint::MinimumValue(const LinearForm& l, double value) {
  return l >= value;
}

Constraint Constraint::MaximumValue(const LinearForm& l, double value) {
  return l <= value;
}

Constraint Constraint::Difference(const LinearForm& l, const LinearForm& r, double value) {
  return l - r == value;
}

Constraint Constraint::MinimumDifference(const LinearForm& l, const LinearForm& r, double value) {
  return l - r >= value;
}

Constraint Constraint::MaximumDifference(const LinearForm& l, const LinearForm& r, double value) {
  return l - r <= value;
}

const LinearForm& Constraint::getLinearForm() const {
  return m_linearForm;
}

bool Constraint::isEquality() const {
  return m_isEquality;
}

} // Namespace
