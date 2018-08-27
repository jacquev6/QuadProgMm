==========
User guide
==========

.. @todo Add links to the reference for all ``QP::thing``

Variables
=========

.. BEGIN SECTION user_guide.cpp
..  #include <QP/QP.hpp>

..  int main() {

Quadratic problems aim at optimizing a quadratic form of several variables,
subject to linear constraints on these variables.

So, let's start with variables::

    QP::Variable a, b, c, d;

This defines four instances representing the variables of your QP problem.
Solving your quadratic problem means finding values for these variables.
Once the problem is solved, these values can be retrieved using ``QP::Variable::getValue`` (see below).

It is your responsibility to ensure that variables are utlimately only used by a single call to ``QP::solve``.
@todo Maybe change the interface so that this is ensured by the library.

@todo Variables have pointer semantics

Linear and quadratic forms
==========================

Variables can then be combined into linear forms using conventional operators and floating points::

    QP::LinearForm l = 3.5 * a + b - c / 9.8 + 12;

All operators producing degree zero or one polynoms are available::

    std::vector<QP::LinearForm> all_linear_forms {
      1, a, l,
      +a, +l, -a, -l,
      a + 1, 1 + a, a + b, a + l, l + a, l + l,
      a - 1, 1 - a, a - b, a - l, l - a, l - l,
      2 * a, a * 2, 2 * l, l * 2,
      a / 2, l / 2,
    };

Variables and linear forms can be combined into quadratic forms as well::

    QP::QuadraticForm q = 2.3 * l * l + a * b + c - d * l + 4;

    std::vector<QP::QuadraticForm> all_quadratic_forms {
      1, a, l, q,
      a * a, a * b,
      +q, -q,
      l * l,
    };

Oh, and linear and quadratic forms are actually mutable::

    l += 1;
    q += l;
    q *= 2;
    // etc.

@todo Linear and quadratic forms have value semantics

Objectives and constraints
==========================

Solving
=======

Solve, cost

Degenerate problems
===================

definite positive, or else...

..  }

.. END SECTION user_guide.cpp
