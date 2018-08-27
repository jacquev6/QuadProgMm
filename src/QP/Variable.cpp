// Header
#include "Variable.hpp"

// QP
#include "VariableResolvable.hpp"
#include "QuadraticForm.hpp"

namespace QP {

Variable::Variable(const std::string& name) :
  m_resolvable(new Resolvable(name))
{
}

double Variable::getValue() const {
  return m_resolvable->getValue();
}

std::ostream& operator << (std::ostream& s, const Variable& variable) {
  if(variable.m_resolvable->isResolved()) {
    s << variable.m_resolvable->getValue();
  } else {
    s << "???";
  }
  return s;
}

Variable::operator LinearForm () const {
  return LinearForm(m_resolvable);
}

Variable::operator QuadraticForm () const {
  return QuadraticForm(LinearForm(m_resolvable));
}

LinearForm Variable::operator - () const {
  return -LinearForm(m_resolvable);
}

LinearForm Variable::operator + () const {
  return LinearForm(m_resolvable);
}

LinearForm operator + (const Variable& l, const Variable& r) {
  return LinearForm(l.m_resolvable) + LinearForm(r.m_resolvable);
}

LinearForm operator + (const Variable& l, double r) {
  return LinearForm(l.m_resolvable) + r;
}

LinearForm operator + (double l, const Variable& r) {
  return l + LinearForm(r.m_resolvable);
}

LinearForm operator - (const Variable& l, const Variable& r) {
  return LinearForm(l.m_resolvable) - LinearForm(r.m_resolvable);
}

LinearForm operator - (const Variable& l, double r) {
  return LinearForm(l.m_resolvable) - r;
}

LinearForm operator - (double l, const Variable& r) {
  return l - LinearForm(r.m_resolvable);
}

QuadraticForm operator * (const Variable& l, const Variable& r) {
  return QuadraticForm(l.m_resolvable, r.m_resolvable);
}

LinearForm operator * (const Variable& l, double r) {
  return LinearForm(l.m_resolvable) * r;
}

LinearForm operator / (const Variable& l, double r) {
  return LinearForm(l.m_resolvable) / r;
}

LinearForm operator * (double l, const Variable& r) {
  return l * LinearForm(r.m_resolvable);
}

} // Namespace
