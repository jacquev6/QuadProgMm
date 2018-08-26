Introduction
============

[Quadratic programming](http://en.wikipedia.org/wiki/Quadratic_programming) problems are generaly specified using matrices and vectors,
because this is the form used by resolution algorithms.
This is true for example for [QuadProg++](https://github.com/liuq/QuadProgpp), which I use as algorithmic fundation of this library.

What I propose here is to specify your QP with C++ expressions of C++ variables, and let my library do what is needed to invoke QuadProg++'s algorithms.

A full example
==============

Let's say that your want to compute the positions of a chain of springs.

<img src="example_spring_chain.svg"/>

First, let's define a struct to represent a spring. It's a simple aggregate with the spring's strength, and its unconstrained length:

    struct Spring {
        Spring( float strength_, float length_ ) :
            strength( strength_ ),
            length( length_ )
        {
        }

        float strength;
        float length;
    };

Then, a class for the chain. It has two boundaries, `left` and `right`,
a vector of `Spring`s, and a vector of `QP::Variable` for the positions to be calculated.

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

`createPositions` is really needed because `QP::Variable` doesn't have a value semantic but a pointer semantic:
when a `QP::Variable` is copied, the copy refers to the same resolvable entity as the original.
That's why we can't use the constructors of vector taking the number of elements as an argument.

Then, the most important part is the definition of `SpringChain::resolve`:

    void SpringChain::resolve() {

First, we define the objectives: each spring has a potential energy of \\(1/2 \\cdot k \\cdot (l - l_0)\\),
and the balance position is achieved when the energy is minimal. Our objective is to minimize the sum of all those energies.
So, we fill a `QP::Objectives` (a vector) with one objective per spring:

        QP::Objectives objectives;
        for( size_t i = 0; i != springs.size(); ++i ) {
            QP::LinearForm l_minus_l0 =
                position[ i + 1 ]
                - position[ i ]
                - springs[ i ].length;

            objectives.push_back( QP::Objective::Minimize(
                0.5 * springs[ i ].strength * l_minus_l0 * l_minus_l0
            ) );
        }

Then, we add our constraints on boundaries:

        QP::Constraints constraints;
        constraints.push_back( position.front() == left );
        constraints.push_back( position.back() == right );

And we solve the QP problem:

        QP::Problem p( objectives, constraints );
        p.solve();

And we end `SpringChain::resolve`:

    }

Finally, we use our `SpringChain` class:

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

And here is the result:

    7.60026e-10
    1.35484
    3.70968
    8.64516
    10

In this example, you have seen the basics of how we can express objectives and constraints as arithmetic expressions of C++ variables.
Dealing with vectors and matrices is the responibility of the library.

<script type="text/javascript" src="http://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-AMS-MML_HTMLorMML"></script>

