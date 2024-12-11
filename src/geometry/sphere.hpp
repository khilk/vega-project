#pragma once

#include <geometry/vector.hpp>

namespace rt::geom {

class Sphere {
 public:
  Sphere(Vector center, double radius) noexcept : center_(center), radius_(radius) {
  }

  [[nodiscard]] const Vector& GetCenter() const noexcept {
    return center_;
  }

  [[nodiscard]] double GetRadius() const noexcept {
    return radius_;
  }

 private:
  Vector center_;
  double radius_;
};

}  // namespace rt::geom
