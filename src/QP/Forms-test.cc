#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "Forms.hpp"
using namespace QP;

void expect_linear_form(const LinearForm&) {}
void expect_quadratic_form(const QuadraticForm&) {}

BOOST_AUTO_TEST_CASE(Conversions) {
  Variable v;
  LinearForm l = 0;
  QuadraticForm q = 0;

  l = 0;
  l = v;
  l = l;

  q = 0;
  q = v;
  q = l;
  q = q;

  expect_linear_form(0);
  expect_linear_form(v);
  expect_linear_form(l);

  expect_quadratic_form(0);
  expect_quadratic_form(v);
  expect_quadratic_form(l);
  expect_quadratic_form(q);
}

BOOST_AUTO_TEST_CASE(UnarySignOperators) {
  Variable v;
  LinearForm l = 0;
  QuadraticForm q = 0;

  expect_linear_form(-v);
  expect_linear_form(+v);
  expect_linear_form(-l);
  expect_linear_form(+l);

  expect_quadratic_form(-v);
  expect_quadratic_form(+v);
  expect_quadratic_form(-l);
  expect_quadratic_form(+l);
  expect_quadratic_form(-q);
  expect_quadratic_form(+q);
}

BOOST_AUTO_TEST_CASE(AdditionOperators) {
  Variable v;
  LinearForm l = 0;
  QuadraticForm q = 0;

  expect_linear_form(v + v);
  expect_linear_form(v - v);
  expect_linear_form(v + 1);
  expect_linear_form(v - 1);
  expect_linear_form(1 + v);
  expect_linear_form(1 - v);

  l += l;
  l -= l;
  l += v;
  l -= v;
  l += 1;
  l -= 1;
  expect_linear_form(l + l);
  expect_linear_form(l - l);
  expect_linear_form(l + v);
  expect_linear_form(l - v);
  expect_linear_form(v + l);
  expect_linear_form(v - l);
  expect_linear_form(l + 1);
  expect_linear_form(l - 1);
  expect_linear_form(1 + l);
  expect_linear_form(1 - l);

  q += q;
  q -= q;
  q += l;
  q -= l;
  q += v;
  q -= v;
  q += 1;
  q -= 1;
  expect_quadratic_form(v + v);
  expect_quadratic_form(v - v);
  expect_quadratic_form(v + 1);
  expect_quadratic_form(v - 1);
  expect_quadratic_form(1 + v);
  expect_quadratic_form(1 - v);

  expect_quadratic_form(l + l);
  expect_quadratic_form(l - l);
  expect_quadratic_form(l + v);
  expect_quadratic_form(l - v);
  expect_quadratic_form(v + l);
  expect_quadratic_form(v - l);
  expect_quadratic_form(l + 1);
  expect_quadratic_form(l - 1);
  expect_quadratic_form(1 + l);
  expect_quadratic_form(1 - l);

  expect_quadratic_form(q + q);
  expect_quadratic_form(q - q);
  expect_quadratic_form(q + l);
  expect_quadratic_form(q - l);
  expect_quadratic_form(l + q);
  expect_quadratic_form(l - q);
  expect_quadratic_form(q + v);
  expect_quadratic_form(q - v);
  expect_quadratic_form(v + q);
  expect_quadratic_form(v - q);
  expect_quadratic_form(q + 1);
  expect_quadratic_form(q - 1);
  expect_quadratic_form(1 + q);
  expect_quadratic_form(1 - q);
}

BOOST_AUTO_TEST_CASE(MultiplicationOperators) {
  Variable v;
  LinearForm l = 0;
  QuadraticForm q = 0;

  expect_linear_form(v * 1);
  expect_linear_form(v / 1);
  expect_linear_form(1 * v);

  l *= 1;
  l /= 1;
  expect_linear_form(l * 1);
  expect_linear_form(l / 1);
  expect_linear_form(1 * l);

  q *= 1;
  q /= 1;
  expect_quadratic_form(q * 1);
  expect_quadratic_form(q / 1);
  expect_quadratic_form(1 * q);
  expect_quadratic_form(l * l);
  expect_quadratic_form(l * v);
  expect_quadratic_form(v * l);
  expect_quadratic_form(v * v);
}
