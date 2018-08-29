// Header
#include "Forms.hpp"

// Standard library
#include <algorithm>
#include <vector>

// Boost
#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH

namespace QP {
  int Variable::nextId = 0;

  Variable::Variable() : id(++nextId) {}

  Variable::operator QuadraticForm () const {
    return QuadraticForm(LinearForm(*this));
  }

  LinearForm Variable::operator - () const {
    return -LinearForm(*this);
  }

  LinearForm Variable::operator + () const {
    return LinearForm(*this);
  }

  LinearForm operator + (const Variable& l, const Variable& r) {
    return LinearForm(l) + LinearForm(r);
  }

  LinearForm operator + (const Variable& l, double r) {
    return LinearForm(l) + r;
  }

  LinearForm operator + (double l, const Variable& r) {
    return l + LinearForm(r);
  }

  LinearForm operator - (const Variable& l, const Variable& r) {
    return LinearForm(l) - LinearForm(r);
  }

  LinearForm operator - (const Variable& l, double r) {
    return LinearForm(l) - r;
  }

  LinearForm operator - (double l, const Variable& r) {
    return l - LinearForm(r);
  }

  QuadraticForm operator * (const Variable& l, const Variable& r) {
    return QuadraticForm(l, r);
  }

  LinearForm operator * (const Variable& l, double r) {
    return LinearForm(l) * r;
  }

  LinearForm operator / (const Variable& l, double r) {
    return LinearForm(l) / r;
  }

  LinearForm operator * (double l, const Variable& r) {
    return l * LinearForm(r);
  }

  LinearForm::LinearForm(const Variable& v) :
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

  double LinearForm::getCoefficient(const Variable& v) const {
    auto it = m_coefficients.find(v);
    if (it == m_coefficients.end()) {
      return 0;
    } else {
      return it->second;
    }
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
        m_coefficients[std::minmax(lc.first, rc.first)] += lc.second * rc.second;
      }
    }
  }

  const QuadraticForm::Coefficients& QuadraticForm::getCoefficients() const {
    return m_coefficients;
  }

  double QuadraticForm::getCoefficient(const Variable& v1, const Variable& v2) const {
    auto it = m_coefficients.find(std::minmax(v1, v2));
    if (it == m_coefficients.end()) {
      return 0;
    } else {
      return it->second;
    }
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
}
