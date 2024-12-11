#pragma once

#include <geometry/vector.hpp>

namespace rt::geom {

class Ray {
 public:
  Ray(Vector origin, Vector direction) noexcept : origin_(origin), direction_(direction) {
    direction_.Normalize();
  }

  [[nodiscard]] const Vector& GetOrigin() const noexcept {
    return origin_;
  }

  [[nodiscard]] const Vector& GetDirection() const noexcept {
    return direction_;
  }

 private:
  Vector origin_;
  Vector direction_;
};

}  // namespace rt::geom
