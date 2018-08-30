// Header
#include "Translate.hpp"

// Boost
#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH

namespace QuadProgMm {
  /* From QuadProg++:

  The problem is in the expression:

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
  Translation::Translation(const std::set<Variable>& variables_, int p, int m) :
    Translation(
      std::vector<Variable>(variables_.begin(), variables_.end()),
      variables_.size(),
      p,
      m
    )
  {}

  Translation::Translation(const std::vector<Variable>& variables_, int n, int p, int m) :
    variables(variables_),
    G(n, n), g0(n), g00(0),
    CE(n, p), ce0(p),
    CI(n, m), ci0(m)
  {}

  void translate(
    const std::vector<Variable>& variables,
    const QuadraticExpression& q,
    quadprogpp::Matrix<double>& G,
    quadprogpp::Vector<double>& g0,
    double& g00
  ) {
    int index1 = 0;
    foreach(Variable v1, variables) {
      int index2 = 0;
      foreach(Variable v2, variables) {
        const double coeff = q.getQuadraticCoefficient(v1, v2);
        if (index1 == index2) {
          G[index1][index1] = 2 * coeff;
        } else {
          G[index1][index2] = coeff;
          G[index2][index1] = coeff;
        }
        ++index2;
      }
      ++index1;
    }

    int index = 0;
    foreach(Variable v, variables) {
      g0[index] = q.getLinearCoefficient(v);
      ++index;
    }

    g00 = q.getConstantCoefficient();
  }

  void translate(
    const std::vector<Variable>& variables,
    const LinearExpression& l,
    quadprogpp::Matrix<double>& CIE,
    quadprogpp::Vector<double>& cie0,
    int indexIE
  ) {
    int index = 0;
    foreach(Variable v, variables) {
      CIE[index][indexIE] = l.getLinearCoefficient(v);
      ++index;
    }

    cie0[indexIE] = l.getConstantCoefficient();
  }

  Translation translate(const QuadraticExpression& q, const std::vector<Constraint>& constraints) {
    int p = 0;
    int m = 0;
    std::set<Variable> variables = q.getVariables();
    foreach(Constraint c, constraints) {
      switch(c.getType()) {
        case Constraint::ZERO:
          ++p;
          break;
        case Constraint::POSITIVE:
          ++m;
          break;
      }
      const std::set<Variable> constraintVariables = c.getLinearExpression().getVariables();
      variables.insert(constraintVariables.begin(), constraintVariables.end());
    }

    Translation t(variables, p, m);

    translate(t.variables, q, t.G, t.g0, t.g00);

    int indexE = 0;
    int indexI = 0;
    foreach(Constraint c, constraints) {
      const LinearExpression& l = c.getLinearExpression();

      switch(c.getType()) {
        case Constraint::ZERO:
          translate(t.variables, l, t.CE, t.ce0, indexE);
          ++indexE;
          break;
        case Constraint::POSITIVE:
          translate(t.variables, l, t.CI, t.ci0, indexI);
          ++indexI;
          break;
      }
    }

    return t;
  }
}
