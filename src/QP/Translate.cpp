// Header
#include "Translate.hpp"

// Boost
#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH

namespace QP {

Translation translate(const QuadraticForm& q, const std::vector<Constraint>& constraints) {
  Translation t;

  int equalityConstraints = 0;
  int inequalityConstraints = 0;

  {
    int index = 0;

    foreach(QuadraticForm::Coefficient c, q.getCoefficients()) {
      if(t.variables.left.find(c.first.first) == t.variables.left.end()) {
        t.variables.insert(Variables::value_type(c.first.first, index));
        ++index;
      }
      if(t.variables.left.find(c.first.second) == t.variables.left.end()) {
        t.variables.insert(Variables::value_type(c.first.second, index));
        ++index;
      }
    }
    const LinearForm& l = q.getLinearForm();
    foreach(LinearForm::Coefficient c, l.getCoefficients()) {
      if(t.variables.left.find(c.first) == t.variables.left.end()) {
        t.variables.insert(Variables::value_type(c.first, index));
        ++index;
      }
    }

    foreach(Constraint c, constraints) {
      if(c.isEquality()) {
        ++equalityConstraints;
      } else {
        ++inequalityConstraints;
      }
      const LinearForm& l = c.getLinearForm();
      foreach(LinearForm::Coefficient c, l.getCoefficients()) {
        if(t.variables.left.find(c.first) == t.variables.left.end()) {
          t.variables.insert(Variables::value_type(c.first, index));
          ++index;
        }
      }
    }
  }

  t.n = t.variables.size();
  t.p = equalityConstraints;
  t.m = inequalityConstraints;

  t.G.resize(t.n, t.n);
  t.G0.resize(t.n);
  t.CE.resize(t.n, t.p);
  t.CE0.resize(t.p);
  t.CI.resize(t.n, t.m);
  t.CI0.resize(t.m);

  for(int i = 0; i < t.n; ++i) {
    for(int j = 0; j < t.n; ++j) {
      t.G0[j] = 0;
      t.G[i][j] = 0;
    }
    for(int j = 0; j < t.p; ++j) {
      t.CE0[j] = 0;
      t.CE[i][j] = 0;
    }
    for(int j = 0; j < t.m; ++j) {
      t.CI0[j] = 0;
      t.CI[i][j] = 0;
    }
  }

  t.baseCost = 0;

  foreach(QuadraticForm::Coefficient c, q.getCoefficients()) {
    int v1 = t.variables.left.find(c.first.first)->second;
    int v2 = t.variables.left.find(c.first.second)->second;
    double coeff = c.second;

    t.G[v1][v2] += coeff;
    t.G[v2][v1] += coeff;
  }
  const LinearForm& l = q.getLinearForm();
  foreach(LinearForm::Coefficient c, l.getCoefficients()) {
    int v = t.variables.left.find(c.first)->second;
    double coeff = c.second;

    t.G0[v] += coeff;
  }
  t.baseCost += l.getConstant();

  int index = 0;
  foreach(Constraint c, constraints) {
    quadprogpp::Matrix<double>& C = c.isEquality() ? t.CE : t.CI;
    quadprogpp::Vector<double>& C0 = c.isEquality() ? t.CE0 : t.CI0;

    const LinearForm& l = c.getLinearForm();
    foreach(LinearForm::Coefficient c, l.getCoefficients()) {
      int v = t.variables.left.find(c.first)->second;
      double coeff = c.second;

      C[v][index] = coeff;
    }
    C0[index] = l.getConstant();
    ++index;
  }

  return t;
}

} // Namespace
