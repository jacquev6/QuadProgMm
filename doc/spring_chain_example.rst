Spring chain example
====================

.. BEGIN SECTION spring_chain_example.cpp

Let's say that you want to compute the balance positions of a chain of springs.

.. @todo Add image

Start with includes::

    #include <iostream>
    #include <QuadProgMm.hpp>

    using namespace QuadProgMm;

Define a class representing a spring.
It's a simple aggregate with the spring's strength, and unconstrained length::

    struct Spring {
      Spring(float strength_, float length_) :
        strength(strength_), length(length_)
      {}

      float strength, length;
    };

Then, a class for the chain.
It has two boundaries (``left`` and ``right``),
a vector of ``Spring``, and a vector of :class:`Variable <QuadProgMm::Variable>` for the positions to be computed::

    struct SpringChain {
      SpringChain(
        float left_, float right_,
        const std::vector<Spring>& springs_
      ) :
        left(left_), right(right_),
        springs(springs_),
        positions(makePositions(springs_)),
        solution(resolve())
      {}

      double get(size_t i) const {
        return solution.get(positions[i]);
      }

      private:
        static std::vector<Variable> makePositions(
          const std::vector<Spring>&
        );
        Solution resolve();

        float left, right;
        std::vector<Spring> springs;
        std::vector<Variable> positions;
        Solution solution;
    };

Then, the definition of ``makePositions`` that constructs an independent :class:`Variable <QuadProgMm::Variable>`
for each position to solve::

    std::vector<Variable> SpringChain::makePositions(
      const std::vector<Spring>& springs
    ) {
      std::vector<Variable> positions;
      for(size_t i = 0; i != springs.size() + 1; ++i) {
        positions.push_back(Variable());
      }
      return positions;
    }

Then, the most important part: the definition of ``SpringChain::resolve``::

    Solution SpringChain::resolve() {

First, define the objective: each spring has a `potential energy <https://en.wikipedia.org/wiki/Elastic_energy>`_ of :math:`1/2 \cdot k \cdot (l - l_0) ^ 2`,
where :math:`k` is its strength, :math:`l_0` is its unconstrained length, and :math:`l` is its current length.
The balance position is reached when the total energy is minimal: the objective is to minimize the sum of all those energies.
So, build the total energy as a quadratic expression of the variables::

      QuadraticExpression energy = 0;
      for(size_t i = 0; i != springs.size(); ++i) {
        LinearExpression l_minus_l0 =
          positions[i + 1] - positions[i]
          - springs[i].length;

        energy += 0.5 * springs[i].strength * l_minus_l0 * l_minus_l0;
      }

Add the constraints on boundaries::

      std::vector<Constraint> boundaries {
        positions.front() == left,
        positions.back() == right,
      };

And solve the QP problem::

      return minimize(energy, boundaries);
    }

Finally, use the ``SpringChain`` class::

    int main() {
      std::vector<Spring> springs {
        Spring(1, 2),
        Spring(1, 3),
        Spring(10, 5),
        Spring(1, 2),
      };

      SpringChain chain(0, 10, springs);
      for(size_t i = 0; i != springs.size() + 1; ++i) {
        std::cout << "Position " << i << ": "
          << chain.get(i) << std::endl;
      }
    }

.. END SECTION spring_chain_example.cpp

And here is the result:

.. include:: spring_chain_example.out
    :literal:
