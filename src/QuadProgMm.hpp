#ifndef QuadProgMm_hpp
#define QuadProgMm_hpp

// Standard library
#include <vector>
#include <map>
#include <set>

// Boost
#include <boost/operators.hpp>

//! The namespace
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
  struct LinearExpression :
    boost::addable<LinearExpression>,
    boost::subtractable<LinearExpression>,
    boost::multipliable<LinearExpression, double>,
    boost::dividable<LinearExpression, double>
  {
    /*! Yup */
    LinearExpression(double = 0);
    /*! Yup */
    LinearExpression(const Variable&);

    /*! Yup */
    LinearExpression& operator += (const LinearExpression&);
    /*! Yup */
    LinearExpression& operator -= (const LinearExpression&);
    /*! Yup */
    LinearExpression& operator *= (double);
    /*! Yup */
    LinearExpression& operator /= (double);

    /*! Yup */
    std::set<Variable> getVariables() const;
    /*! Yup */
    double getLinearCoefficient(const Variable&) const;
    /*! Yup */
    double getConstantCoefficient() const;

    private:
      LinearExpression(const std::map<Variable, double>&, double);
      std::map<Variable, double> linearCoefficients;
      double constantCoefficient;
      friend class QuadraticExpression;
  };

  //! A degre-2 expression
  struct QuadraticExpression :
    boost::addable<QuadraticExpression>,
    boost::subtractable<QuadraticExpression>,
    boost::multipliable<QuadraticExpression, double>,
    boost::dividable<QuadraticExpression, double>
  {
    /*! Yup */
    QuadraticExpression(double = 0);
    /*! Yup */
    QuadraticExpression(const Variable&);
    /*! Yup */
    QuadraticExpression(const LinearExpression&);

    /*! Yup */
    QuadraticExpression& operator +=(const QuadraticExpression&);
    /*! Yup */
    QuadraticExpression& operator -=(const QuadraticExpression&);
    /*! Yup */
    QuadraticExpression& operator *=(double);
    /*! Yup */
    QuadraticExpression& operator /=(double);

    /*! Yup */
    std::set<Variable> getVariables() const;
    /*! Yup */
    double getQuadraticCoefficient(const Variable&, const Variable&) const;
    /*! Yup */
    double getLinearCoefficient(const Variable&) const;
    /*! Yup */
    double getConstantCoefficient() const;

    static QuadraticExpression multiply(const LinearExpression&, const LinearExpression&);

    private:
      QuadraticExpression(const std::map<std::pair<Variable, Variable>, double>&, const std::map<Variable, double>&, double);
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

    Constraint(const LinearExpression&, Type);

    /*! Yup */
    const LinearExpression& getLinearExpression() const;
    /*! Yup */
    Type getType() const;

    private:
      LinearExpression linearExpression;
      Type type;
  };
}

namespace QuadProgMm {
  /*! Yup */
  LinearExpression operator + (const Variable&);
  /*! Yup */
  LinearExpression operator + (const LinearExpression&);
  /*! Yup */
  QuadraticExpression operator + (const QuadraticExpression&);

  /*! Yup */
  LinearExpression operator - (const Variable&);
  /*! Yup */
  LinearExpression operator - (const LinearExpression&);
  /*! Yup */
  QuadraticExpression operator - (const QuadraticExpression&);

  /*! Yup */
  LinearExpression operator + (double, const Variable&);
  /*! Yup */
  LinearExpression operator + (const Variable&, double);
  /*! Yup */
  LinearExpression operator + (const Variable&, const Variable&);

  /*! Yup */
  LinearExpression operator - (double, const Variable&);
  /*! Yup */
  LinearExpression operator - (const Variable&, double);
  /*! Yup */
  LinearExpression operator - (const Variable&, const Variable&);

  /*! Yup */
  LinearExpression operator * (double, const Variable&);
  /*! Yup */
  LinearExpression operator * (const Variable&, double);

  /*! Yup */
  QuadraticExpression operator * (const LinearExpression&, const LinearExpression&);

  /*! Yup */
  LinearExpression operator / (const Variable&, double);

  /*! Yup */
  Constraint operator == (const LinearExpression&, const LinearExpression&);
  /*! Yup */
  Constraint operator >= (const LinearExpression&, const LinearExpression&);
  /*! Yup */
  Constraint operator <= (const LinearExpression&, const LinearExpression&);
}

namespace QuadProgMm {
  //! The result of an optimization
  /*! Returned by minimize and maximize */
  struct Solution {
    //! Get the optimal value of the QuadraticExpression
    double getCost() const {return cost;}

    //! Get the value a variable must take to reach the optimum
    // Behavior is undefined when calling with a variable that was not part of the problem
    double get(const Variable& v) const {return values.find(v)->second;}

    public:
      Solution(double cost_, const std::map<Variable, double>& values_) : cost(cost_), values(values_) {}

    private:
      const double cost;
      std::map<Variable, double> values;
  };

  //! Minimize a QuadraticExpression subject to Constraints
  Solution minimize(const QuadraticExpression&, const std::vector<Constraint>& = std::vector<Constraint>());

  //! Maximize a QuadraticExpression subject to Constraints
  Solution maximize(const QuadraticExpression&, const std::vector<Constraint>& = std::vector<Constraint>());
}

#endif // Include guard
