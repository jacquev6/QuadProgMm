// Header
#include "Problem.hpp"

// Standard library
//#include <iostream>

// Boost
#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH

// QuadProg++
#include "QuadProg++.hpp"
#undef solve // WTF!!!

// QP
#include "VariableResolvable.hpp"

namespace QP {

Problem::Problem( const Objectives& objectives, const Constraints& constraints ) :
    m_objectives( objectives ),
    m_constraints( constraints ),
    m_variables(),
    m_equalityConstraints( 0 ),
    m_inequalityConstraints( 0 )
{
    int index = 0;
    foreach( Objective o, m_objectives ) {
        const QuadraticForm& q = o.getQuadraticForm();
        foreach( QuadraticForm::Coefficient c, q.getCoefficients() ) {
            if( m_variables.left.find( c.first.first ) == m_variables.left.end() ) {
                m_variables.insert( Variables::value_type( c.first.first, index ) );
                ++index;
            }
            if( m_variables.left.find( c.first.second ) == m_variables.left.end() ) {
                m_variables.insert( Variables::value_type( c.first.second, index ) );
                ++index;
            }
        }
        const LinearForm& l = q.getLinearForm();
        foreach( LinearForm::Coefficient c, l.getCoefficients() ) {
            if( m_variables.left.find( c.first ) == m_variables.left.end() ) {
                m_variables.insert( Variables::value_type( c.first, index ) );
                ++index;
            }
        }
    }
    foreach( Constraint c, m_constraints ) {
        if( c.isEquality() ) {
            ++m_equalityConstraints;
        } else {
            ++m_inequalityConstraints;
        }
        const LinearForm& l = c.getLinearForm();
        foreach( LinearForm::Coefficient c, l.getCoefficients() ) {
            if( m_variables.left.find( c.first ) == m_variables.left.end() ) {
                m_variables.insert( Variables::value_type( c.first, index ) );
                ++index;
            }
        }
    }
}

double Problem::solve() const {
    int n = m_variables.size();
    /*std::cout << n << " variables:" << std::endl;
    for( int i = 0; i < n; ++i ) {
        std::cout << " " << *m_variables.right.find( i )->second << std::endl;
    }*/
    int m = m_equalityConstraints;
    int p = m_inequalityConstraints;

    QuadProgPP::Matrix< double > G, CE, CI;
    QuadProgPP::Vector< double > G0, CE0, CI0, x;
    G.resize( n, n );
    G0.resize( n );
    CE.resize( n, m );
    CE0.resize( m );
    CI.resize( n, p );
    CI0.resize( p );

    for( int i = 0; i < n; ++i ) {
        for( int j = 0; j < n; ++j ) {
            G0[j] = 0;
            G[i][j] = 0;
        }
        for( int j = 0; j < m; ++j ) {
            CE0[j] = 0;
            CE[i][j] = 0;
        }
        for( int j = 0; j < p; ++j ) {
            CI0[j] = 0;
            CI[i][j] = 0;
        }
    }

    double baseCost = 0;

    foreach( Objective o, m_objectives ) {
        const QuadraticForm& q = o.getQuadraticForm();
        //std::cout << q << std::endl;
        foreach( QuadraticForm::Coefficient c, q.getCoefficients() ) {
            int v1 = m_variables.left.find( c.first.first )->second;
            int v2 = m_variables.left.find( c.first.second )->second;
            double coeff = c.second;

            G[ v1 ][ v2 ] += coeff;
            G[ v2 ][ v1 ] += coeff;
        }
        const LinearForm& l = q.getLinearForm();
        foreach( LinearForm::Coefficient c, l.getCoefficients() ) {
            int v = m_variables.left.find( c.first )->second;
            double coeff = c.second;

            G0[ v ] += coeff;
        }
        baseCost += l.getConstant();
    }

    int index = 0;
    foreach( Constraint c, m_constraints ) {
        QuadProgPP::Matrix< double >& C = c.isEquality() ? CE : CI;
        QuadProgPP::Vector< double >& C0 = c.isEquality() ? CE0 : CI0;

        const LinearForm& l = c.getLinearForm();
        foreach( LinearForm::Coefficient c, l.getCoefficients() ) {
            int v = m_variables.left.find( c.first )->second;
            double coeff = c.second;

            C[ v ][ index ] = coeff;
        }
        C0[ index ] = l.getConstant();
        ++index;
    }

    //std::cout << G << std::endl << G0 << std::endl;

    double cost = QuadProgPP::solve_quadprog( G, G0, CE, CE0, CI, CI0, x );

    for( int i = 0; i < n; ++i ) {
        assert( m_variables.right.find( i ) != m_variables.right.end() );
        m_variables.right.find( i )->second->setValue( x[ i ] );
    }

    return cost + baseCost;
}

} // Namespace
