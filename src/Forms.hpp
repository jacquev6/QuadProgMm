#ifndef QuadProgMm_Forms_hpp
#define QuadProgMm_Forms_hpp

// Standard library
#include <map>
#include <set>

// Boost
#include <boost/operators.hpp>

namespace QuadProgMm {
  struct Variable {
    Variable();

    Variable(const Variable&) = default;
    Variable& operator = (const Variable&) = delete;
    Variable& operator = (const Variable&&) = delete;

    bool operator < (const Variable& other) const {return id < other.id;}

    private:
      int id;
      static int nextId;
  };

  struct LinearForm :
    boost::addable<LinearForm>,
    boost::subtractable<LinearForm>,
    boost::multipliable<LinearForm, double>,
    boost::dividable<LinearForm, double>
  {
    LinearForm(double = 0);
    LinearForm(const Variable&);

    LinearForm& operator += (const LinearForm&);
    LinearForm& operator -= (const LinearForm&);
    LinearForm& operator *= (double);
    LinearForm& operator /= (double);

    std::set<Variable> getVariables() const;
    double getLinearCoefficient(const Variable&) const;
    double getConstantCoefficient() const;

    private:
      LinearForm(const std::map<Variable, double>&, double);
      std::map<Variable, double> linearCoefficients;
      double constantCoefficient;
      friend class QuadraticForm;
  };

  struct QuadraticForm :
    boost::addable<QuadraticForm>,
    boost::subtractable<QuadraticForm>,
    boost::multipliable<QuadraticForm, double>,
    boost::dividable<QuadraticForm, double>
  {
    QuadraticForm(double = 0);
    QuadraticForm(const Variable&);
    QuadraticForm(const LinearForm&);

    QuadraticForm& operator +=(const QuadraticForm&);
    QuadraticForm& operator -=(const QuadraticForm&);
    QuadraticForm& operator *=(double);
    QuadraticForm& operator /=(double);

    std::set<Variable> getVariables() const;
    double getQuadraticCoefficient(const Variable&, const Variable&) const;
    double getLinearCoefficient(const Variable&) const;
    double getConstantCoefficient() const;

    static QuadraticForm multiply(const LinearForm&, const LinearForm&);

    private:
      QuadraticForm(const std::map<std::pair<Variable, Variable>, double>&, const std::map<Variable, double>&, double);
      std::map<std::pair<Variable, Variable>, double> quadraticCoefficients;
      std::map<Variable, double> linearCoefficients;
      double constantCoefficient;
  };

  struct Constraint {
    enum Type {ZERO, POSITIVE};

    Constraint(const LinearForm&, Type);

    const LinearForm& getLinearForm() const;
    Type getType() const;

    private:
      LinearForm linearForm;
      Type type;
  };
}

namespace QuadProgMm {
  LinearForm operator + (const Variable&);
  LinearForm operator + (const LinearForm&);
  QuadraticForm operator + (const QuadraticForm&);

  LinearForm operator - (const Variable&);
  LinearForm operator - (const LinearForm&);
  QuadraticForm operator - (const QuadraticForm&);

  LinearForm operator + (double, const Variable&);
  LinearForm operator + (const Variable&, double);
  LinearForm operator + (const Variable&, const Variable&);

  LinearForm operator - (double, const Variable&);
  LinearForm operator - (const Variable&, double);
  LinearForm operator - (const Variable&, const Variable&);

  LinearForm operator * (double, const Variable&);
  LinearForm operator * (const Variable&, double);

  QuadraticForm operator * (const LinearForm&, const LinearForm&);

  LinearForm operator / (const Variable&, double);

  Constraint operator == (const LinearForm&, const LinearForm&);
  Constraint operator >= (const LinearForm&, const LinearForm&);
  Constraint operator <= (const LinearForm&, const LinearForm&);
}

#endif // Include guard
