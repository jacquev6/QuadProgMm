#ifndef QP_Forms_hpp
#define QP_Forms_hpp

// Standard library
#include <map>
#include <set>

// Boost
#include <boost/operators.hpp>

namespace QP {
  struct Variable {
    Variable();

    bool operator < (const Variable& other) const {return id < other.id;}

    private:
      int id;
      static int nextId;
  };

  struct QuadraticForm;

  struct LinearForm :
    boost::addable<LinearForm>,
    boost::subtractable<LinearForm>,
    boost::multipliable<LinearForm, double>,
    boost::dividable<LinearForm, double>
  {
    // @todo Could/should we have a single private ctor taking coefficients and const?
    LinearForm(double);
    LinearForm(const Variable&);

    LinearForm& operator += (const LinearForm&);
    LinearForm& operator -= (const LinearForm&);
    LinearForm& operator *= (double);
    LinearForm& operator /= (double);

    std::set<Variable> getVariables() const;
    double getLinearCoefficient(const Variable&) const;
    double getConstantCoefficient() const;

    private:
      std::map<Variable, double> linearCoefficients;
      double constantCoefficient;

      friend QuadraticForm;
  };

  struct QuadraticForm :
    boost::addable<QuadraticForm>,
    boost::subtractable<QuadraticForm>,
    boost::multipliable<QuadraticForm, double>,
    boost::dividable<QuadraticForm, double>
  {
    // @todo Could/should we have a single private ctor taking coefficients and const?
    QuadraticForm(double);
    QuadraticForm(const Variable&);
    QuadraticForm(const LinearForm&);
    QuadraticForm(const LinearForm& a, const LinearForm& b);

    QuadraticForm& operator +=(const QuadraticForm&);
    QuadraticForm& operator -=(const QuadraticForm&);
    QuadraticForm& operator *=(double);
    QuadraticForm& operator /=(double);

    std::set<Variable> getVariables() const;
    double getQuadraticCoefficient(const Variable&, const Variable&) const;
    double getLinearCoefficient(const Variable&) const;
    double getConstantCoefficient() const;

    private:
      std::map<std::pair<Variable, Variable>, double> quadraticCoefficients;
      LinearForm linearForm;
  };

  LinearForm operator + (const Variable&);
  LinearForm operator + (const LinearForm&);
  QuadraticForm operator + (const QuadraticForm&);

  LinearForm operator - (const Variable&);
  LinearForm operator - (const LinearForm&);
  QuadraticForm operator - (const QuadraticForm&);

  LinearForm operator + (const Variable&, double);
  LinearForm operator + (double, const Variable&);
  LinearForm operator + (const Variable&, const Variable&);

  LinearForm operator - (const Variable&, double);
  LinearForm operator - (double, const Variable&);
  LinearForm operator - (const Variable&, const Variable&);

  LinearForm operator * (const Variable&, double);
  LinearForm operator * (double, const Variable&);

  QuadraticForm operator * (const Variable&, const Variable&);
  QuadraticForm operator * (const LinearForm&, const LinearForm&);
  LinearForm operator / (const Variable&, double);

  struct Constraint {
    const LinearForm& getLinearForm() const;

    enum Type {ZERO, POSITIVE};
    Type getType() const;

    private:
      Constraint(const LinearForm&, Type);
      friend Constraint operator == (const LinearForm&, const LinearForm&);
      friend Constraint operator >= (const LinearForm&, const LinearForm&);
      friend Constraint operator <= (const LinearForm&, const LinearForm&);
      LinearForm linearForm;
      Type type;
  };

  Constraint operator == (const LinearForm&, const LinearForm&);
  Constraint operator >= (const LinearForm&, const LinearForm&);
  Constraint operator <= (const LinearForm&, const LinearForm&);
}

#endif // Include guard
