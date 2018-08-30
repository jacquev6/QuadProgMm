#ifndef QuadProgMm_hpp
#define QuadProgMm_hpp

// Standard library
#include <vector>
#include <map>
#include <set>

// Boost
#include <boost/operators.hpp>

namespace QuadProgMm {
  /*!
    The representation of a variable usable in QP problems.

    It is immutable: a Variable instance represents the same variable from its construction to its destruction.
  */
  struct Variable {
    /*!
      Construct a new variable.

      It represents a different variable from all \ref Variable "Variables" created before.
    */
    Variable();

    /*!
      Construct a copy of an existing Variable.

      It represents the same variable as the original and is usable interchangably with the original everywhere.
    */
    Variable(const Variable&) = default;
    Variable& operator = (const Variable&) = delete;
    Variable& operator = (const Variable&&) = delete;

    /*!
      Strict comparison operator, to be able to use \ref Variable "Variables" in std::maps for example.
    */
    bool operator < (const Variable& other) const {return id < other.id;}

    private:
      int id;
      static int nextId;
  };

  /*!
    A polynomial of \ref Variable "Variables" of degree 0 or 1.
  */
  struct LinearExpression :
    boost::addable<LinearExpression>,
    boost::subtractable<LinearExpression>,
    boost::multipliable<LinearExpression, double>,
    boost::dividable<LinearExpression, double>
  {
    /*!
      Construct a polynomial of degree 0, equal to the parameter.
    */
    LinearExpression(double = 0);

    /*!
      Construct a single-variable, unitary, polynomial of degree 1.
    */
    LinearExpression(const Variable&);

    /*! 
      Add another LinearExpression to the current one.
    */
    LinearExpression& operator += (const LinearExpression&);

    /*! 
      Substract another LinearExpression to the current one.
    */
    LinearExpression& operator -= (const LinearExpression&);

    /*! 
      Multiply the current LinearExpression by a constant.
    */
    LinearExpression& operator *= (double);

    /*! 
      Divide the current LinearExpression by a constant.
    */
    LinearExpression& operator /= (double);

    // Not part of the public interface of the library
    std::set<Variable> getVariables() const;
    double getLinearCoefficient(const Variable&) const;
    double getConstantCoefficient() const;

    private:
      LinearExpression(const std::map<Variable, double>&, double);
      std::map<Variable, double> linearCoefficients;
      double constantCoefficient;
      friend class QuadraticExpression;
  };

  /*!
    A polynomial of \ref Variable "Variables" of degree 0, 1, or 2.
  */
  struct QuadraticExpression :
    boost::addable<QuadraticExpression>,
    boost::subtractable<QuadraticExpression>,
    boost::multipliable<QuadraticExpression, double>,
    boost::dividable<QuadraticExpression, double>
  {
    /*!
      Construct a polynomial of degree 0, equal to the parameter.
    */
    QuadraticExpression(double = 0);

    /*!
      Construct a single-variable, unitary, polynomial of degree 1.
    */
    QuadraticExpression(const Variable&);

    /*!
      Construct a polynomial of degree 0 or 1, equal to the parameter.
    */
    QuadraticExpression(const LinearExpression&);

    /*! 
      Add another QuadraticExpression to the current one.
    */
    QuadraticExpression& operator +=(const QuadraticExpression&);

    /*! 
      Substract another QuadraticExpression to the current one.
    */
    QuadraticExpression& operator -=(const QuadraticExpression&);

    /*! 
      Multiply the current QuadraticExpression by a constant.
    */
    QuadraticExpression& operator *=(double);

    /*! 
      Divide the current QuadraticExpression by a constant.
    */
    QuadraticExpression& operator /=(double);

    // Not part of the public interface of the library
    std::set<Variable> getVariables() const;
    double getQuadraticCoefficient(const Variable&, const Variable&) const;
    double getLinearCoefficient(const Variable&) const;
    double getConstantCoefficient() const;

    static QuadraticExpression multiply(const LinearExpression&, const LinearExpression&);

    private:
      QuadraticExpression(const std::map<std::pair<Variable, Variable>, double>&, const std::map<Variable, double>&, double);
      std::map<std::pair<Variable, Variable>, double> quadraticCoefficients;
      std::map<Variable, double> linearCoefficients;
      double constantCoefficient;
  };

  /*!
    A constraint.
  */
  // Contents not part of the public interface of the library
  struct Constraint {
    enum Type {ZERO, POSITIVE};

    Constraint(const LinearExpression&, Type);

    const LinearExpression& getLinearExpression() const;
    Type getType() const;

    private:
      LinearExpression linearExpression;
      Type type;
  };
}

namespace QuadProgMm {
  /**
   * \defgroup arithmetic_operators Arithmetic operators of the DLS
   * @{
   */
  LinearExpression operator + (const Variable&);
  LinearExpression operator + (const LinearExpression&);
  QuadraticExpression operator + (const QuadraticExpression&);

  LinearExpression operator - (const Variable&);
  LinearExpression operator - (const LinearExpression&);
  QuadraticExpression operator - (const QuadraticExpression&);

  LinearExpression operator + (double, const Variable&);
  LinearExpression operator + (const Variable&, double);
  LinearExpression operator + (const Variable&, const Variable&);

  LinearExpression operator - (double, const Variable&);
  LinearExpression operator - (const Variable&, double);
  LinearExpression operator - (const Variable&, const Variable&);

  LinearExpression operator * (double, const Variable&);
  LinearExpression operator * (const Variable&, double);

  QuadraticExpression operator * (const LinearExpression&, const LinearExpression&);

  LinearExpression operator / (const Variable&, double);
  /**@}*/

  /**
   * \defgroup comparison_operators Comparison operators of the DLS
   * @{
   */
  Constraint operator == (const LinearExpression&, const LinearExpression&);
  Constraint operator >= (const LinearExpression&, const LinearExpression&);
  Constraint operator <= (const LinearExpression&, const LinearExpression&);
  /**@}*/
}

namespace QuadProgMm {
  /*!
    The result of an optimization.

    Returned by \ref minimize and \ref maximize.
  */
  struct Solution {
    /*!
      Get the optimal value of the QuadraticExpression.

      This is the minimal value of the QuadraticExpression passed to \ref minimize and its maximum value when using \ref maximize.
    */
    double getCost() const {return cost;}

    /*!
      Get the value a variable must take to reach the optimum.

      Behavior is undefined when called with a variable that was not part of the QuadraticExpression optimized.
    */
    double get(const Variable& v) const {return values.find(v)->second;}

    public:
      // Not part of the public interface of the library
      Solution(double cost_, const std::map<Variable, double>& values_) : cost(cost_), values(values_) {}

    private:
      const double cost;
      std::map<Variable, double> values;
  };

  //! Minimize a QuadraticExpression subject to \ref Constraint "Constraints"
  Solution minimize(const QuadraticExpression&, const std::vector<Constraint>& = std::vector<Constraint>());

  //! Maximize a QuadraticExpression subject to \ref Constraint "Constraints"
  Solution maximize(const QuadraticExpression&, const std::vector<Constraint>& = std::vector<Constraint>());
}

#endif // Include guard
