#pragma once

#include <geometry/vector.hpp>

#include <array>
#include <string>

namespace rt {

struct Material {
  std::string name;
  geom::Vector ambient_color;   // Ka
  geom::Vector diffuse_color;   // Kd
  geom::Vector specular_color;  // Ks
  geom::Vector intensity;       // Ke
  double specular_exponent;     // Ns
  double refraction_index;      // Ni
  std::array<double, 3> albedo;
};

}  // namespace rt
