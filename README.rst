Introduction
============

*QuadProgMm* is a C++ library providing a domain specific language to express
`Quadratic programming <http://en.wikipedia.org/wiki/Quadratic_programming>`_ problems as literal C++ equations on named variables.

These problems are generally specified using matrices and vectors, because it is the generic mathematical form used by resolution algorithms.
This is sometime tedius, and *QuadProgMm* tries to fix that.

Note that *QuadProgMm* does *not* implement the arguably complex linear algebra algorithms for solving these problems
and delegates all that hard work to `QuadProg++ <https://github.com/liuq/QuadProgpp>`_, by Luca Di Gaspero.

It's licensed under the `MIT license <http://choosealicense.com/licenses/mit/>`__.
Its `documentation <http://jacquev6.github.io/QuadProgMm>`__
and its `source code <https://github.com/jacquev6/QuadProgMm>`__ are on GitHub.

Questions? Remarks? Bugs? Want to contribute? `Open an issue <https://github.com/jacquev6/QuadProgMm/issues>`__!

Quick start
===========

Clone and build::

    $ git clone https://github.com/jacquev6/QuadProgMm.git
    $ cd QuadProgMm
    $ make lib

.. BEGIN SECTION quick_start.cpp

Start with Includes::

    #include <iostream>
    #include <QuadProgMm.hpp>

    using namespace QuadProgMm;

    int main() {

Create variables::

      Variable a, b, c;

And a quadratic expression to optimize::

      QuadraticExpression q = a + b + (a - b) * (a - b) + c + (b - c) * (b - c);

And constraints::

      std::vector<Constraint> constraints {
        a <= 1,
        c >= 4,
        a - 2 * b <= 12,
      };

Finally, solve::

      Solution solution = minimize(q, constraints);

      std::cout << "a: " << solution.get(a) << std::endl;
      std::cout << "b: " << solution.get(b) << std::endl;
      std::cout << "c: " << solution.get(c) << std::endl;
      std::cout << "Cost: " << solution.getCost() << std::endl;
    }

.. END SECTION quick_start.cpp

Build and run::

    $ g++ -Ipath/to/QuadProgMm/src quick_start.cpp path/to/QuadProgMm/build/libquadprogmm.a path/to/QuadProgMm/QuadProgpp/src/libquadprog.a -o quick_start
    $ ./quick_start

Result::

    a: 1
    b: 2
    c: 4
    Cost: 16
