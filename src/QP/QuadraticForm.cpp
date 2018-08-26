// Header
#include "QuadraticForm.hpp"

// Boost
#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH

// QP
#include "VariableResolvable.hpp"

namespace QP {

QuadraticForm::QuadraticForm(const LinearForm& l) :
  m_linearForm(l),
  m_coefficients()
{
}

QuadraticForm::QuadraticForm(double d) :
  m_linearForm(d),
  m_coefficients()
{
}

QuadraticForm::QuadraticForm(const LinearForm& l, const LinearForm& r) :
  m_linearForm(l.getConstant() * r + r.getConstant() * l - l.getConstant() * r.getConstant()),
  m_coefficients()
{
  foreach(LinearForm::Coefficient lc, l.getCoefficients()) {
    foreach(LinearForm::Coefficient rc, r.getCoefficients()) {
      m_coefficients[std::make_pair(lc.first, rc.first)] = lc.second * rc.second;
    }
  }
}

const QuadraticForm::Coefficients& QuadraticForm::getCoefficients() const {
  return m_coefficients;
}

std::ostream& operator << (std::ostream& s, const QuadraticForm& q) {
  foreach(QuadraticForm::Coefficient c, q.m_coefficients) {
    s << c.second << "*" << *c.first.first << "*" << *c.first.second << " + ";
  }
  s << q.m_linearForm;
  return s;
}

const LinearForm& QuadraticForm::getLinearForm() const {
  return m_linearForm;
}

QuadraticForm& QuadraticForm::operator += (const QuadraticForm& other) {
  foreach(Coefficient c, other.m_coefficients) {
    m_coefficients[c.first] += c.second;
  }
  m_linearForm += other.m_linearForm;
  return *this;
}

QuadraticForm& QuadraticForm::operator -= (const QuadraticForm& other) {
  return *this += -other;
}

QuadraticForm& QuadraticForm::operator *= (double d) {
  foreach(Coefficient c, m_coefficients) {
    m_coefficients[c.first] *= d;
  }
  m_linearForm *= d;
  return *this;
}

QuadraticForm& QuadraticForm::operator /= (double d) {
  return *this *= 1/d;
}

QuadraticForm QuadraticForm::operator - () const {
  QuadraticForm l(*this);
  l *= -1;
  return l;
}

} // Namespace
