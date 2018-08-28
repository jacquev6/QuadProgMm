#ifndef QP_Constraint_hpp
#define QP_Constraint_hpp

// Standard library
#include <vector>

// QP
#include "Forms.hpp"

namespace QP {

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
