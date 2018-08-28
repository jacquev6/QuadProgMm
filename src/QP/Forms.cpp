// Header
#include "Forms.hpp"

// Boost
#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH

namespace QP {

Variable::Variable() :
  m_resolvable(new Resolvable())
{
}

double Variable::getValue() const {
  return m_resolvable->getValue();
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

Variable::Resolvable::Resolvable() :
  m_value()
{
}

void Variable::Resolvable::setValue(double value) {
  m_value = value;
}

bool Variable::Resolvable::isResolved() const {
  return (bool)m_value;
}

double Variable::Resolvable::getValue() const {
  return *m_value;
}

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

QuadraticForm QuadraticForm::operator + () const {
  return QuadraticForm(*this);
}

} // Namespace
