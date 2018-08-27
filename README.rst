Introduction
============

`Quadratic programming <http://en.wikipedia.org/wiki/Quadratic_programming>`_ problems are generally specified
using matrices and vectors, because it is the generic mathematical form used by resolution algorithms.
*QuadProgMm* is a C++ library providing a domain specific language to specify them as literal C++ equations on named variables.

Note that *QuadProgMm* does *not* implement the arguably complex algorithms for solving these problems
and delegates all the hard work to `QuadProg++ <https://github.com/liuq/QuadProgpp>`_, by Luca Di Gaspero.

It's licensed under the `MIT license <http://choosealicense.com/licenses/mit/>`__.
Its `documentation <http://jacquev6.github.io/QuadProgMm>`__
and its `source code <https://github.com/jacquev6/QuadProgMm>`__ are on GitHub.

Questions? Remarks? Bugs? Want to contribute? `Open an issue <https://github.com/jacquev6/QuadProgMm/issues>`__!

Quick start
===========

Build:

(@todo Add a real build system and document it here)

.. BEGIN SECTION quick_start.cpp

Start with Includes::

    #include <iostream>
    #include <QP/QP.hpp>

    int main() {

Create variables::

      QP::Variable a, b, c;

State objectives::

      std::vector<QP::Objective> objectives;
      objectives.push_back(QP::Objective::Minimize(a + b + (a - b) * (a - b)));
      objectives.push_back(QP::Objective::Minimize(c + (b - c) * (b - c)));

And constraints::

      std::vector<QP::Constraint> constraints {
        a <= 1,
        c >= 4,
        a - 2 * b <= 12,
      };

Finally, solve::

      QP::solve(objectives, constraints);

      std::cout << "a: " << a.getValue() << std::endl;
      std::cout << "b: " << b.getValue() << std::endl;
      std::cout << "c: " << c.getValue() << std::endl;
    }

.. END SECTION quick_start.cpp

Result::

    a: 1
    b: 2
    c: 4
