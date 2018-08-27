Spring chain example
====================

.. BEGIN SECTION spring_chain_example.cpp

Let's say that you want to compute the balance positions of a chain of springs.

.. @todo Add image http://www.physics.utah.edu/~belz/phys3730/lab26/oscillators.png
.. @todo Add link to full source code

Start with includes::

    #include <iostream>
    #include <QP/QP.hpp>

Define a class representing a spring.
It's a simple aggregate with the spring's strength, and unconstrained length::

    struct Spring {
      Spring(float strength_, float length_) :
        strength(strength_), length(length_)
      {
      }

      float strength;
      float length;
    };

Then, a class for the chain.
It has two boundaries (``left`` and ``right``),
a vector of ``Spring``, and a vector of ``QP::Variable`` for the positions to be computed::

    class SpringChain {
    public:
      SpringChain(
        float left_, float right_,
        const std::vector<Spring>& springs_
      ) :
        left(left_), right(right_),
        springs(springs_),
        positions(makePositions(springs_))
      {
        resolve();
      }

      float get(size_t i) const {
        return positions[i].getValue();
      }

    private:
      static std::vector<QP::Variable> makePositions(
        const std::vector<Spring>& springs
      ) {
        std::vector<QP::Variable> positions;
        for(size_t i = 0; i != springs.size() + 1; ++i) {
          positions.push_back(QP::Variable());
        }
        return positions;
      }

      void resolve();

    private:
      float left, right;
      std::vector<Spring> springs;
      std::vector<QP::Variable> positions;
    };

``makePositions`` is required because ``QP::Variable`` has a pointer semantic:
when a ``QP::Variable`` is copied, the copy refers to the same resolvable entity as the original.
That's why an original ``QP::Variable`` instance is required for each position to compute.

Then, the most important part: the definition of ``SpringChain::resolve``::

    void SpringChain::resolve() {

First, define the objectives: each spring has a `potential energy <https://en.wikipedia.org/wiki/Elastic_energy>`_ of \\(1/2 \\cdot k \\cdot (l - l_0) ^ 2\\),
where \\(k\\) is its strength, \\(l_0\\) is its unconstrained length, and \\(l\\) is its current length.
The balance position is reached when the total energy is minimal:
the objective is to minimize the sum of all those energies.
So, fill a ``QP::Objectives`` (a vector) with one objective per spring::

      QP::Objectives objectives;
      for(size_t i = 0; i != springs.size(); ++i) {
        QP::LinearForm l_minus_l0 =
          positions[i + 1] - positions[i]
          - springs[i].length;

        objectives.push_back(QP::Objective::Minimize(
          0.5 * springs[i].strength * l_minus_l0 * l_minus_l0
        ));
      }

Add the constraints on boundaries::

      QP::Constraints constraints;
      constraints.push_back(positions.front() == left);
      constraints.push_back(positions.back() == right);

And solve the QP problem::

      QP::Problem p(objectives, constraints);
      p.solve();
    }

Finally, use the ``SpringChain`` class::

    int main() {
      std::vector<Spring> springs;
      springs.push_back(Spring(1, 2));
      springs.push_back(Spring(1, 3));
      springs.push_back(Spring(10, 5));
      springs.push_back(Spring(1, 2));

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

In this example, you have seen the basics of how to express objectives and constraints as arithmetic expressions of C++ variables.
Dealing with vectors and matrices is the responibility of the library.
