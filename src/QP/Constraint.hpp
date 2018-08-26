#ifndef QP_Constraint_hpp
#define QP_Constraint_hpp

// Standard library
#include <vector>

// QP
#include "LinearForm.hpp"

namespace QP {

class Constraint {
public:
  static Constraint Value(const LinearForm&, double value);
  static Constraint MinimumValue(const LinearForm&, double value);
  static Constraint MaximumValue(const LinearForm&, double value);
  static Constraint Difference(const LinearForm&, const LinearForm&, double value);
  static Constraint MinimumDifference(const LinearForm&, const LinearForm&, double value);
  static Constraint MaximumDifference(const LinearForm&, const LinearForm&, double value);

private:
  friend Constraint operator == (const LinearForm&, const LinearForm&);
  friend Constraint operator >= (const LinearForm&, const LinearForm&);
  friend Constraint operator <= (const LinearForm&, const LinearForm&);
  Constraint(const LinearForm&, bool isEquality);

public:
  const LinearForm& getLinearForm() const;
  bool isEquality() const;

private:
  LinearForm m_linearForm;
  bool m_isEquality;
};

Constraint operator == (const LinearForm&, const LinearForm&);
Constraint operator >= (const LinearForm&, const LinearForm&);
Constraint operator <= (const LinearForm&, const LinearForm&);

typedef std::vector<Constraint> Constraints;

} // Namespace

#endif // Include guard
