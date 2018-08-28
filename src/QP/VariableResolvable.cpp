// Header
#include "VariableResolvable.hpp"

namespace QP {

Variable::Resolvable::Resolvable() :
  m_value()
{
}

void Variable::Resolvable::setValue(double value) {
  m_value = value;
}

bool Variable::Resolvable::isResolved() const {
  return (bool)m_value;
}

double Variable::Resolvable::getValue() const {
  return *m_value;
}

} // Namespace
