// Header
#include "VariableResolvable.hpp"

namespace QP {

Variable::Resolvable::Resolvable( const std::string& name ) :
    m_value(),
    m_name( name )
{
}

void Variable::Resolvable::setValue( double value ) {
    m_value = value;
}

bool Variable::Resolvable::isResolved() const {
    return m_value;
}

double Variable::Resolvable::getValue() const {
    return *m_value;
}

std::ostream& operator << ( std::ostream& s, const Variable::Resolvable& v ) {
    s << v.m_name;
    return s;
}

} // Namespace
