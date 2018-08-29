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

    std::set<Variable> allVariables = q.getVariables();
    foreach(Constraint c, constraints) {
      switch(c.getType()) {
        case Constraint::ZERO:
          ++equalityConstraints;
          break;
        case Constraint::POSITIVE:
          ++inequalityConstraints;
          break;
      }
      const std::set<Variable> constraintVariables = c.getLinearForm().getVariables();
      allVariables.insert(constraintVariables.begin(), constraintVariables.end());
    }

    t.variables = std::vector<Variable>(allVariables.begin(), allVariables.end());

    int n = t.variables.size();
    int p = equalityConstraints;
    int m = inequalityConstraints;

    t.G.resize(n, n);
    t.g0.resize(n);
    t.CE.resize(n, p);
    t.ce0.resize(p);
    t.CI.resize(n, m);
    t.ci0.resize(m);

    for(int i = 0; i < n; ++i) {
      for(int j = 0; j < n; ++j) {
        t.g0[j] = 0;
        t.G[i][j] = 0;
      }
      for(int j = 0; j < p; ++j) {
        t.ce0[j] = 0;
        t.CE[i][j] = 0;
      }
      for(int j = 0; j < m; ++j) {
        t.ci0[j] = 0;
        t.CI[i][j] = 0;
      }
    }

    t.g00 = 0;

    {
      int index1 = 0;
      foreach(Variable v1, t.variables) {
        int index2 = 0;
        foreach(Variable v2, t.variables) {
          const double coeff = q.getQuadraticCoefficient(v1, v2);
          if (index1 == index2) {
            t.G[index1][index1] = 2 * coeff;
          } else {
            t.G[index1][index2] = coeff;
            t.G[index2][index1] = coeff;
          }
          ++index2;
        }
        ++index1;
      }
    }

    {
      int index = 0;
      foreach(Variable v, t.variables) {
        t.g0[index] = q.getLinearCoefficient(v);
        ++index;
      }
    }

    t.g00 += q.getConstantCoefficient();

    int indexE = 0;
    int indexI = 0;
    foreach(Constraint c, constraints) {
      quadprogpp::Matrix<double>* CIE;
      quadprogpp::Vector<double>* cie0;
      int* indexIE;
      switch(c.getType()) {
        case Constraint::ZERO:
          CIE = &t.CE;
          cie0 = &t.ce0;
          indexIE = &indexE;
          break;
        case Constraint::POSITIVE:
          CIE = &t.CI;
          cie0 = &t.ci0;
          indexIE = &indexI;
          break;
      }

      const LinearForm& l = c.getLinearForm();
      {
        int index = 0;
        foreach(Variable v, t.variables) {
          (*CIE)[index][*indexIE] = l.getLinearCoefficient(v);
          ++index;
        }
      }

      (*cie0)[*indexIE] = l.getConstantCoefficient();
      ++(*indexIE);
    }

    return t;
  }
}
