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

auto d2(Variable left, Variable right, float l0) {
  auto d = (right - left) - l0;
  return d * d;
}

BOOST_AUTO_TEST_CASE(SpringChainExample) {
  Variable x0, x1, x2, x3, x4;
  Solution s = minimize(
    0.5 * (
      1 * d2(x0, x1, 2)
      + 1 * d2(x1, x2, 3)
      + 10 * d2(x2, x3, 5)
      + 1 * d2(x3, x4, 2)
    ),
    {x0 == 0, x4 == 10}
  );
  BOOST_CHECK_SMALL(s.get(x0).value(), 1e-8);
  BOOST_CHECK_CLOSE(s.get(x1).value(), 1.35484, 0.001);
  BOOST_CHECK_CLOSE(s.get(x2).value(), 3.70968, 0.001);
  BOOST_CHECK_CLOSE(s.get(x3).value(), 8.64516, 0.001);
  BOOST_CHECK_CLOSE(s.get(x4).value(), 10, 0.001);
  BOOST_CHECK_CLOSE(s.getCost(), 0.64516, 0.001);
}

BOOST_AUTO_TEST_CASE(QuickStart) {
  Variable a, b, c;
  Solution s = minimize(
    a + b + (a - b) * (a - b) + c + (b - c) * (b - c),
    {a <= 1, c >= 4, a - 2 * b <= 12}
  );
  BOOST_CHECK_CLOSE(s.get(a).value(), 1, 0.001);
  BOOST_CHECK_CLOSE(s.get(b).value(), 2, 0.001);
  BOOST_CHECK_CLOSE(s.get(c).value(), 4, 0.001);
  BOOST_CHECK_CLOSE(s.getCost(), 16, 0.001);
}
