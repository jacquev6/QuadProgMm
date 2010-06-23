#ifndef QP_VariableResolvable_hpp
#define QP_VariableResolvable_hpp

// Standard library
#include <ostream>
#include <string>

// Boost
#include <boost/optional.hpp>

// QP
#include "Variable.hpp"

namespace QP {

class Variable::Resolvable {
public:
    Resolvable( const std::string& name );
    Resolvable( const Resolvable& );

public:
    void setValue( double value );
    bool isResolved() const;
    double getValue() const;

public:
    friend std::ostream& operator << ( std::ostream&, const Variable::Resolvable& );

private:
    boost::optional< double > m_value;
    std::string m_name;
};

} // Namespace

#endif // Include guard
