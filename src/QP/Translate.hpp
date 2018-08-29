#ifndef QP_Translate_hpp
#define QP_Translate_hpp

// Boost
#include <boost/bimap.hpp>

// QuadProg++
#include <QuadProg++.hh>
#undef det
#undef inverse
#undef solve

// QP
#include "Forms.hpp"

namespace QP {

typedef boost::bimap<Variable, int> Variables;

struct Translation {
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
  Variables variables;
  double baseCost;
  int n, m, p;
  quadprogpp::Matrix<double> G, CE, CI;
  quadprogpp::Vector<double> G0, CE0, CI0;
};

Translation translate(const QuadraticForm&, const std::vector<Constraint>&);

} // Namespace

#endif // Include guard
