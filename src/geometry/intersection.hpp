#pragma once

#include <geometry/vector.hpp>

namespace rt::geom {

class Intersection {
 public:
  Intersection(const Vector& pos, const Vector& norm, double dist) noexcept
    : position_(pos), normal_(norm), distance_(dist) {
  }

  [[nodiscard]] const Vector& GetPosition() const noexcept {
    return position_;
  }

  [[nodiscard]] const Vector& GetNormal() const noexcept {
    return normal_;
  }

  [[nodiscard]] double GetDistance() const noexcept {
    return distance_;
  }

 private:
  Vector position_;
  Vector normal_;
  double distance_;
};

}  // namespace rt::geom
