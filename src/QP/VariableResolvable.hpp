#ifndef QP_VariableResolvable_hpp
#define QP_VariableResolvable_hpp

// Boost
#include <boost/optional.hpp>

// QP
#include "Variable.hpp"

namespace QP {

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

} // Namespace

#endif // Include guard
