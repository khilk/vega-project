#include <geometry/geometry.hpp>

#include <cmath>
#include <optional>

#include <gtest/gtest.h>

namespace {

constexpr double kX = 123.;
constexpr double kY = 456.;
constexpr double kZ = 789.;
constexpr double kErr = 1e-6;

using namespace rt::geom;

TEST(InitializeVector, Raytracer) {
  {
    const Vector k_vec{kX, kY, kZ};
    EXPECT_LT(std::fabs(k_vec[0] - kX), kErr);
    EXPECT_LT(std::fabs(k_vec[1] - kY), kErr);
    EXPECT_LT(std::fabs(k_vec[2] - kZ), kErr);
  }
  {
    Vector vec{};
    vec[0] = kX;
    vec[1] = kY;
    vec[2] = kZ;
    EXPECT_LT(std::fabs(vec[0] - kX), kErr);
    EXPECT_LT(std::fabs(vec[1] - kY), kErr);
    EXPECT_LT(std::fabs(vec[2] - kZ), kErr);
  }
}

TEST(NormalizeLength, Raytracer) {
  Vector fst{kX, 0., 0.};
  EXPECT_LT(std::fabs(Length(fst) - kX), kErr);
  fst.Normalize();
  EXPECT_LT(std::fabs(fst[0] - 1.), kErr);

  Vector snd{0., kY, 0.};
  EXPECT_LT(std::fabs(Length(snd) - kY), kErr);
  snd.Normalize();
  EXPECT_LT(std::fabs(snd[1] - 1.), kErr);

  Vector trd{0., 0., kZ};
  EXPECT_LT(std::fabs(Length(trd) - kZ), kErr);
  trd.Normalize();
  EXPECT_LT(std::fabs(trd[2] - 1.), kErr);

  Vector vec{kX, kY, kZ};
  vec.Normalize();
  EXPECT_LT(std::fabs(Length(vec) - 1.), kErr);
}

TEST(DotProduct, Raytracer) {
  {
    const Vector lhs{kX, 1., 0.};
    const Vector rhs{1., -kX, 0.};
    EXPECT_LT(std::fabs(DotProduct(lhs, rhs)), kErr);
  }
  {
    const Vector lhs{0., kY, -1.};
    const Vector rhs{0., 1., kY};
    EXPECT_LT(std::fabs(DotProduct(lhs, rhs)), kErr);
  }
  {
    Vector vec{kX, kY, kZ};
    EXPECT_LT(std::fabs(DotProduct(vec, {1., 0., 0.}) - kX), kErr);
    EXPECT_LT(std::fabs(DotProduct(vec, {0., 1., 0.}) - kY), kErr);
    EXPECT_LT(std::fabs(DotProduct(vec, {0., 0., 1.}) - kZ), kErr);
  }
}

TEST(CrossProduct, Raytracer) {
  {
    const Vector lhs{kX, 0., 0.};
    const Vector rhs{0., kY, 0.};
    EXPECT_LT(std::fabs(CrossProduct(lhs, rhs)[2] - kX * kY), kErr);
  }
  {
    const Vector lhs{0., kY, 0.};
    const Vector rhs{0., 0., kZ};
    EXPECT_LT(std::fabs(CrossProduct(lhs, rhs)[0] - kY * kZ), kErr);
  }
  {
    const Vector lhs{0., 0., kZ};
    const Vector rhs{kX, 0., 0.};
    EXPECT_LT(std::fabs(CrossProduct(lhs, rhs)[1] - kZ * kX), kErr);
  }
}

TEST(Triangle, Raytracer) {
  {
    Triangle triangle{{kX, 0., 0.}, {0., kY, 0.}, {0., 0., 0.}};
    EXPECT_LT(std::fabs(triangle.GetVertex(0)[0] - kX), kErr);
    EXPECT_LT(std::fabs(triangle.GetVertex(1)[1] - kY), kErr);
    EXPECT_LT(std::fabs(triangle.Area() - 0.5 * kX * kY), kErr);
  }
  {
    Triangle triangle{{1., 0., 0.}, {1., kY, 0.}, {1., 0., kZ}};
    EXPECT_LT(std::fabs(triangle.GetVertex(1)[1] - kY), kErr);
    EXPECT_LT(std::fabs(triangle.GetVertex(2)[2] - kZ), kErr);
    EXPECT_LT(std::fabs(triangle.Area() - 0.5 * kY * kZ), kErr);
  }
  {
    Triangle triangle{{0., 2., 1. + kZ}, {0., 2., 1.}, {kX, 2., 1.}};
    EXPECT_LT(std::fabs(triangle.GetVertex(0)[2] - 1 - kZ), kErr);
    EXPECT_LT(std::fabs(triangle.GetVertex(2)[0] - kX), kErr);
    EXPECT_LT(std::fabs(triangle.Area() - 0.5 * kZ * kX), kErr);
  }
}

TEST(Intersection, Raytracer) {
  Sphere sphere({0, 0, 0}, 2.);
  Ray ray{{5, 0, 2.2}, {-1, 0, 0}};
  auto intersection = GetIntersection(ray, sphere);
  EXPECT_TRUE(!intersection);

  ray = {{5, 0, 0}, {-1, 0, 0}};
  intersection = GetIntersection(ray, sphere);
  EXPECT_TRUE(intersection);
  EXPECT_LT(std::fabs(intersection->GetPosition()[0] - 2), kErr);
  EXPECT_LT(std::fabs(intersection->GetNormal()[0] - 1), kErr);
  EXPECT_LT(std::fabs(intersection->GetDistance() - 3), kErr);

  ray = {{5, 0, 2}, {-1, 0, 0}};
  intersection = GetIntersection(ray, sphere);
  EXPECT_TRUE(intersection);
  EXPECT_LT(std::fabs(intersection->GetPosition()[2] - 2), kErr);
  EXPECT_LT(std::fabs(intersection->GetNormal()[2] - 1), kErr);
  EXPECT_LT(std::fabs(intersection->GetDistance() - 5), kErr);

  ray = {{0, 0, 0}, {-1, 0, 0}};
  intersection = GetIntersection(ray, sphere);
  EXPECT_TRUE(intersection);
  EXPECT_LT(std::fabs(intersection->GetPosition()[0] + 2), kErr);
  EXPECT_LT(std::fabs(intersection->GetNormal()[0] - 1), kErr);
  EXPECT_LT(std::fabs(intersection->GetDistance() - 2), kErr);

  Triangle triangle{{0, 0, 0}, {4, 0, 0}, {0, 4, 0}};
  ray = {{2, 2, 1}, {0, 0, -1}};
  intersection = GetIntersection(ray, triangle);
  EXPECT_LT(std::fabs(intersection->GetPosition()[0] - 2), kErr);
  EXPECT_LT(std::fabs(intersection->GetPosition()[1] - 2), kErr);
  EXPECT_LT(std::fabs(intersection->GetNormal()[2] - 1), kErr);
  EXPECT_LT(std::fabs(intersection->GetDistance() - 1), kErr);

  ray = {{3, 3, 1}, {-1, -1, 0}};
  intersection = GetIntersection(ray, triangle);
  EXPECT_TRUE(!intersection);
}

TEST(RefractReflect, Raytracer) {
  Vector normal{0, 1, 0};
  Vector ray{0.707107, -0.707107, 0};
  auto reflect = Reflect(ray, normal);
  EXPECT_LT(std::fabs(reflect[0] - 0.707107), kErr);
  EXPECT_LT(std::fabs(reflect[1] - 0.707107), kErr);

  auto refract_opt = Refract(ray, normal, 0.9);
  EXPECT_LT(std::fabs(refract_opt.value()[0] - 0.636396), kErr);
  EXPECT_LT(std::fabs(refract_opt.value()[1] - (-0.771362)), kErr);
}

TEST(BarycentricCoords, Raytracer) {
  Triangle triangle{{0, 0, 0}, {2, 0, 0}, {0, 2, 0}};
  auto on_edge = GetBarycentricCoords(triangle, {1, 1, 0});
  EXPECT_LT(std::fabs(on_edge[1] - 0.5), kErr);
  EXPECT_LT(std::fabs(on_edge[2] - 0.5), kErr);

  auto on_vertex = GetBarycentricCoords(triangle, {2, 0, 0});
  EXPECT_LT(std::fabs(on_vertex[1] - 1), kErr);

  auto inside = GetBarycentricCoords(triangle, {0.2, 0.2, 0});
  EXPECT_LT(std::fabs(inside[0] - 0.8), kErr);
  EXPECT_LT(std::fabs(inside[1] - 0.1), kErr);
  EXPECT_LT(std::fabs(inside[2] - 0.1), kErr);
}

}  // namespace
