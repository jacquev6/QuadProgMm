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

    {
      int index = 0;
      foreach(Variable v, allVariables) {
        t.variables.push_back(std::make_pair(v, index));
        ++index;
      }
    }

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

    foreach(auto it1, t.variables) {
      foreach(auto it2, t.variables) {
        const int index1 = it1.second;
        const int index2 = it2.second;
        const double coeff = q.getQuadraticCoefficient(it1.first, it2.first);
        if (index1 == index2) {
          t.G[index1][index1] = 2 * coeff;
        } else {
          t.G[index1][index2] = coeff;
          t.G[index2][index1] = coeff;
        }
      }
    }

    foreach(auto it, t.variables) {
      t.g0[it.second] = q.getLinearCoefficient(it.first);
    }

    t.g00 += q.getConstantCoefficient();

    int indexE = 0;
    int indexI = 0;
    foreach(Constraint c, constraints) {
      quadprogpp::Matrix<double>* C;
      quadprogpp::Vector<double>* c0;
      int* index;
      switch(c.getType()) {
        case Constraint::ZERO:
          C = &t.CE;
          c0 = &t.ce0;
          index = &indexE;
          break;
        case Constraint::POSITIVE:
          C = &t.CI;
          c0 = &t.ci0;
          index = &indexI;
          break;
      }

      const LinearForm& l = c.getLinearForm();
      foreach(auto it, t.variables) {
        (*C)[it.second][*index] = l.getLinearCoefficient(it.first);
      }

      (*c0)[*index] = l.getConstantCoefficient();
      ++(*index);
    }

    return t;
  }
}
