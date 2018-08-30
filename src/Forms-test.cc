#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "QuadProgMm.hpp"
using namespace QuadProgMm;

void expect_linear_form_(const LinearForm&) {}
void expect_quadratic_form_(const QuadraticForm&) {}
void expect_constraint_(const Constraint&) {}

#define EXPECT_LINEAR_FORM(x) expect_linear_form_(x); expect_quadratic_form_(x);
#define EXPECT_QUADRATIC_FORM(x) expect_quadratic_form_(x);
#define EXPECT_CONSTRAINT(x) expect_constraint_(x);

BOOST_AUTO_TEST_CASE(Conversions) {
  double d = 0;
  Variable v;
  LinearForm l;
  QuadraticForm q;

  EXPECT_LINEAR_FORM(d);
  EXPECT_LINEAR_FORM(v);
  EXPECT_LINEAR_FORM(l);
  EXPECT_QUADRATIC_FORM(q);

  // N/A: d = d;
  // N/A: d = v;
  // N/A: d = l;
  // N/A: d = q;

  // N/A: v = d;
  // N/A: v = v;
  // N/A: v = l;
  // N/A: v = q;

  l = d;
  l = v;
  l = l;
  // N/A: l = q;

  q = d;
  q = v;
  q = l;
  q = q;
}

BOOST_AUTO_TEST_CASE(UnarySignOperators) {
  Variable v;
  LinearForm l;
  QuadraticForm q;

  EXPECT_LINEAR_FORM(+v);
  EXPECT_LINEAR_FORM(-v);
  EXPECT_LINEAR_FORM(+l);
  EXPECT_LINEAR_FORM(-l);
  EXPECT_QUADRATIC_FORM(+q);
  EXPECT_QUADRATIC_FORM(-q);
}

BOOST_AUTO_TEST_CASE(AdditionOperators) {
  double d = 0;
  Variable v;
  LinearForm l;
  QuadraticForm q;

  // N/A: d d
  // d v
  // N/A: d += v;
  // N/A: d -= v;
  EXPECT_LINEAR_FORM(d + v);
  EXPECT_LINEAR_FORM(d - v);
  // d l
  // N/A: d += l;
  // N/A: d -= l;
  EXPECT_LINEAR_FORM(d + l);
  EXPECT_LINEAR_FORM(d - l);
  // d q
  // N/A: d += q;
  // N/A: d -= q;
  EXPECT_QUADRATIC_FORM(d + q);
  EXPECT_QUADRATIC_FORM(d - q);

  // v d
  // N/A: v += d;
  // N/A: v -= d;
  EXPECT_LINEAR_FORM(v + d);
  EXPECT_LINEAR_FORM(v - d);
  // v v
  // N/A: v += v;
  // N/A: v -= v;
  EXPECT_LINEAR_FORM(v + v);
  EXPECT_LINEAR_FORM(v - v);
  // v l
  // N/A: v += l;
  // N/A: v -= l;
  EXPECT_LINEAR_FORM(v + l);
  EXPECT_LINEAR_FORM(v - l);
  // v q
  // N/A: v += q;
  // N/A: v -= q;
  EXPECT_QUADRATIC_FORM(v + q);
  EXPECT_QUADRATIC_FORM(v - q);

  // l d
  l += d;
  l -= d;
  EXPECT_LINEAR_FORM(l + d);
  EXPECT_LINEAR_FORM(l - d);
  // l v
  l += v;
  l -= v;
  EXPECT_LINEAR_FORM(l + v);
  EXPECT_LINEAR_FORM(l - v);
  // l l
  l += l;
  l -= l;
  EXPECT_LINEAR_FORM(l + l);
  EXPECT_LINEAR_FORM(l - l);
  // l q
  // N/A: l += q;
  // N/A: l -= q;
  EXPECT_QUADRATIC_FORM(l + q);
  EXPECT_QUADRATIC_FORM(l - q);

  // q d
  q += d;
  q -= d;
  EXPECT_QUADRATIC_FORM(q + d);
  EXPECT_QUADRATIC_FORM(q - d);
  // q v
  q += v;
  q -= v;
  EXPECT_QUADRATIC_FORM(q + v);
  EXPECT_QUADRATIC_FORM(q - v);
  // q l
  q += l;
  q -= l;
  EXPECT_QUADRATIC_FORM(q + l);
  EXPECT_QUADRATIC_FORM(q - l);
  // q q
  q += q;
  q -= q;
  EXPECT_QUADRATIC_FORM(q + q);
  EXPECT_QUADRATIC_FORM(q - q);
}

