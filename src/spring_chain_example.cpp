// Standard library
#include <iostream>
#include <vector>

// QP
#include <QP/QP.hpp>

struct Spring {
    Spring( float strength_, float length_ ) : strength( strength_ ), length( length_ ) {}
    float strength;
    float length;
};

class SpringChain {
public:
    SpringChain( float left_, float right_, const std::vector< Spring > springs_ ) :
        left( left_ ),
        right( right_ ),
        springs( springs_ ),
        position()
    {
        createPositions();
        resolve();
    }
    float get( int i ) const { return position[ i ].getValue(); }

private:
    void createPositions() {
        position.reserve( springs.size() + 1 );
        for( size_t i = 0; i != springs.size() + 1; ++i ) {
            position.push_back( QP::Variable() );
        }
    }

    void resolve();

private:
    float left, right;
    std::vector< Spring > springs;
    std::vector< QP::Variable > position;
};

void SpringChain::resolve() {
    QP::Objectives objectives;
    for( size_t i = 0; i != springs.size(); ++i ) {
        QP::LinearForm l_minus_l0 = position[ i + 1 ] - position[ i ] - springs[ i ].length;
        objectives.push_back( QP::Objective::Minimize(
            0.5 * springs[ i ].strength * l_minus_l0 * l_minus_l0
        ) );
    }
    
    QP::Constraints constraints;
    constraints.push_back( position.front() == left );
    constraints.push_back( position.back() == right );

    QP::Problem p( objectives, constraints );
    p.solve();
}

int main() {
    std::vector< Spring > springs;
    springs.push_back( Spring( 1, 2 ) );
    springs.push_back( Spring( 1, 3 ) );
    springs.push_back( Spring( 10, 5 ) );
    springs.push_back( Spring( 1, 2 ) );

    SpringChain chain( 0, 10, springs );
    for( size_t i = 0; i != springs.size() + 1; ++i ) {
        std::cout << chain.get( i ) << std::endl;
    }
}
