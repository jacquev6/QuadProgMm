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
      foreach(Variable id, allVariables) {
        t.variables.insert(Variables::value_type(id, index));
        ++index;
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

    foreach(auto it1, t.variables.left) {
      foreach(auto it2, t.variables.left) {
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

    foreach(auto it, t.variables.left) {
      t.G0[it.second] = q.getLinearCoefficient(it.first);
    }

    t.baseCost += q.getConstantCoefficient();

    int indexE = 0;
    int indexI = 0;
    foreach(Constraint c, constraints) {
      quadprogpp::Matrix<double>* C;
      quadprogpp::Vector<double>* C0;
      int* index;
      switch(c.getType()) {
        case Constraint::ZERO:
          C = &t.CE;
          C0 = &t.CE0;
          index = &indexE;
          break;
        case Constraint::POSITIVE:
          C = &t.CI;
          C0 = &t.CI0;
          index = &indexI;
          break;
      }

      const LinearForm& l = c.getLinearForm();
      foreach(auto it, t.variables.left) {
        (*C)[it.second][*index] = l.getLinearCoefficient(it.first);
      }

      (*C0)[*index] = l.getConstantCoefficient();
      ++(*index);
    }

    return t;
  }
}
