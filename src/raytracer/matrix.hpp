#pragma once
#include <geometry/vector.hpp>

#include <array>

namespace rt {

struct Matrix {
  double epsilon = 0.0000001;
  [[nodiscard]] std::array<double, 4>& operator[](std::size_t ind) noexcept {
    return matrix_[ind];
  }
  [[nodiscard]] geom::Vector multiply_vector(const geom::Vector& v) const noexcept;
  [[nodiscard]] const std::array<double, 4>& operator[](std::size_t ind) const noexcept {
    return matrix_[ind];
  }
  std::array<std::array<double, 4>, 4> matrix_;
};

[[nodiscard]] Matrix MakeCameraToWorld(const geom::Vector& from, const geom::Vector& to) noexcept;

}  // namespace rt
