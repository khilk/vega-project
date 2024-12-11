#pragma once

#include <geometry/vector.hpp>

#include <cassert>
#include <cmath>
#include <initializer_list>

namespace rt::geom {

class Triangle {
 public:
  Triangle(std::initializer_list<Vector> list) {
    assert(list.size() <= 3);
    for (std::size_t i = 0; const Vector& item : list) {
      vertices_[i] = item;
      ++i;
    }
  }

  [[nodiscard]] double Area() const noexcept {
    double a = Length(vertices_[0] - vertices_[1]);
    double b = Length(vertices_[1] - vertices_[2]);
    double c = Length(vertices_[0] - vertices_[2]);
    double p = (a + b + c) / 2;
    return sqrt(p * (p - a) * (p - b) * (p - c));
  }

  [[nodiscard]] const Vector& GetVertex(std::size_t ind) const noexcept {
    return vertices_[ind];
  }

 private:
  std::array<Vector, 3> vertices_;
};

}  // namespace rt::geom
