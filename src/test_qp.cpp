// Standard library
#include <iostream>

// QP
#include <QP/QP.hpp>

void compilationChecks() {
    double d = 1.;
    QP::Variable v( "V" );
    QP::LinearForm l = v;
    QP::QuadraticForm q = v;

    // +
    //      d,   v,   l,   q
    /*d*/ d+d; d+v; d+l; d+q;
    /*v*/ v+d; v+v; v+l; v+q;
    /*l*/ l+d; l+v; l+l; l+q;
    /*q*/ q+d; q+v; q+l; q+q;

    // -
    //      d,   v,   l,   q
    /*d*/ d-d; d-v; d-l; d-q;
    /*v*/ v-d; v-v; v-l; v-q;
    /*l*/ l-d; l-v; l-l; l-q;
    /*q*/ q-d; q-v; q-l; q-q;

    // -
    -d; -v; -l; -q;

    // *
    //      d,   v,   l,   q
    /*d*/ d*d; d*v; d*l; d*q;
    /*v*/ v*d; v*v; v*l; //v*q;
    /*l*/ l*d; l*v; l*l; //l*q;
    /*q*/ q*d; //q*v; q*l; q*q;

    // /
    //      d,   v,   l,   q
    /*d*/ d/d; //d/v; d/l; d/q;
    /*v*/ v/d; //v/v; v/l; v/q;
    /*l*/ l/d; //l/v; l/l; l/q;
    /*q*/ q/d; //q/v; q/l; q/q;
}

int main() {
    QP::Variable a( "A" );
    QP::Variable b( "B" );
    QP::Variable c( "C" );

    QP::Objectives objectives;
    objectives.push_back( QP::Objective::Value( a, 6 ) );
    //objectives.push_back( QP::Objective::Value( b, -6 ) );
    objectives.push_back( QP::Objective::Difference( b, a, 10 ) );
    //objectives.push_back( QP::Objective::Minimize( a*a + b*b ) );

    QP::Constraints constraints;
    //constraints.push_back( b - a >= 12 );
    constraints.push_back( QP::Constraint::Difference( b, a, 9 ) );
    //constraints.push_back( QP::Constraint::MaximumDifference( b, a, 9 ) );
    //constraints.push_back( QP::Constraint::MinimumDifference( b, a, 11 ) );
    //constraints.push_back( b + c >= 10 );

    QP::Problem p( objectives, constraints );
    std::cout << "Cost: " << p.solve() << std::endl;

    std::cout << "A=" << a << std::endl;
    std::cout << "B=" << b << std::endl;
    std::cout << "C=" << c << std::endl;
}
