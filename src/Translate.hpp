#ifndef QuadProgMm_Translate_hpp
#define QuadProgMm_Translate_hpp

// Standard library
#include <vector>

// QuadProg++
#include <QuadProg++.hh>
#undef det
#undef inverse
#undef solve

// QuadProgMm
#include "QuadProgMm.hpp"

namespace QuadProgMm {
  struct Translation {
    Translation(const std::set<Variable>&, int p, int m);
    std::vector<Variable> variables;
    quadprogpp::Matrix<double> G;
    quadprogpp::Vector<double> g0;
    double g00;
    quadprogpp::Matrix<double> CE;
    quadprogpp::Vector<double> ce0;
    quadprogpp::Matrix<double> CI;
    quadprogpp::Vector<double> ci0;

    private:
      Translation(const std::vector<Variable>&, int n, int p, int m);
  };

  Translation translate(const QuadraticForm&, const std::vector<Constraint>&);
}

#endif // Include guard
