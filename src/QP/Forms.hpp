#ifndef QP_Forms_hpp
#define QP_Forms_hpp

// Standard library
#include <map>

// Boost
#include <boost/operators.hpp>
#include <boost/optional.hpp>
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

class Variable::Resolvable {
  public:
    Resolvable();
    Resolvable(const Resolvable&);

  public:
    void setValue(double value);
    bool isResolved() const;
    double getValue() const;

  private:
    boost::optional<double> m_value;
};

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

  public: // @todo Why public?
    double getValue() const;

  public:
    LinearForm& operator *= (double);
    LinearForm& operator /= (double);
    LinearForm& operator += (const LinearForm&);
    LinearForm& operator -= (const LinearForm&);

  public:
    LinearForm operator - () const;
    LinearForm operator + () const;

  public:
    typedef std::pair<VarPtr, double> Coefficient;
    typedef std::map<VarPtr, double> Coefficients;
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
