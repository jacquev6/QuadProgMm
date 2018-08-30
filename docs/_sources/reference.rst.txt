=========
Reference
=========

Include file and namespace
==========================

Everything is in the single include file ``QuadProgMm.hpp`` and in the single namespace ``QuadProgMm``.

Main entry points
=================

.. doxygenfunction:: QuadProgMm::minimize

.. doxygenfunction:: QuadProgMm::maximize

.. doxygenstruct:: QuadProgMm::Solution

Domain specific language
========================

Linear and quadratic expressions
--------------------------------

Classes
~~~~~~~

.. doxygenstruct:: QuadProgMm::Variable

.. doxygenstruct:: QuadProgMm::LinearExpression

.. doxygenstruct:: QuadProgMm::QuadraticExpression

Operators
~~~~~~~~~

The `Boost Operators <https://www.boost.org/doc/libs/1_68_0/libs/utility/operators.htm#smpl_oprs>`_
base classes provide homogeneous addition and subtraction operators,
and multiplication and division operators for expressions and doubles.

The operators below, along with the cast constructors in LinearExpression and QuadraticExpression, complete the DSL.
Any "well-formed" polynomial of degree 0, 1 or 2 can be expressed.

.. doxygengroup:: arithmetic_operators
  :content-only:

Constraints
-----------

.. doxygenstruct:: QuadProgMm::Constraint

.. doxygengroup:: comparison_operators
  :content-only:
