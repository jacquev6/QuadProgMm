==========
User guide
==========

Includes
========

.. BEGIN SECTION user_guide.cpp
..  #include <iostream>

*QuadProgMm* comes with a single include file::

    #include <QuadProgMm.hpp>

..  using namespace QuadProgMm;
..  int main() {

Variables
=========

`Quadratic programming <http://en.wikipedia.org/wiki/Quadratic_programming>`_ (QP) aims at optimizing a quadratic
expression of several variables, subject to linear constraints on these variables.

Let's for example solve this very simple quadratic programming problem: find values of :math:`x_1` and :math:`x_2`
that minimize

.. math::

   {1 \over 2} \cdot x_1^2 + x_2^2 - x_1 \cdot x_2 - 2 \cdot x_1 - 6 \cdot x_2

subject to

.. math::

   x_1 + x_2 \le 2 \\
   2 \cdot x_2 \le x_1 + 2 \\
   2 \cdot x_1 + x_2 \le 3 \\
   x_1 \ge 0 \\
   x_2 \ge 0

So, let's start with variables::

    Variable x1, x2;

This defines two instances of the :class:`Variable <QuadProgMm::Variable>` class, representing the variables of your QP problem.
Solving your quadratic problem means finding values for these variables.

The :class:`Variable <QuadProgMm::Variable>` class has some sort of "pointer" semantics:
an instance *represents* a variable of your QP problem, and if you copy this instance,
the copy represents the *same* variable. The original and the copy can be used interchangeably anywhere.

They are also immutable: :class:`Variable <QuadProgMm::Variable>` instances represent
the same variable from their construction to their destruction.

Linear and quadratic expressions
================================

:class:`Variables <QuadProgMm::Variable>` can then be combined into
:class:`LinearExpressions <QuadProgMm::LinearExpression>` and :class:`QuadraticExpressions <QuadProgMm::QuadraticExpression>`
using conventional operators and floating point numbers.
All operators producing degree zero, one, or two polynomials are available. Here are a few examples::

    Variable a, b, c;
    LinearExpression l1 = a + 2 * b - c / 3 + 4;
    LinearExpression l2 = l1 + 2 * a;
    QuadraticExpression q1 = a * a + 2 * b + 1;
    QuadraticExpression q2 = a * a - (l1 - b) * l2;

Linear and quadratic expressions have value semantics.
Copying them creates an independent expression that can be mutated independently::

    LinearExpression l3 = l2;
    l3 *= 4;
    QuadraticExpression q3 = q2;
    q3 += l3;

Constraints
===========

:class:`Constraints <QuadProgMm::Constraint>` can be constructed using (in)equality operators between
:class:`LinearExpressions <QuadProgMm::LinearExpression>`::

    Constraint c1 = a <= b + 2;
    Constraint c2 = c - 2 * a == b;

Solving
=======

The :func:`minimize <QuadProgMm::minimize>` and :func:`maximize <QuadProgMm::maximize>` functions solve QP problems.
For our original example, we just have to translate the mathematical expressions to their close equivalent in C++::

    Solution s = minimize(
      0.5 * x1 * x1 + x2 * x2 - x1 * x2 - 2 * x1 - 6 * x2,
      {
        x1 + x2 <= 2,
        2 * x2 <= x1 + 2,
        2 * x1 + x2 <= 3,
        x1 >= 0,
        x2 >= 0,
      }
    );

The optimal (here, minimal) value is available through :func:`Solution::getCost <QuadProgMm::Solution::getCost>`::

    std::cout << "Optimal value: " << s.getCost() << std::endl;

.. include:: user_guide.out
   :literal:
   :end-line: 1

The values that variables must take to reach this optimum are available through :func:`Solution::get <QuadProgMm::Solution::get>`::

    std::cout << "x1: " << s.get(x1) << std::endl;
    std::cout << "x2: " << s.get(x2) << std::endl;

.. include:: user_guide.out
   :literal:
   :start-line: 1

..  }

.. END SECTION user_guide.cpp

