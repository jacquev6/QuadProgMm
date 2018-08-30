#ifndef QuadProgMm_Forms_hpp
#define QuadProgMm_Forms_hpp

// @todo Fix naming
// Rename files to DSL.*
// Rename LinearForm to Degree1Polynom?
// Rename QuadraticForm to Degree2Polynom?

// Standard library
#include <map>
#include <set>

// Boost
#include <boost/operators.hpp>

namespace QuadProgMm {
  /*! A variable usable in QP problems

  @todoc Pointer-like semantics
  */
  struct Variable {
    //! Default constructor
    Variable();

    Variable(const Variable&) = default;
    Variable& operator = (const Variable&) = delete;
    Variable& operator = (const Variable&&) = delete;

    //! Strict comparison operator, to be able to use Variables in std::maps
    bool operator < (const Variable& other) const {return id < other.id;}

    private:
      int id;
      static int nextId;
  };

  //! A degre-1 expression
  struct LinearForm :
    boost::addable<LinearForm>,
    boost::subtractable<LinearForm>,
    boost::multipliable<LinearForm, double>,
    boost::dividable<LinearForm, double>
  {
    /*! Yup */
    LinearForm(double = 0);
    /*! Yup */
    LinearForm(const Variable&);

    /*! Yup */
    LinearForm& operator += (const LinearForm&);
    /*! Yup */
    LinearForm& operator -= (const LinearForm&);
    /*! Yup */
    LinearForm& operator *= (double);
    /*! Yup */
    LinearForm& operator /= (double);

    /*! Yup */
    std::set<Variable> getVariables() const;
    /*! Yup */
    double getLinearCoefficient(const Variable&) const;
    /*! Yup */
    double getConstantCoefficient() const;

    private:
      LinearForm(const std::map<Variable, double>&, double);
      std::map<Variable, double> linearCoefficients;
      double constantCoefficient;
      friend class QuadraticForm;
  };

  //! A degre-2 expression
  struct QuadraticForm :
    boost::addable<QuadraticForm>,
    boost::subtractable<QuadraticForm>,
    boost::multipliable<QuadraticForm, double>,
    boost::dividable<QuadraticForm, double>
  {
    /*! Yup */
    QuadraticForm(double = 0);
    /*! Yup */
    QuadraticForm(const Variable&);
    /*! Yup */
    QuadraticForm(const LinearForm&);

    /*! Yup */
    QuadraticForm& operator +=(const QuadraticForm&);
    /*! Yup */
    QuadraticForm& operator -=(const QuadraticForm&);
    /*! Yup */
    QuadraticForm& operator *=(double);
    /*! Yup */
    QuadraticForm& operator /=(double);

    /*! Yup */
    std::set<Variable> getVariables() const;
    /*! Yup */
    double getQuadraticCoefficient(const Variable&, const Variable&) const;
    /*! Yup */
    double getLinearCoefficient(const Variable&) const;
    /*! Yup */
    double getConstantCoefficient() const;

    static QuadraticForm multiply(const LinearForm&, const LinearForm&);

    private:
      QuadraticForm(const std::map<std::pair<Variable, Variable>, double>&, const std::map<Variable, double>&, double);
      std::map<std::pair<Variable, Variable>, double> quadraticCoefficients;
      std::map<Variable, double> linearCoefficients;
      double constantCoefficient;
  };

  /*! Yup */
  struct Constraint {
    /*! Yup */
    enum Type {
      /*! Yup */
      ZERO,
      /*! Yup */
      POSITIVE
    };

    Constraint(const LinearForm&, Type);

    /*! Yup */
    const LinearForm& getLinearForm() const;
    /*! Yup */
    Type getType() const;

    private:
      LinearForm linearForm;
      Type type;
  };
}

namespace QuadProgMm {
  /*! Yup */
  LinearForm operator + (const Variable&);
  /*! Yup */
  LinearForm operator + (const LinearForm&);
  /*! Yup */
  QuadraticForm operator + (const QuadraticForm&);

  /*! Yup */
  LinearForm operator - (const Variable&);
  /*! Yup */
  LinearForm operator - (const LinearForm&);
  /*! Yup */
  QuadraticForm operator - (const QuadraticForm&);

  /*! Yup */
  LinearForm operator + (double, const Variable&);
  /*! Yup */
  LinearForm operator + (const Variable&, double);
  /*! Yup */
  LinearForm operator + (const Variable&, const Variable&);

  /*! Yup */
  LinearForm operator - (double, const Variable&);
  /*! Yup */
  LinearForm operator - (const Variable&, double);
  /*! Yup */
  LinearForm operator - (const Variable&, const Variable&);

  /*! Yup */
  LinearForm operator * (double, const Variable&);
  /*! Yup */
  LinearForm operator * (const Variable&, double);

  /*! Yup */
  QuadraticForm operator * (const LinearForm&, const LinearForm&);

  /*! Yup */
  LinearForm operator / (const Variable&, double);

  /*! Yup */
  Constraint operator == (const LinearForm&, const LinearForm&);
  /*! Yup */
  Constraint operator >= (const LinearForm&, const LinearForm&);
  /*! Yup */
  Constraint operator <= (const LinearForm&, const LinearForm&);
}

#endif // Include guard
