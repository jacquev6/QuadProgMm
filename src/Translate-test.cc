#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "Translate.hpp"
using namespace QuadProgMm;

#define CHECK_SIZES(t) do { \
  BOOST_CHECK_EQUAL(t.variables.size(), t.G.nrows()); \
  BOOST_CHECK_EQUAL(t.G.ncols(), t.G.nrows()); \
  BOOST_CHECK_EQUAL(t.g0.size(), t.G.nrows()); \
  BOOST_CHECK_EQUAL(t.CE.nrows(), t.G.nrows()); \
  BOOST_CHECK_EQUAL(t.ce0.size(), t.CE.ncols()); \
  BOOST_CHECK_EQUAL(t.CI.nrows(), t.G.nrows()); \
  BOOST_CHECK_EQUAL(t.ci0.size(), t.CI.ncols()); \
} while(false)

#define CHECK_MATRIX(actual, expected) do { \
  const int rows = expected.size(); \
  BOOST_CHECK_EQUAL(actual.nrows(), rows); \
  if(rows) { \
    for(int row = 0; row != rows; ++row) { \
      const int cols = expected[row].size(); \
      BOOST_CHECK_EQUAL(actual.ncols(), cols); \
      for(int col = 0; col != cols; ++col) { \
        BOOST_CHECK_EQUAL(actual[row][col], expected[row][col]); \
      } \
    } \
  } else { \
    BOOST_CHECK_EQUAL(actual.ncols(), 0); \
  } \
} while(false)

#define CHECK_VECTOR(actual, expected) do { \
  const int n = expected.size(); \
  BOOST_CHECK_EQUAL(actual.size(), n); \
  for(int i = 0; i != n; ++i) { \
    BOOST_CHECK_EQUAL(actual[i], expected[i]); \
  } \
} while(false)

#define CHECK_QUADRATIC_EXPRESSION_TRANSLATION(q, _G, _G0, expected_g00) do { \
  const Translation t = translate(q, {}); \
  const std::vector<std::vector<double>> expected_G _G; \
  const std::vector<double> expected_g0 _G0; \
  CHECK_SIZES(t); \
  CHECK_MATRIX(t.G, expected_G); \
  CHECK_VECTOR(t.g0, expected_g0); \
  BOOST_CHECK_EQUAL(t.g00, expected_g00); \
} while(false)

BOOST_AUTO_TEST_CASE(TranslateQuadraticExpression) {
  Variable a, b;

  CHECK_QUADRATIC_EXPRESSION_TRANSLATION(0, {}, {}, 0);
  CHECK_QUADRATIC_EXPRESSION_TRANSLATION(1, {}, {}, 1);
  CHECK_QUADRATIC_EXPRESSION_TRANSLATION(a, {{0}}, {1}, 0);
  CHECK_QUADRATIC_EXPRESSION_TRANSLATION(1 + a, {{0}}, {1}, 1);
  CHECK_QUADRATIC_EXPRESSION_TRANSLATION(a*a, {{2}}, {0}, 0);
  CHECK_QUADRATIC_EXPRESSION_TRANSLATION(1 + a*a, {{2}}, {0}, 1);
  CHECK_QUADRATIC_EXPRESSION_TRANSLATION(1 + a + a*a, {{2}}, {1}, 1);
  CHECK_QUADRATIC_EXPRESSION_TRANSLATION(0.5*a*a, {{1}}, {0}, 0);
  CHECK_QUADRATIC_EXPRESSION_TRANSLATION(a*a + b*b, ({{2, 0}, {0, 2}}), ({0, 0}), 0);
  CHECK_QUADRATIC_EXPRESSION_TRANSLATION(a*a + 0.5*b*b, ({{2, 0}, {0, 1}}), ({0, 0}), 0);
  CHECK_QUADRATIC_EXPRESSION_TRANSLATION(0.5*b*b + a*a, ({{2, 0}, {0, 1}}), ({0, 0}), 0);
  CHECK_QUADRATIC_EXPRESSION_TRANSLATION(a*b, ({{0, 1}, {1, 0}}), ({0, 0}), 0);
  CHECK_QUADRATIC_EXPRESSION_TRANSLATION(b*a, ({{0, 1}, {1, 0}}), ({0, 0}), 0);
  CHECK_QUADRATIC_EXPRESSION_TRANSLATION(a*a + 2*b*b + 3*b*a + 4*a + 5*b + 6, ({{2, 3}, {3, 4}}), ({4, 5}), 6);
}

