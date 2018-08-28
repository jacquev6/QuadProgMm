#ifndef QP_QuadraticForm_hpp
#define QP_QuadraticForm_hpp

// Standard library
#include <map>

// QP
#include "LinearForm.hpp"

namespace QP {

class QuadraticForm :
  boost::addable<QuadraticForm>,
  boost::subtractable<QuadraticForm>,
  boost::multipliable2<QuadraticForm, double>,
  boost::dividable2<QuadraticForm, double>
{
  typedef Variable::ResolvablePtr VarPtr;

public:
  QuadraticForm(double);
  QuadraticForm(const LinearForm&);
  QuadraticForm(const LinearForm& a, const LinearForm& b);

public:
  double getValue() const;

public:
  QuadraticForm& operator +=(const QuadraticForm&);
  QuadraticForm& operator -=(const QuadraticForm&);
  QuadraticForm operator -() const;
  QuadraticForm operator +() const;
  QuadraticForm& operator *=(double);
  QuadraticForm& operator /=(double);

public:  // @todo Why public?
  typedef std::pair<std::pair<VarPtr, VarPtr>, double> Coefficient;
  typedef std::map<std::pair<VarPtr, VarPtr>, double> Coefficients;
  const Coefficients& getCoefficients() const;
  const LinearForm& getLinearForm() const;

private:
  LinearForm m_linearForm;
  Coefficients m_coefficients;
};

} // Namespace

#endif // Include guard
