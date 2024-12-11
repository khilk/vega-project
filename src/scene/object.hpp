#pragma once

#include <geometry/sphere.hpp>
#include <geometry/triangle.hpp>
#include <scene/material.hpp>

#include <optional>
#include <utility>

namespace rt {

struct Object {
  [[nodiscard]] const geom::Vector* GetNormal(size_t index) const noexcept {
    return &*normals[index];
  }

  std::array<std::optional<geom::Vector>, 3> normals;
  const Material* material = nullptr;
  geom::Triangle polygon;
};

struct SphereObject {
  SphereObject(const Material* m, geom::Vector c, double r) noexcept : material(m), sphere(std::move(c), r) {
  }
  const Material* material = nullptr;
  geom::Sphere sphere;
};

}  // namespace rt
