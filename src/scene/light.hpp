#pragma once

#include <geometry/vector.hpp>

namespace rt {

struct Light {
  geom::Vector position;
  geom::Vector intensity;

  Light(geom::Vector position, geom::Vector intensity) noexcept
    : position(std::move(position)), intensity(std::move(intensity)) {
  }
};

}  // namespace rt
