#ifndef QP_Forms_hpp
#define QP_Forms_hpp

// Standard library
#include <map>
#include <vector>

// Boost
#include <boost/operators.hpp>
#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>

namespace QP {
class QuadraticForm;
class LinearForm;
class Solution;

class Variable {
  friend class LinearForm;
  friend class Solution;

  public:
    Variable();

  public:
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
    const int id;
    static int nextId;
};

class LinearForm :
  boost::addable<LinearForm>,
  boost::subtractable<LinearForm>,
  boost::multipliable2<LinearForm, double>,
  boost::dividable2<LinearForm, double>
{
  public:
    LinearForm(double);
    LinearForm(const Variable&);

  public:
    LinearForm& operator *= (double);
    LinearForm& operator /= (double);
    LinearForm& operator += (const LinearForm&);
    LinearForm& operator -= (const LinearForm&);

  public:
    LinearForm operator - () const;
    LinearForm operator + () const;

  public:
    typedef std::pair<int, double> Coefficient;
    typedef std::map<int, double> Coefficients;
    const Coefficients& getCoefficients() const;
    double getConstant() const;

  private:
    Coefficients m_coefficients;
    double m_const;
};

QuadraticForm operator * (const LinearForm&, const LinearForm&);

class QuadraticForm :
  boost::addable<QuadraticForm>,
  boost::subtractable<QuadraticForm>,
  boost::multipliable2<QuadraticForm, double>,
  boost::dividable2<QuadraticForm, double>
{
  public:
    QuadraticForm(double);
    QuadraticForm(const LinearForm&);
    QuadraticForm(const LinearForm& a, const LinearForm& b);

  public:
    QuadraticForm& operator +=(const QuadraticForm&);
    QuadraticForm& operator -=(const QuadraticForm&);
    QuadraticForm operator -() const;
    QuadraticForm operator +() const;
    QuadraticForm& operator *=(double);
    QuadraticForm& operator /=(double);

  public:  // @todo Why public?
    typedef std::pair<std::pair<int, int>, double> Coefficient;
    typedef std::map<std::pair<int, int>, double> Coefficients;
    const Coefficients& getCoefficients() const;
    const LinearForm& getLinearForm() const;

  private:
    LinearForm m_linearForm;
    Coefficients m_coefficients;
};

class Constraint {
  public: // @todo Why public?
    const LinearForm& getLinearForm() const;
    bool isEquality() const;

  private:
    Constraint(const LinearForm&, bool isEquality);

  private:
    friend Constraint operator == (const LinearForm&, const LinearForm&);
    friend Constraint operator >= (const LinearForm&, const LinearForm&);
    friend Constraint operator <= (const LinearForm&, const LinearForm&);

  private:
    LinearForm m_linearForm;
    bool m_isEquality;
};

Constraint operator == (const LinearForm&, const LinearForm&);
Constraint operator >= (const LinearForm&, const LinearForm&);
Constraint operator <= (const LinearForm&, const LinearForm&);
} // Namespace

#endif // Include guard
