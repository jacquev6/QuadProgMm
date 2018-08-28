#ifndef QP_Variable_hpp
#define QP_Variable_hpp

// Boost
#include <boost/shared_ptr.hpp>

namespace QP {

class QuadraticForm;
class LinearForm;

class Variable {
public:
  Variable();

public:
  double getValue() const;

public:
  class Resolvable;
  typedef boost::shared_ptr<Resolvable> ResolvablePtr;

public:
  operator LinearForm () const; // @todo Could this implicit conversion be enough?
  operator QuadraticForm () const;
  LinearForm operator - () const;
  LinearForm operator + () const;

  friend LinearForm operator + (const Variable&, double);
  friend LinearForm operator + (double, const Variable&);
  friend LinearForm operator + (const Variable&, const Variable&);

  friend LinearForm operator - (const Variable&, double);
  friend LinearForm operator - (double, const Variable&);
  friend LinearForm operator - (const Variable&, const Variable&);

  friend LinearForm operator * (const Variable&, double);
  friend LinearForm operator / (const Variable&, double);
  friend LinearForm operator * (double, const Variable&);

  friend QuadraticForm operator * (const Variable&, const Variable&);

private:
  ResolvablePtr m_resolvable;
};

} // Namespace

#endif // Include guard
