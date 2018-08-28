#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "Forms.hpp"

BOOST_AUTO_TEST_CASE(Conversions) {
  QP::Variable v;
  QP::LinearForm l = v;
  QP::QuadraticForm q1 = v;
  QP::QuadraticForm q2 = l;
}
