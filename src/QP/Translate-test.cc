#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "Translate.hpp"
using namespace QP;


#define CHECK_SIZES(t) do { \
  BOOST_CHECK_EQUAL(t.G.nrows(), t.n); \
  BOOST_CHECK_EQUAL(t.G.ncols(), t.n); \
  BOOST_CHECK_EQUAL(t.G0.size(), t.n); \
  BOOST_CHECK_EQUAL(t.CE.nrows(), t.n); \
  BOOST_CHECK_EQUAL(t.CE.ncols(), t.p); \
  BOOST_CHECK_EQUAL(t.CE0.size(), t.p); \
  BOOST_CHECK_EQUAL(t.CI.nrows(), t.n); \
  BOOST_CHECK_EQUAL(t.CI.ncols(), t.m); \
  BOOST_CHECK_EQUAL(t.CI0.size(), t.m); \
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
  for(int i = 0; i != n; ++i) { \
    BOOST_CHECK_EQUAL(actual[i], expected[i]); \
  } \
} while(false)

#define CHECK_QUADRATIC_FORM_TRANSLATION(q, _G, _G0, expected_baseCost) do { \
  const Translation t = translate(q, {}); \
  const std::vector<std::vector<double>> expected_G _G; \
  const std::vector<double> expected_G0 _G0; \
  CHECK_SIZES(t); \
  CHECK_MATRIX(t.G, expected_G); \
  CHECK_VECTOR(t.G0, expected_G0); \
  BOOST_CHECK_EQUAL(t.baseCost, expected_baseCost); \
} while(false)

BOOST_AUTO_TEST_CASE(TranslateQuadraticForm) {
  Variable a, b;

  CHECK_QUADRATIC_FORM_TRANSLATION(0, {}, {}, 0);
  CHECK_QUADRATIC_FORM_TRANSLATION(1, {}, {}, 1);
  CHECK_QUADRATIC_FORM_TRANSLATION(a, {{0}}, {1}, 0);
  CHECK_QUADRATIC_FORM_TRANSLATION(1 + a, {{0}}, {1}, 1);
  CHECK_QUADRATIC_FORM_TRANSLATION(a*a, {{2}}, {0}, 0);
  CHECK_QUADRATIC_FORM_TRANSLATION(1 + a*a, {{2}}, {0}, 1);
  CHECK_QUADRATIC_FORM_TRANSLATION(1 + a + a*a, {{2}}, {1}, 1);
  CHECK_QUADRATIC_FORM_TRANSLATION(0.5*a*a, {{1}}, {0}, 0);
  CHECK_QUADRATIC_FORM_TRANSLATION(a*a + b*b, ({{2, 0}, {0, 2}}), ({0, 0}), 0);
  CHECK_QUADRATIC_FORM_TRANSLATION(a*a + 0.5*b*b, ({{2, 0}, {0, 1}}), ({0, 0}), 0);
  CHECK_QUADRATIC_FORM_TRANSLATION(0.5*b*b + a*a, ({{2, 0}, {0, 1}}), ({0, 0}), 0);
  CHECK_QUADRATIC_FORM_TRANSLATION(a*b, ({{0, 1}, {1, 0}}), ({0, 0}), 0);
  CHECK_QUADRATIC_FORM_TRANSLATION(b*a, ({{0, 1}, {1, 0}}), ({0, 0}), 0);
  CHECK_QUADRATIC_FORM_TRANSLATION(a*a + 2*b*b + 3*b*a + 4*a + 5*b + 6, ({{2, 3}, {3, 4}}), ({4, 5}), 6);
}

#define CHECK_EQUALITY_CONSTRAINT_TRANSLATION(c, _CE, _CE0) do { \
  const Translation t = translate(0, {c}); \
  const std::vector<std::vector<double>> expected_CE _CE; \
  const std::vector<double> expected_CE0 _CE0; \
  CHECK_SIZES(t); \
  CHECK_MATRIX(t.CE, expected_CE); \
  CHECK_VECTOR(t.CE0, expected_CE0); \
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
  // CHECK_EQUALITY_CONSTRAINT_TRANSLATION(a + 2*b + 3 == 0, ({{1}, {2}}), {3});
  // CHECK_EQUALITY_CONSTRAINT_TRANSLATION(b + 2*a + 3 == 0, ({{2}, {1}}), {3});
}

#define CHECK_INEQUALITY_CONSTRAINT_TRANSLATION(c, _CI, _CI0) do { \
  const Translation t = translate(0, {c}); \
  const std::vector<std::vector<double>> expected_CI _CI; \
  const std::vector<double> expected_CI0 _CI0; \
  CHECK_SIZES(t); \
  CHECK_MATRIX(t.CI, expected_CI); \
  CHECK_VECTOR(t.CI0, expected_CI0); \
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
  // CHECK_INEQUALITY_CONSTRAINT_TRANSLATION(a + 2*b + 3 >= 0, ({{1}, {2}}), {3});
  // CHECK_INEQUALITY_CONSTRAINT_TRANSLATION(b + 2*a + 3 >= 0, ({{2}, {1}}), {3});
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
  // CHECK_INEQUALITY_CONSTRAINT_TRANSLATION(a + 2*b + 3 <= 0, ({{-1}, {-2}}), {-3});
  // CHECK_INEQUALITY_CONSTRAINT_TRANSLATION(b + 2*a + 3 <= 0, ({{-2}, {-1}}), {-3});
}
