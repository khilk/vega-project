#include <geometry/vector.hpp>

namespace rt::geom {

double DotProduct(const Vector& lhs, const Vector& rhs) noexcept {
  double sum = 0;
  for (std::size_t i = 0; i < 3; ++i) {
    sum += rhs[i] * lhs[i];
  }
  return sum;
}

Vector CrossProduct(const Vector& a, const Vector& b) noexcept {
  double x = a[1] * b[2] - a[2] * b[1];
  double y = -(a[0] * b[2] - b[0] * a[2]);
  double z = a[0] * b[1] - a[1] * b[0];
  return Vector({x, y, z});
}

double Length(const Vector& vec) noexcept {
  double sum = 0;
  for (std::size_t i = 0; i < 3; ++i) {
    sum += vec[i] * vec[i];
  }
  return sqrt(sum);
}

Vector operator+(Vector lhs, const Vector& rhs) noexcept {
  return lhs += rhs;
}

Vector operator*(Vector lhs, const Vector& rhs) noexcept {
  return lhs *= rhs;
}

Vector operator/(Vector lhs, const Vector& rhs) noexcept {
  return lhs /= rhs;
}

Vector operator-(Vector lhs, const Vector& rhs) noexcept {
  return lhs -= rhs;
}

Vector operator*(Vector v, double val) noexcept {
  return v *= val;
}

Vector operator*(double val, Vector v) noexcept {
  return v *= val;
}

Vector operator/(Vector v, double val) noexcept {
  return v /= val;
}

}  // namespace rt::geom
