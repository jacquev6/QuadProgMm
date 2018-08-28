#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "QP.hpp"
using namespace QP;


BOOST_AUTO_TEST_CASE(OneVariableNoConstraint) {
  Variable a;
  {
    Solution s = minimize(a*a);
    BOOST_CHECK_EQUAL(s.getCost(), 0);
    BOOST_CHECK_EQUAL(s.get(a).value(), 0);
  }
  {
    Solution s = minimize(a*a - a);
    BOOST_CHECK_CLOSE(s.getCost(), -0.25, 0.001);
    BOOST_CHECK_CLOSE(s.get(a).value(), 0.5, 0.001);
  }
}

BOOST_AUTO_TEST_CASE(TwoVariablesNoConstraint) {
  Variable a, b, c, d;
  {
    Solution s = minimize(a*a + b*b);
    BOOST_CHECK_EQUAL(s.getCost(), 0);
    BOOST_CHECK_EQUAL(s.get(a).value(), 0);
    BOOST_CHECK_EQUAL(s.get(b).value(), 0);
    BOOST_CHECK_EQUAL(bool(s.get(c)), false);
    BOOST_CHECK_EQUAL(bool(s.get(d)), false);
  }
  {
    Solution s = minimize(a*a - a + b*b -2*b);
    BOOST_CHECK_CLOSE(s.getCost(), -1.25, 0.001);
    BOOST_CHECK_CLOSE(s.get(a).value(), 0.5, 0.001);
    BOOST_CHECK_CLOSE(s.get(b).value(), 1, 0.001);
    BOOST_CHECK_EQUAL(bool(s.get(c)), false);
    BOOST_CHECK_EQUAL(bool(s.get(d)), false);
  }
  {
    Solution s = minimize(c*c - c + b*b -2*b);
    BOOST_CHECK_CLOSE(s.getCost(), -1.25, 0.001);
    BOOST_CHECK_EQUAL(bool(s.get(a)), false);
    BOOST_CHECK_CLOSE(s.get(b).value(), 1, 0.001);
    BOOST_CHECK_CLOSE(s.get(c).value(), 0.5, 0.001);
    BOOST_CHECK_EQUAL(bool(s.get(d)), false);
  }
  {
    Solution s = minimize(a*a - a + d*d -2*d);
    BOOST_CHECK_CLOSE(s.getCost(), -1.25, 0.001);
    BOOST_CHECK_CLOSE(s.get(a).value(), 0.5, 0.001);
    BOOST_CHECK_EQUAL(bool(s.get(b)), false);
    BOOST_CHECK_EQUAL(bool(s.get(c)), false);
    BOOST_CHECK_CLOSE(s.get(d).value(), 1, 0.001);
  }
}
