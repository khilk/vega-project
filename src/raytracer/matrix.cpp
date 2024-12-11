#include <raytracer/matrix.hpp>

namespace rt {

[[nodiscard]] Matrix MakeCameraToWorld(const geom::Vector& from, const geom::Vector& to) noexcept {
  geom::Vector forward = from - to;
  forward.Normalize();
  geom::Vector right;
  if (Length(forward - geom::Vector{0, 1, 0}) < 0.00000001) {
    right = CrossProduct({0, 0, -1}, forward);
  } else if (Length(forward - geom::Vector{0, -1, 0}) < 0.00000001) {
    right = CrossProduct({0, 0, 1}, forward);
  } else {
    right = CrossProduct({0, 1, 0}, forward);
  }
  right.Normalize();
  geom::Vector up = CrossProduct(forward, right);
  Matrix camera_to_world;

  assert(fabs(Length(right) - 1) < 0.00000000001);
  assert(fabs(Length(up) - 1) < 0.00000000001);
  assert(fabs(Length(forward) - 1) < 0.00000000001);

  camera_to_world[0][0] = right[0];
  camera_to_world[0][1] = right[1];
  camera_to_world[0][2] = right[2];
  camera_to_world[1][0] = up[0];
  camera_to_world[1][1] = up[1];
  camera_to_world[1][2] = up[2];
  camera_to_world[2][0] = forward[0];
  camera_to_world[2][1] = forward[1];
  camera_to_world[2][2] = forward[2];

  camera_to_world[3][0] = from[0];
  camera_to_world[3][1] = from[1];
  camera_to_world[3][2] = from[2];
  camera_to_world[3][3] = 1;

  return camera_to_world;
}

[[nodiscard]] geom::Vector Matrix::multiply_vector(const geom::Vector& v) const noexcept {
  double x = v[0] * matrix_[0][0] + v[1] * matrix_[1][0] + v[2] * matrix_[2][0] + matrix_[3][0];
  double y = v[0] * matrix_[0][1] + v[1] * matrix_[1][1] + v[2] * matrix_[2][1] + matrix_[3][1];
  double z = v[0] * matrix_[0][2] + v[1] * matrix_[1][2] + v[2] * matrix_[2][2] + matrix_[3][2];
  double w = v[0] * matrix_[0][3] + v[1] * matrix_[1][3] + v[2] * matrix_[2][3] + matrix_[3][3];
  if (fabs(w - 1) > epsilon && fabs(w) > epsilon) {
    x /= w;
    y /= w;
    z /= w;
  }
  return {x, y, z};
}

}  // namespace rt
