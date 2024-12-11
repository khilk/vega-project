#pragma once

#include <geometry/intersection.hpp>
#include <geometry/ray.hpp>
#include <geometry/sphere.hpp>
#include <geometry/triangle.hpp>
#include <geometry/vector.hpp>

#include <optional>

namespace rt::geom {

[[nodiscard]] std::optional<Intersection> GetIntersection(const Ray& ray, const Sphere& sphere) noexcept;
[[nodiscard]] std::optional<Intersection> GetIntersection(const Ray& ray, const Triangle& triangle) noexcept;
[[nodiscard]] Vector GetBarycentricCoords(const Triangle& triangle, const Vector& point) noexcept;

[[nodiscard]] std::optional<Vector> Refract(const Vector& ray, const Vector& normal, double eta) noexcept;
[[nodiscard]] Vector Reflect(const Vector& ray, const Vector& normal) noexcept;

}  // namespace rt::geom
