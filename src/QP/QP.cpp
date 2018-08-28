// Header
#include "QP.hpp"

// Boost
#include <boost/bimap.hpp>
#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH

// QuadProg++
#include "QuadProg++.hh"
#undef det
#undef inverse
#undef solve

// QP
#include "Objective.hpp"
#include "Constraint.hpp"
#include "VariableResolvable.hpp"

namespace QP {

class Solver {
public:
  Solver(const std::vector<Objective>&, const std::vector<Constraint>&);
  double solve() const;

private:
  std::vector<Objective> m_objectives;
  std::vector<Constraint> m_constraints;
  typedef boost::bimap<Variable::ResolvablePtr, int> Variables;
  Variables m_variables;
  int m_equalityConstraints;
  int m_inequalityConstraints;
};

Solver::Solver(const std::vector<Objective>& objectives, const std::vector<Constraint>& constraints) :
  m_objectives(objectives),
  m_constraints(constraints),
  m_variables(),
  m_equalityConstraints(0),
  m_inequalityConstraints(0)
{
  int index = 0;
  foreach(Objective o, m_objectives) {
    const QuadraticForm& q = o.getQuadraticForm();
    foreach(QuadraticForm::Coefficient c, q.getCoefficients()) {
      if(m_variables.left.find(c.first.first) == m_variables.left.end()) {
        m_variables.insert(Variables::value_type(c.first.first, index));
        ++index;
      }
      if(m_variables.left.find(c.first.second) == m_variables.left.end()) {
        m_variables.insert(Variables::value_type(c.first.second, index));
        ++index;
      }
    }
    const LinearForm& l = q.getLinearForm();
    foreach(LinearForm::Coefficient c, l.getCoefficients()) {
      if(m_variables.left.find(c.first) == m_variables.left.end()) {
        m_variables.insert(Variables::value_type(c.first, index));
        ++index;
      }
    }
  }
  foreach(Constraint c, m_constraints) {
    if(c.isEquality()) {
      ++m_equalityConstraints;
    } else {
      ++m_inequalityConstraints;
    }
    const LinearForm& l = c.getLinearForm();
    foreach(LinearForm::Coefficient c, l.getCoefficients()) {
      if(m_variables.left.find(c.first) == m_variables.left.end()) {
        m_variables.insert(Variables::value_type(c.first, index));
        ++index;
      }
    }
  }
}

double Solver::solve() const {
  /* From QuadProg++:

  The problem is in the form:

  min 0.5 * x G x + g0 x
  s.t.
      CE^T x + ce0 = 0
      CI^T x + ci0 >= 0
     
   The matrix and vectors dimensions are as follows:
       G: n * n
      g0: n
          
      CE: n * p
     ce0: p
          
      CI: n * m
     ci0: m

       x: n
  */
 
  int n = m_variables.size();
  int m = m_equalityConstraints;
  int p = m_inequalityConstraints;

  quadprogpp::Matrix<double> G, CE, CI;
  quadprogpp::Vector<double> G0, CE0, CI0, x;
  G.resize(n, n);
  G0.resize(n);
  CE.resize(n, m);
  CE0.resize(m);
  CI.resize(n, p);
  CI0.resize(p);

  for(int i = 0; i < n; ++i) {
    for(int j = 0; j < n; ++j) {
      G0[j] = 0;
      G[i][j] = 0;
    }
    for(int j = 0; j < m; ++j) {
      CE0[j] = 0;
      CE[i][j] = 0;
    }
    for(int j = 0; j < p; ++j) {
      CI0[j] = 0;
      CI[i][j] = 0;
    }
  }

  double baseCost = 0;

  foreach(Objective o, m_objectives) {
    const QuadraticForm& q = o.getQuadraticForm();
    foreach(QuadraticForm::Coefficient c, q.getCoefficients()) {
      int v1 = m_variables.left.find(c.first.first)->second;
      int v2 = m_variables.left.find(c.first.second)->second;
      double coeff = c.second;

      G[v1][v2] += coeff;
      G[v2][v1] += coeff;
    }
    const LinearForm& l = q.getLinearForm();
    foreach(LinearForm::Coefficient c, l.getCoefficients()) {
      int v = m_variables.left.find(c.first)->second;
      double coeff = c.second;

      G0[v] += coeff;
    }
    baseCost += l.getConstant();
  }

  int index = 0;
  foreach(Constraint c, m_constraints) {
    quadprogpp::Matrix<double>& C = c.isEquality() ? CE : CI;
    quadprogpp::Vector<double>& C0 = c.isEquality() ? CE0 : CI0;

    const LinearForm& l = c.getLinearForm();
    foreach(LinearForm::Coefficient c, l.getCoefficients()) {
      int v = m_variables.left.find(c.first)->second;
      double coeff = c.second;

      C[v][index] = coeff;
    }
    C0[index] = l.getConstant();
    ++index;
  }

  double cost = quadprogpp::solve_quadprog(G, G0, CE, CE0, CI, CI0, x);

  for(int i = 0; i < n; ++i) {
    assert(m_variables.right.find(i) != m_variables.right.end());
    m_variables.right.find(i)->second->setValue(x[i]);
  }

  return cost + baseCost;
}

Solution solve(const std::vector<Objective>& objectives, const std::vector<Constraint>& constraints) {
  return Solution(Solver(objectives, constraints).solve());
}

} // Namespace
