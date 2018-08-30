// Header
#include "QuadProgMm.hpp"

// Standard library
#include <algorithm>
#include <vector>

// Boost
#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH

namespace QuadProgMm {
  int Variable::nextId = 0;

  Variable::Variable() : id(++nextId) {}


  LinearExpression::LinearExpression(const std::map<Variable, double>& linearCoefficients_, double constantCoefficient_) :
    linearCoefficients(linearCoefficients_),
    constantCoefficient(constantCoefficient_)
  {}

  LinearExpression::LinearExpression(double d) : LinearExpression({}, d) {}

  LinearExpression::LinearExpression(const Variable& v) : LinearExpression({std::make_pair(v, 1)}, 0) {}

  LinearExpression& LinearExpression::operator += (const LinearExpression& other) {
    foreach(auto c, other.linearCoefficients) {
      linearCoefficients[c.first] += c.second;
    }
    constantCoefficient += other.constantCoefficient;
    return *this;
  }

  LinearExpression& LinearExpression::operator -= (const LinearExpression& other) {
    foreach(auto c, other.linearCoefficients) {
      linearCoefficients[c.first] -= c.second;
    }
    constantCoefficient -= other.constantCoefficient;
    return *this;
  }

  LinearExpression& LinearExpression::operator *= (double d) {
    foreach(auto c, linearCoefficients) {
      linearCoefficients[c.first] *= d;
    }
    constantCoefficient *= d;
    return *this;
  }

  LinearExpression& LinearExpression::operator /= (double d) {
    foreach(auto c, linearCoefficients) {
      linearCoefficients[c.first] /= d;
    }
    constantCoefficient /= d;
    return *this;
  }

  std::set<Variable> LinearExpression::getVariables() const {
    std::set<Variable> variables;
    foreach(auto c, linearCoefficients) {
      variables.insert(c.first);
    }
    return variables;
  }

  double LinearExpression::getLinearCoefficient(const Variable& v) const {
    auto it = linearCoefficients.find(v);
    if (it == linearCoefficients.end()) {
      return 0;
    } else {
      return it->second;
    }
  }

  double LinearExpression::getConstantCoefficient() const {
    return constantCoefficient;
  }


  QuadraticExpression::QuadraticExpression(
    const std::map<std::pair<Variable, Variable>, double>& quadraticCoefficients_,
    const std::map<Variable, double>& linearCoefficients_,
    double constantCoefficient_
  ) :
    quadraticCoefficients(quadraticCoefficients_),
    linearCoefficients(linearCoefficients_),
    constantCoefficient(constantCoefficient_)
  {}

  QuadraticExpression::QuadraticExpression(const LinearExpression& l) : QuadraticExpression({}, l.linearCoefficients, l.constantCoefficient) {}

  QuadraticExpression::QuadraticExpression(const Variable& v) : QuadraticExpression({}, {std::make_pair(v, 1)}, 0) {}

  QuadraticExpression::QuadraticExpression(double d) : QuadraticExpression({}, {}, d) {}

  std::set<Variable> QuadraticExpression::getVariables() const {
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

  QuadraticExpression QuadraticExpression::multiply(const LinearExpression& l, const LinearExpression& r) {
    double constantCoefficient = l.constantCoefficient * r.constantCoefficient;
    std::map<Variable, double> linearCoefficients = (l.constantCoefficient * r + r.constantCoefficient * l).linearCoefficients;
    std::map<std::pair<Variable, Variable>, double> quadraticCoefficients;

    foreach(auto lc, l.linearCoefficients) {
      foreach(auto rc, r.linearCoefficients) {
        quadraticCoefficients[std::minmax(lc.first, rc.first)] += lc.second * rc.second;
      }
    }

    return QuadraticExpression(quadraticCoefficients, linearCoefficients, constantCoefficient);
  }

  double QuadraticExpression::getQuadraticCoefficient(const Variable& v1, const Variable& v2) const {
    auto it = quadraticCoefficients.find(std::minmax(v1, v2));
    if (it == quadraticCoefficients.end()) {
      return 0;
    } else {
      return it->second;
    }
  }

  double QuadraticExpression::getLinearCoefficient(const Variable& v) const {
    auto it = linearCoefficients.find(v);
    if (it == linearCoefficients.end()) {
      return 0;
    } else {
      return it->second;
    }
  }

  double QuadraticExpression::getConstantCoefficient() const {
    return constantCoefficient;
  }

  QuadraticExpression& QuadraticExpression::operator += (const QuadraticExpression& other) {
    foreach(auto c, other.quadraticCoefficients) {
      quadraticCoefficients[c.first] += c.second;
    }
    foreach(auto c, other.linearCoefficients) {
      linearCoefficients[c.first] += c.second;
    }
    constantCoefficient += other.constantCoefficient;
    return *this;
  }

  QuadraticExpression& QuadraticExpression::operator -= (const QuadraticExpression& other) {
    return *this += -other;
  }

  QuadraticExpression& QuadraticExpression::operator *= (double d) {
    foreach(auto c, quadraticCoefficients) {
      quadraticCoefficients[c.first] *= d;
    }
    foreach(auto c, linearCoefficients) {
      linearCoefficients[c.first] *= d;
    }
    constantCoefficient *= d;
    return *this;
  }

  QuadraticExpression& QuadraticExpression::operator /= (double d) {
    return *this *= 1/d;
  }


  Constraint::Constraint(const LinearExpression& linearExpression_, Type type_) :
    linearExpression(linearExpression_),
    type(type_)
  {}

  const LinearExpression& Constraint::getLinearExpression() const {return linearExpression;}

  Constraint::Type Constraint::getType() const {return type;}
}

namespace QuadProgMm {
  LinearExpression operator + (const Variable& v) {return v;}
  LinearExpression operator + (const LinearExpression& l) {return l;}
  QuadraticExpression operator + (const QuadraticExpression& q) {return q;}

  LinearExpression operator - (const Variable& v) {return v * -1;}
  LinearExpression operator - (const LinearExpression& l) {return l * -1;}
  QuadraticExpression operator - (const QuadraticExpression& q) {return q * -1;}

  LinearExpression operator + (const Variable& l, double r) {return LinearExpression(l) + r;}
  LinearExpression operator + (double l, const Variable& r) {return LinearExpression(l) + r;}
  LinearExpression operator + (const Variable& l, const Variable& r) {return LinearExpression(l) + r;}

  LinearExpression operator - (const Variable& l, double r) {return LinearExpression(l) - r;}
  LinearExpression operator - (double l, const Variable& r) {return LinearExpression(l) - r;}
  LinearExpression operator - (const Variable& l, const Variable& r) {return LinearExpression(l) - r;}

  LinearExpression operator * (const Variable& v, double d) {return d * LinearExpression(v);}
  LinearExpression operator * (double d, const Variable& v) {return d * LinearExpression(v);}

  QuadraticExpression operator * (const LinearExpression& l, const LinearExpression& r) {return QuadraticExpression::multiply(l, r);}

  LinearExpression operator / (const Variable& l, double r) {return LinearExpression(l) / r;}

  Constraint operator == (const LinearExpression& l, const LinearExpression& r) {return Constraint(l - r, Constraint::ZERO);}
  Constraint operator >= (const LinearExpression& l, const LinearExpression& r) {return Constraint(l - r, Constraint::POSITIVE);}
  Constraint operator <= (const LinearExpression& l, const LinearExpression& r) {return Constraint(r - l, Constraint::POSITIVE);}
}
