#ifndef QP_LinearForm_hpp
#define QP_LinearForm_hpp

// Standard library
#include <map>

// Boost
#include <boost/operators.hpp>

// QP
#include "Variable.hpp"

namespace QP {

class LinearForm :
  boost::addable<LinearForm>,
  boost::subtractable<LinearForm>,
  boost::multipliable2<LinearForm, double>,
  boost::dividable2<LinearForm, double>
{
  typedef Variable::ResolvablePtr VarPtr;

public:
  LinearForm(double);
  LinearForm(VarPtr);

public:
  double getValue() const;

public:
  LinearForm& operator *= (double);
  LinearForm& operator /= (double);
  LinearForm& operator += (const LinearForm&);
  LinearForm& operator -= (const LinearForm&);

public:
  LinearForm operator - () const;

public:
  typedef std::pair<VarPtr, double> Coefficient;
  typedef std::map<VarPtr, double> Coefficients;
  const Coefficients& getCoefficients() const;
  double getConstant() const;

public:
  friend std::ostream& operator << (std::ostream&, const LinearForm&);

private:
  Coefficients m_coefficients;
  double m_const;
};

QuadraticForm operator * (const LinearForm&, const LinearForm&);

} // Namespace

#endif // Include guard