#define CHECK_EQUALITY_CONSTRAINT_TRANSLATION(c, _CE, _CE0) do { \
  const Translation t = translate(0, {c}); \
  const std::vector<std::vector<double>> expected_CE _CE; \
  const std::vector<double> expected_ce0 _CE0; \
  CHECK_SIZES(t); \
  CHECK_MATRIX(t.CE, expected_CE); \
  CHECK_VECTOR(t.ce0, expected_ce0); \
} while(false)

BOOST_AUTO_TEST_CASE(TranslateEqualityConstraint) {
  Variable a, b;

  CHECK_EQUALITY_CONSTRAINT_TRANSLATION(a == 0, {{1}}, {0});
  CHECK_EQUALITY_CONSTRAINT_TRANSLATION(a == 1, {{1}}, {-1});
  CHECK_EQUALITY_CONSTRAINT_TRANSLATION(2*a == 1, {{2}}, {-1});
  CHECK_EQUALITY_CONSTRAINT_TRANSLATION(a == 0.5, {{1}}, {-0.5});
  CHECK_EQUALITY_CONSTRAINT_TRANSLATION(a - a == 0, {{0}}, {0});
  CHECK_EQUALITY_CONSTRAINT_TRANSLATION(a + b == 0, ({{1}, {1}}), {0});
  CHECK_EQUALITY_CONSTRAINT_TRANSLATION(a + b + 2 == 0, ({{1}, {1}}), {2});
  CHECK_EQUALITY_CONSTRAINT_TRANSLATION(b + a + 2 == 0, ({{1}, {1}}), {2});
  CHECK_EQUALITY_CONSTRAINT_TRANSLATION(a + 2*b + 3 == 0, ({{1}, {2}}), {3});
  CHECK_EQUALITY_CONSTRAINT_TRANSLATION(b + 2*a + 3 == 0, ({{2}, {1}}), {3});
}

#define CHECK_INEQUALITY_CONSTRAINT_TRANSLATION(c, _CI, _CI0) do { \
  const Translation t = translate(0, {c}); \
  const std::vector<std::vector<double>> expected_CI _CI; \
  const std::vector<double> expected_ci0 _CI0; \
  CHECK_SIZES(t); \
  CHECK_MATRIX(t.CI, expected_CI); \
  CHECK_VECTOR(t.ci0, expected_ci0); \
} while(false)

BOOST_AUTO_TEST_CASE(TranslateGreaterThanConstraint) {
  Variable a, b;

  CHECK_INEQUALITY_CONSTRAINT_TRANSLATION(a >= 0, {{1}}, {0});
  CHECK_INEQUALITY_CONSTRAINT_TRANSLATION(a >= 1, {{1}}, {-1});
  CHECK_INEQUALITY_CONSTRAINT_TRANSLATION(2*a >= 1, {{2}}, {-1});
  CHECK_INEQUALITY_CONSTRAINT_TRANSLATION(a >= 0.5, {{1}}, {-0.5});
  CHECK_INEQUALITY_CONSTRAINT_TRANSLATION(a - a >= 0, {{0}}, {0});
  CHECK_INEQUALITY_CONSTRAINT_TRANSLATION(a + b >= 0, ({{1}, {1}}), {0});
  CHECK_INEQUALITY_CONSTRAINT_TRANSLATION(a + b + 2 >= 0, ({{1}, {1}}), {2});
  CHECK_INEQUALITY_CONSTRAINT_TRANSLATION(b + a + 2 >= 0, ({{1}, {1}}), {2});
  CHECK_INEQUALITY_CONSTRAINT_TRANSLATION(a + 2*b + 3 >= 0, ({{1}, {2}}), {3});
  CHECK_INEQUALITY_CONSTRAINT_TRANSLATION(b + 2*a + 3 >= 0, ({{2}, {1}}), {3});
}

