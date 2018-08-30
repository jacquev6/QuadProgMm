#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "QuadProgMm.hpp"
using namespace QuadProgMm;

void expect_linear_expression_(const LinearExpression&) {}
void expect_quadratic_expression_(const QuadraticExpression&) {}
void expect_constraint_(const Constraint&) {}

#define EXPECT_LINEAR_EXPRESSION(x) expect_linear_expression_(x); expect_quadratic_expression_(x);
#define EXPECT_QUADRATIC_EXPRESSION(x) expect_quadratic_expression_(x);
#define EXPECT_CONSTRAINT(x) expect_constraint_(x);

BOOST_AUTO_TEST_CASE(Conversions) {
  double d = 0;
  Variable v;
  LinearExpression l;
  QuadraticExpression q;

  EXPECT_LINEAR_EXPRESSION(d);
  EXPECT_LINEAR_EXPRESSION(v);
  EXPECT_LINEAR_EXPRESSION(l);
  EXPECT_QUADRATIC_EXPRESSION(q);

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
  LinearExpression l;
  QuadraticExpression q;

  EXPECT_LINEAR_EXPRESSION(+v);
  EXPECT_LINEAR_EXPRESSION(-v);
  EXPECT_LINEAR_EXPRESSION(+l);
  EXPECT_LINEAR_EXPRESSION(-l);
  EXPECT_QUADRATIC_EXPRESSION(+q);
  EXPECT_QUADRATIC_EXPRESSION(-q);
}

BOOST_AUTO_TEST_CASE(AdditionOperators) {
  double d = 0;
  Variable v;
  LinearExpression l;
  QuadraticExpression q;

  // N/A: d d
  // d v
  // N/A: d += v;
  // N/A: d -= v;
  EXPECT_LINEAR_EXPRESSION(d + v);
  EXPECT_LINEAR_EXPRESSION(d - v);
  // d l
  // N/A: d += l;
  // N/A: d -= l;
  EXPECT_LINEAR_EXPRESSION(d + l);
  EXPECT_LINEAR_EXPRESSION(d - l);
  // d q
  // N/A: d += q;
  // N/A: d -= q;
  EXPECT_QUADRATIC_EXPRESSION(d + q);
  EXPECT_QUADRATIC_EXPRESSION(d - q);

  // v d
  // N/A: v += d;
  // N/A: v -= d;
  EXPECT_LINEAR_EXPRESSION(v + d);
  EXPECT_LINEAR_EXPRESSION(v - d);
  // v v
  // N/A: v += v;
  // N/A: v -= v;
  EXPECT_LINEAR_EXPRESSION(v + v);
  EXPECT_LINEAR_EXPRESSION(v - v);
  // v l
  // N/A: v += l;
  // N/A: v -= l;
  EXPECT_LINEAR_EXPRESSION(v + l);
  EXPECT_LINEAR_EXPRESSION(v - l);
  // v q
  // N/A: v += q;
  // N/A: v -= q;
  EXPECT_QUADRATIC_EXPRESSION(v + q);
  EXPECT_QUADRATIC_EXPRESSION(v - q);

  // l d
  l += d;
  l -= d;
  EXPECT_LINEAR_EXPRESSION(l + d);
  EXPECT_LINEAR_EXPRESSION(l - d);
  // l v
  l += v;
  l -= v;
  EXPECT_LINEAR_EXPRESSION(l + v);
  EXPECT_LINEAR_EXPRESSION(l - v);
  // l l
  l += l;
  l -= l;
  EXPECT_LINEAR_EXPRESSION(l + l);
  EXPECT_LINEAR_EXPRESSION(l - l);
  // l q
  // N/A: l += q;
  // N/A: l -= q;
  EXPECT_QUADRATIC_EXPRESSION(l + q);
  EXPECT_QUADRATIC_EXPRESSION(l - q);

  // q d
  q += d;
  q -= d;
  EXPECT_QUADRATIC_EXPRESSION(q + d);
  EXPECT_QUADRATIC_EXPRESSION(q - d);
  // q v
  q += v;
  q -= v;
  EXPECT_QUADRATIC_EXPRESSION(q + v);
  EXPECT_QUADRATIC_EXPRESSION(q - v);
  // q l
  q += l;
  q -= l;
  EXPECT_QUADRATIC_EXPRESSION(q + l);
  EXPECT_QUADRATIC_EXPRESSION(q - l);
  // q q
  q += q;
  q -= q;
  EXPECT_QUADRATIC_EXPRESSION(q + q);
  EXPECT_QUADRATIC_EXPRESSION(q - q);
}

BOOST_AUTO_TEST_CASE(MultiplicationOperators) {
  double d = 0;
  Variable v;
  LinearExpression l;
  QuadraticExpression q;

  // N/A: d d
  // d v
  // N/A: d *= v;
  // N/A: d /= v;
  EXPECT_LINEAR_EXPRESSION(d * v);
  // N/A: d / v;
  // d l
  // N/A: d *= l;
  // N/A: d /= l;
  EXPECT_LINEAR_EXPRESSION(d * l);
  // N/A: d / l;
  // d q
  // N/A: d *= q;
  // N/A: d /= q;
  EXPECT_QUADRATIC_EXPRESSION(d * q);
  // N/A: d / q;

  // v d
  // N/A: v *= d;
  // N/A: v /= d;
  EXPECT_LINEAR_EXPRESSION(v * d);
  EXPECT_LINEAR_EXPRESSION(v / d);
  // v v
  // N/A: v *= v;
  // N/A: v /= v;
  EXPECT_QUADRATIC_EXPRESSION(v * v);
  // N/A: v / v;
  // v l
  // N/A: v *= l;
  // N/A: v /= l;
  EXPECT_QUADRATIC_EXPRESSION(v * l);
  // N/A: v / l;
  // v q
  // N/A: v *= q;
  // N/A: v /= q;
  // N/A: v * q;
  // N/A: v / q;

  // l d
  l *= d;
  l /= d;
  EXPECT_LINEAR_EXPRESSION(l * d);
  EXPECT_LINEAR_EXPRESSION(l / d);
  // l v
  // N/A: l *= v;
  // N/A: l /= v;
  EXPECT_QUADRATIC_EXPRESSION(l * v);
  // N/A: l / v;
  // l l
  // N/A: l *= l;
  // N/A: l /= l;
  EXPECT_QUADRATIC_EXPRESSION(l * l);
  // N/A: l / l;
  // l q
  // N/A: l *= q;
  // N/A: l /= q;
  // N/A: l * q;
  // N/A: l / q;

  // q d
  q *= d;
  q /= d;
  EXPECT_QUADRATIC_EXPRESSION(q * d);
  EXPECT_QUADRATIC_EXPRESSION(q / d);
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
  LinearExpression l;

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
