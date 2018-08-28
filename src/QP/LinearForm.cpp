// Header
#include "LinearForm.hpp"

// Boost
#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH

// QP
#include "QuadraticForm.hpp"
#include "VariableResolvable.hpp"

namespace QP {

LinearForm::LinearForm(VarPtr v) :
  m_coefficients(),
  m_const(0)
{
  m_coefficients[v] = 1;
}

LinearForm::LinearForm(double d) :
  m_coefficients(),
  m_const(d)
{
}

const LinearForm::Coefficients& LinearForm::getCoefficients() const {
  return m_coefficients;
}

double LinearForm::getConstant() const {
  return m_const;
}

QuadraticForm operator * (const LinearForm& l, const LinearForm& r) {
  return QuadraticForm(l, r);
}

LinearForm& LinearForm::operator += (const LinearForm& other) {
  foreach(Coefficient c, other.m_coefficients) {
    m_coefficients[c.first] += c.second;
  }
  m_const += other.m_const;
  return *this;
}

LinearForm& LinearForm::operator -= (const LinearForm& other) {
  return *this += -other;
}

LinearForm LinearForm::operator - () const {
  LinearForm l(*this);
  l *= -1;
  return l;
}

LinearForm LinearForm::operator + () const {
  return LinearForm(*this);
}

LinearForm operator - (double l, const LinearForm& r) {
  return -(r - l);
}

LinearForm& LinearForm::operator *= (double d) {
  foreach(Coefficient c, m_coefficients) {
    m_coefficients[c.first] *= d;
  }
  m_const *= d;
  return *this;
}

LinearForm& LinearForm::operator /= (double d) {
  return *this *= 1/d;
}

double LinearForm::getValue() const {
  double value = m_const;
  foreach(LinearForm::Coefficient c, m_coefficients) {
    value += c.second * c.first->getValue();
  }
  return value;
}

} // Namespace