BOOST_AUTO_TEST_CASE(TranslateLessThanConstraint) {
  Variable a, b;

  CHECK_INEQUALITY_CONSTRAINT_TRANSLATION(a <= 0, {{-1}}, {0});
  CHECK_INEQUALITY_CONSTRAINT_TRANSLATION(a <= 1, {{-1}}, {1});
  CHECK_INEQUALITY_CONSTRAINT_TRANSLATION(2*a <= 1, {{-2}}, {1});
  CHECK_INEQUALITY_CONSTRAINT_TRANSLATION(a <= 0.5, {{-1}}, {0.5});
  CHECK_INEQUALITY_CONSTRAINT_TRANSLATION(a - a <= 0, {{0}}, {0});
  CHECK_INEQUALITY_CONSTRAINT_TRANSLATION(a + b <= 0, ({{-1}, {-1}}), {0});
  CHECK_INEQUALITY_CONSTRAINT_TRANSLATION(a + b + 2 <= 0, ({{-1}, {-1}}), {-2});
  CHECK_INEQUALITY_CONSTRAINT_TRANSLATION(b + a + 2 <= 0, ({{-1}, {-1}}), {-2});
  CHECK_INEQUALITY_CONSTRAINT_TRANSLATION(a + 2*b + 3 <= 0, ({{-1}, {-2}}), {-3});
  CHECK_INEQUALITY_CONSTRAINT_TRANSLATION(b + 2*a + 3 <= 0, ({{-2}, {-1}}), {-3});
}

auto d2(Variable left, Variable right, float l0) {
  auto d = (right - left) - l0;
  return d * d;
}

BOOST_AUTO_TEST_CASE(MixedConstraints) {
  Variable a, b, c;

  Translation t = translate(0, {
    a == 1,
    a <= 4,
    b >= 8,
    a >= 7,
    b == 2,
    c >= 9,
    b <= 5,
    c <= 6,
  });

  CHECK_SIZES(t);

  CHECK_MATRIX(t.CE, std::vector<std::vector<double>>({{1, 0}, {0, 1}, {0, 0}}));
  CHECK_VECTOR(t.ce0, std::vector<double>({-1, -2}));

  CHECK_MATRIX(t.CI, std::vector<std::vector<double>>({{-1, 0, 1, 0, 0, 0}, {0, 1, 0, 0, -1, 0}, {0, 0, 0, 1, 0, -1}}));
  CHECK_VECTOR(t.ci0, std::vector<double>({4, -8, -7, -9, 5, 6}));
}

BOOST_AUTO_TEST_CASE(SpringChainExample) {
  Variable x0, x1, x2, x3, x4;
  Translation t = translate(
    0.5 * (
      1 * d2(x0, x1, 2)
      + 1 * d2(x1, x2, 3)
      + 10 * d2(x2, x3, 5)
      + 1 * d2(x3, x4, 2)
    ),
    {x0 == 0, x4 == 10}
  );

  CHECK_SIZES(t);

  CHECK_MATRIX(t.G, std::vector<std::vector<double>>({
    { 1, -1,   0,   0,  0},
    {-1,  2,  -1,   0,  0},
    { 0, -1,  11, -10,  0},
    { 0,  0, -10,  11, -1},
    { 0,  0,   0,  -1,  1},
  }));
  CHECK_VECTOR(t.g0, std::vector<double>({2, 1, 47, -48, -2}));
  BOOST_CHECK_EQUAL(t.g00, 133.5);

  CHECK_MATRIX(t.CE, std::vector<std::vector<double>>({{1, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 1}}));
  CHECK_VECTOR(t.ce0, std::vector<double>({0, -10}));

  CHECK_MATRIX(t.CI, std::vector<std::vector<double>>({{}, {}, {}, {}, {}}));
  CHECK_VECTOR(t.ci0, std::vector<double>({}));
}

BOOST_AUTO_TEST_CASE(QuickStart) {
  Variable a, b, c;

  Translation t = translate(
    a + b + (a - b) * (a - b) + c + (b - c) * (b - c),
    {a <= 1, c >= 4, a - 2 * b <= 12}
  );

  CHECK_SIZES(t);

  CHECK_MATRIX(t.G, std::vector<std::vector<double>>({
    {2, -2, 0},
    {-2, 4, -2},
    {0, -2, 2},
  }));
  CHECK_VECTOR(t.g0, std::vector<double>({1, 1, 1}));
  BOOST_CHECK_EQUAL(t.g00, 0);

  CHECK_MATRIX(t.CE, std::vector<std::vector<double>>({{}, {}, {}}));
  CHECK_VECTOR(t.ce0, std::vector<double>({}));

  CHECK_MATRIX(t.CI, std::vector<std::vector<double>>({
    {-1, 0, -1},
    {0, 0, 2},
    {0, 1, 0},
  }));
  CHECK_VECTOR(t.ci0, std::vector<double>({1, -4, 12}));
}
