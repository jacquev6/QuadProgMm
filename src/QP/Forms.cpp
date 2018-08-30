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


  LinearForm::LinearForm(const std::map<Variable, double>& linearCoefficients_, double constantCoefficient_) :
    linearCoefficients(linearCoefficients_),
    constantCoefficient(constantCoefficient_)
  {}

  LinearForm::LinearForm(double d) : LinearForm({}, d) {}

  LinearForm::LinearForm(const Variable& v) : LinearForm({std::make_pair(v, 1)}, 0) {}

  LinearForm& LinearForm::operator += (const LinearForm& other) {
    foreach(auto c, other.linearCoefficients) {
      linearCoefficients[c.first] += c.second;
    }
    constantCoefficient += other.constantCoefficient;
    return *this;
  }

  LinearForm& LinearForm::operator -= (const LinearForm& other) {
    foreach(auto c, other.linearCoefficients) {
      linearCoefficients[c.first] -= c.second;
    }
    constantCoefficient -= other.constantCoefficient;
    return *this;
  }

  LinearForm& LinearForm::operator *= (double d) {
    foreach(auto c, linearCoefficients) {
      linearCoefficients[c.first] *= d;
    }
    constantCoefficient *= d;
    return *this;
  }

  LinearForm& LinearForm::operator /= (double d) {
    foreach(auto c, linearCoefficients) {
      linearCoefficients[c.first] /= d;
    }
    constantCoefficient /= d;
    return *this;
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


  QuadraticForm::QuadraticForm(
    const std::map<std::pair<Variable, Variable>, double>& quadraticCoefficients_,
    const std::map<Variable, double>& linearCoefficients_,
    double constantCoefficient_
  ) :
    quadraticCoefficients(quadraticCoefficients_),
    linearCoefficients(linearCoefficients_),
    constantCoefficient(constantCoefficient_)
  {}

  QuadraticForm::QuadraticForm(const LinearForm& l) : QuadraticForm({}, l.linearCoefficients, l.constantCoefficient) {}

  QuadraticForm::QuadraticForm(const Variable& v) : QuadraticForm({}, {std::make_pair(v, 1)}, 0) {}

  QuadraticForm::QuadraticForm(double d) : QuadraticForm({}, {}, d) {}

  std::set<Variable> QuadraticForm::getVariables() const {
    std::set<Variable> variables;
    foreach(auto c, quadraticCoefficients) {
      variables.insert(c.first.first);
      variables.insert(c.first.second);
    }
    foreach(auto c, linearCoefficients) {
      variables.insert(c.first);
    }
    return variables;
  }

  QuadraticForm QuadraticForm::multiply(const LinearForm& l, const LinearForm& r) {
    double constantCoefficient = l.constantCoefficient * r.constantCoefficient;
    std::map<Variable, double> linearCoefficients = (l.constantCoefficient * r + r.constantCoefficient * l).linearCoefficients;
    std::map<std::pair<Variable, Variable>, double> quadraticCoefficients;

    foreach(auto lc, l.linearCoefficients) {
      foreach(auto rc, r.linearCoefficients) {
        quadraticCoefficients[std::minmax(lc.first, rc.first)] += lc.second * rc.second;
      }
    }

    return QuadraticForm(quadraticCoefficients, linearCoefficients, constantCoefficient);
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
    auto it = linearCoefficients.find(v);
    if (it == linearCoefficients.end()) {
      return 0;
    } else {
      return it->second;
    }
  }

  double QuadraticForm::getConstantCoefficient() const {
    return constantCoefficient;
  }

  QuadraticForm& QuadraticForm::operator += (const QuadraticForm& other) {
    foreach(auto c, other.quadraticCoefficients) {
      quadraticCoefficients[c.first] += c.second;
    }
    foreach(auto c, other.linearCoefficients) {
      linearCoefficients[c.first] += c.second;
    }
    constantCoefficient += other.constantCoefficient;
    return *this;
  }

  QuadraticForm& QuadraticForm::operator -= (const QuadraticForm& other) {
    return *this += -other;
  }

  QuadraticForm& QuadraticForm::operator *= (double d) {
    foreach(auto c, quadraticCoefficients) {
      quadraticCoefficients[c.first] *= d;
    }
    foreach(auto c, linearCoefficients) {
      linearCoefficients[c.first] *= d;
    }
    constantCoefficient *= d;
    return *this;
  }

  QuadraticForm& QuadraticForm::operator /= (double d) {
    return *this *= 1/d;
  }


  Constraint::Constraint(const LinearForm& linearForm_, Type type_) :
    linearForm(linearForm_),
    type(type_)
  {}

  const LinearForm& Constraint::getLinearForm() const {return linearForm;}

  Constraint::Type Constraint::getType() const {return type;}
}

namespace QP {
  LinearForm operator + (const Variable& v) {return v;}
  LinearForm operator + (const LinearForm& l) {return l;}
  QuadraticForm operator + (const QuadraticForm& q) {return q;}

  LinearForm operator - (const Variable& v) {return v * -1;}
  LinearForm operator - (const LinearForm& l) {return l * -1;}
  QuadraticForm operator - (const QuadraticForm& q) {return q * -1;}

  LinearForm operator + (const Variable& l, double r) {return LinearForm(l) + r;}
  LinearForm operator + (double l, const Variable& r) {return LinearForm(l) + r;}
  LinearForm operator + (const Variable& l, const Variable& r) {return LinearForm(l) + r;}

  LinearForm operator - (const Variable& l, double r) {return LinearForm(l) - r;}
  LinearForm operator - (double l, const Variable& r) {return LinearForm(l) - r;}
  LinearForm operator - (const Variable& l, const Variable& r) {return LinearForm(l) - r;}

  LinearForm operator * (const Variable& v, double d) {return d * LinearForm(v);}
  LinearForm operator * (double d, const Variable& v) {return d * LinearForm(v);}

  QuadraticForm operator * (const LinearForm& l, const LinearForm& r) {return QuadraticForm::multiply(l, r);}

  LinearForm operator / (const Variable& l, double r) {return LinearForm(l) / r;}

  Constraint operator == (const LinearForm& l, const LinearForm& r) {return Constraint(l - r, Constraint::ZERO);}
  Constraint operator >= (const LinearForm& l, const LinearForm& r) {return Constraint(l - r, Constraint::POSITIVE);}
  Constraint operator <= (const LinearForm& l, const LinearForm& r) {return Constraint(r - l, Constraint::POSITIVE);}
}
