#include <raytracer/image.hpp>

#include <cmath>
#include <string>

#include <gtest/gtest.h>

[[nodiscard]] std::string ConstructBasePath() {
  std::string result("../models");
  return result;
}

[[nodiscard]] double PixelDistance(const rt::image::RGB& lhs, const rt::image::RGB& rhs) {
  return sqrt(std::pow(lhs.r - rhs.r, 2.0) + std::pow(lhs.g - rhs.g, 2.0) + std::pow(lhs.b - rhs.b, 2.0));
}

void Compare(const rt::image::Image& actual, const rt::image::Image& expected) {
  static const double kEps = 2;
  int matches = 0;

  EXPECT_EQ(actual.Width(), expected.Width());
  EXPECT_EQ(actual.Height(), expected.Height());
  for (int y = 0; y < actual.Height(); ++y) {
    for (int x = 0; x < actual.Width(); ++x) {
      auto actual_data = actual.GetPixel(y, x);
      auto expected_data = expected.GetPixel(y, x);
      auto diff = PixelDistance(actual_data, expected_data);
      matches += diff < kEps;
    }
  }

  double similarity = static_cast<double>(matches) / (actual.Width() * actual.Height());
  EXPECT_GE(similarity, 0.99);
}