BOOST_AUTO_TEST_CASE(MultiplicationOperators) {
  double d = 0;
  Variable v;
  LinearForm l;
  QuadraticForm q;

  // N/A: d d
  // d v
  // N/A: d *= v;
  // N/A: d /= v;
  EXPECT_LINEAR_FORM(d * v);
  // N/A: d / v;
  // d l
  // N/A: d *= l;
  // N/A: d /= l;
  EXPECT_LINEAR_FORM(d * l);
  // N/A: d / l;
  // d q
  // N/A: d *= q;
  // N/A: d /= q;
  EXPECT_QUADRATIC_FORM(d * q);
  // N/A: d / q;

  // v d
  // N/A: v *= d;
  // N/A: v /= d;
  EXPECT_LINEAR_FORM(v * d);
  EXPECT_LINEAR_FORM(v / d);
  // v v
  // N/A: v *= v;
  // N/A: v /= v;
  EXPECT_QUADRATIC_FORM(v * v);
  // N/A: v / v;
  // v l
  // N/A: v *= l;
  // N/A: v /= l;
  EXPECT_QUADRATIC_FORM(v * l);
  // N/A: v / l;
  // v q
  // N/A: v *= q;
  // N/A: v /= q;
  // N/A: v * q;
  // N/A: v / q;

  // l d
  l *= d;
  l /= d;
  EXPECT_LINEAR_FORM(l * d);
  EXPECT_LINEAR_FORM(l / d);
  // l v
  // N/A: l *= v;
  // N/A: l /= v;
  EXPECT_QUADRATIC_FORM(l * v);
  // N/A: l / v;
  // l l
  // N/A: l *= l;
  // N/A: l /= l;
  EXPECT_QUADRATIC_FORM(l * l);
  // N/A: l / l;
  // l q
  // N/A: l *= q;
  // N/A: l /= q;
  // N/A: l * q;
  // N/A: l / q;

  // q d
  q *= d;
  q /= d;
  EXPECT_QUADRATIC_FORM(q * d);
  EXPECT_QUADRATIC_FORM(q / d);
  // q v
  // N/A: q *= v;
  // N/A: q /= v;
  // N/A: q * v;
  // N/A: q / v;
  // q l
  // N/A: q *= l;
  // N/A: q /= l;
  // N/A: q * l;
  // N/A: q / l;
  // q q
  // N/A: q *= q;
  // N/A: q /= q;
  // N/A: q * q;
  // N/A: q / q;
}

BOOST_AUTO_TEST_CASE(ComparisonOperators) {
  double d = 0;
  Variable v;
  LinearForm l;

  // N/A: d d
  // d v
  EXPECT_CONSTRAINT(d == v)
  EXPECT_CONSTRAINT(d >= v)
  EXPECT_CONSTRAINT(d <= v)
  // d l
  EXPECT_CONSTRAINT(d == l)
  EXPECT_CONSTRAINT(d >= l)
  EXPECT_CONSTRAINT(d <= l)

  // v d
  EXPECT_CONSTRAINT(v == d)
  EXPECT_CONSTRAINT(v >= d)
  EXPECT_CONSTRAINT(v <= d)
  // v v
  EXPECT_CONSTRAINT(v == v)
  EXPECT_CONSTRAINT(v >= v)
  EXPECT_CONSTRAINT(v <= v)
  // v l
  EXPECT_CONSTRAINT(v == l)
  EXPECT_CONSTRAINT(v >= l)
  EXPECT_CONSTRAINT(v <= l)

  // l d
  EXPECT_CONSTRAINT(l == d)
  EXPECT_CONSTRAINT(l >= d)
  EXPECT_CONSTRAINT(l <= d)
  // l v
  EXPECT_CONSTRAINT(l == v)
  EXPECT_CONSTRAINT(l >= v)
  EXPECT_CONSTRAINT(l <= v)
  // l l
  EXPECT_CONSTRAINT(l == l)
  EXPECT_CONSTRAINT(l >= l)
  EXPECT_CONSTRAINT(l <= l)
}
