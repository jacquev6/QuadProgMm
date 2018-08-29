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


  LinearForm::LinearForm(double d) :
    linearCoefficients(),
    constantCoefficient(d)
  {
  }

  LinearForm::LinearForm(const Variable& v) :
    linearCoefficients(),
    constantCoefficient(0)
  {
    linearCoefficients[v] = 1;
  }

  LinearForm& LinearForm::operator += (const LinearForm& other) {
    foreach(auto c, other.linearCoefficients) {
      linearCoefficients[c.first] += c.second;
    }
    constantCoefficient += other.constantCoefficient;
    return *this;
  }

  LinearForm& LinearForm::operator -= (const LinearForm& other) {
    return *this += -other;
  }

  LinearForm& LinearForm::operator *= (double d) {
    foreach(auto c, linearCoefficients) {
      linearCoefficients[c.first] *= d;
    }
    constantCoefficient *= d;
    return *this;
  }

  LinearForm& LinearForm::operator /= (double d) {
    return *this *= 1/d;
  }

  LinearForm operator - (const LinearForm& l) {
    return l * -1;
  }

  LinearForm operator + (const LinearForm& l) {
    return l;
  }

  LinearForm operator - (const Variable& v) {
    return -LinearForm(v);
  }

  LinearForm operator + (const Variable& v) {
    return LinearForm(v);
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

  std::set<Variable> LinearForm::getVariables() const {
    std::set<Variable> variables;
    foreach(auto c, linearCoefficients) {
      variables.insert(c.first);
    }
    return variables;
  }

  double LinearForm::getLinearCoefficient(const Variable& v) const {
    auto it = linearCoefficients.find(v);
    if (it == linearCoefficients.end()) {
      return 0;
    } else {
      return it->second;
    }
  }

  double LinearForm::getConstantCoefficient() const {
    return constantCoefficient;
  }

  QuadraticForm operator * (const LinearForm& l, const LinearForm& r) {
    return QuadraticForm(l, r);
  }

  LinearForm operator - (double l, const LinearForm& r) {
    return -(r - l);
  }


  QuadraticForm::QuadraticForm(const LinearForm& l) :
    quadraticCoefficients(),
    linearForm(l)
  {
  }

  QuadraticForm::QuadraticForm(const Variable& v) :
    quadraticCoefficients(),
    linearForm(v)
  {
  }

  QuadraticForm::QuadraticForm(double d) :
    quadraticCoefficients(),
    linearForm(d)
  {
  }

  QuadraticForm::QuadraticForm(const LinearForm& l, const LinearForm& r) :
    quadraticCoefficients(),
    linearForm(l.constantCoefficient * r + r.constantCoefficient * l - l.constantCoefficient * r.constantCoefficient)
  {
    foreach(auto lc, l.linearCoefficients) {
      foreach(auto rc, r.linearCoefficients) {
        quadraticCoefficients[std::minmax(lc.first, rc.first)] += lc.second * rc.second;
      }
    }
  }

  std::set<Variable> QuadraticForm::getVariables() const {
    std::set<Variable> variables = linearForm.getVariables();
    foreach(auto c, quadraticCoefficients) {
      variables.insert(c.first.first);
      variables.insert(c.first.second);
    }
    return variables;
  }

  double QuadraticForm::getQuadraticCoefficient(const Variable& v1, const Variable& v2) const {
    auto it = quadraticCoefficients.find(std::minmax(v1, v2));
    if (it == quadraticCoefficients.end()) {
      return 0;
    } else {
      return it->second;
    }
  }

  double QuadraticForm::getLinearCoefficient(const Variable& v) const {
    return linearForm.getLinearCoefficient(v);
  }

  double QuadraticForm::getConstantCoefficient() const {
    return linearForm.getConstantCoefficient();
  }

  QuadraticForm& QuadraticForm::operator += (const QuadraticForm& other) {
    foreach(auto c, other.quadraticCoefficients) {
      quadraticCoefficients[c.first] += c.second;
    }
    linearForm += other.linearForm;
    return *this;
  }

  QuadraticForm& QuadraticForm::operator -= (const QuadraticForm& other) {
    return *this += -other;
  }

  QuadraticForm& QuadraticForm::operator *= (double d) {
    foreach(auto c, quadraticCoefficients) {
      quadraticCoefficients[c.first] *= d;
    }
    linearForm *= d;
    return *this;
  }

  QuadraticForm& QuadraticForm::operator /= (double d) {
    return *this *= 1/d;
  }

  QuadraticForm operator - (const QuadraticForm& q) {
    return q * -1;
  }

  QuadraticForm operator + (const QuadraticForm& q) {
    return q;
  }

  Constraint::Constraint(const LinearForm& linearForm_, Type type_) :
    linearForm(linearForm_),
    type(type_)
  {
  }

  Constraint operator == (const LinearForm& l, const LinearForm& r) {
    return Constraint(l - r, Constraint::ZERO);
  }

  Constraint operator >= (const LinearForm& l, const LinearForm& r) {
    return Constraint(l - r, Constraint::POSITIVE);
  }

  Constraint operator <= (const LinearForm& l, const LinearForm& r) {
    return Constraint(r - l, Constraint::POSITIVE);
  }

  const LinearForm& Constraint::getLinearForm() const {
    return linearForm;
  }

  Constraint::Type Constraint::getType() const {
    return type;
  }
}
