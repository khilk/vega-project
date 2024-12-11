#include <geometry/geometry.hpp>

namespace rt::geom {

std::optional<Intersection> GetIntersection(const Ray& ray, const Sphere& sphere) noexcept {
  Vector l = sphere.GetCenter() - ray.GetOrigin();
  double tc = DotProduct(ray.GetDirection(), l);
  if (tc < 0) {
    return {};
  }
  double d = sqrt(-tc * tc + Length(l) * Length(l));
  if (d > sphere.GetRadius()) {
    return {};
  }
  double tc1 = sqrt(pow(sphere.GetRadius(), 2) - pow(d, 2));
  double distance = tc >= tc1 ? (tc - tc1) : (tc + tc1);
  Vector point = ray.GetOrigin() + distance * ray.GetDirection();
  Vector normal = point - sphere.GetCenter();
  if (tc < tc1) {
    normal = -normal;
  }
  normal.Normalize();
  return Intersection{point, normal, distance};
}

std::optional<Intersection> GetIntersection(const Ray& ray, const Triangle& triangle) noexcept {
  const double epsilon = 0.0000001;
  Vector vertex0 = triangle.GetVertex(0);
  Vector vertex1 = triangle.GetVertex(1);
  Vector vertex2 = triangle.GetVertex(2);
  Vector edge1, edge2, h, s, q;
  double a, u, v;
  edge1 = vertex1 - vertex0;
  edge2 = vertex2 - vertex0;
  h = CrossProduct(ray.GetDirection(), edge2);
  a = DotProduct(h, edge1);
  if (a > -epsilon && a < epsilon) {
    return {};  // This ray is parallel to this triangle.
  }
  s = ray.GetOrigin() - vertex0;
  u = DotProduct(h, s) / a;
  if (u < 0.0 || u > 1.0) {
    return {};
  }
  q = CrossProduct(s, edge1);
  v = DotProduct(q, ray.GetDirection()) / a;
  if (v < 0.0 || u + v > 1.0) {
    return {};
  }
  // At this stage we can compute t to find out where the intersection point is on the line.
  double t = DotProduct(q, edge2) / a;
  if (t > epsilon)  // ray intersection
  {
    Vector point = ray.GetOrigin() + ray.GetDirection() * t;
    Vector n = CrossProduct(edge1, edge2);
    n.Normalize();
    double distance = DotProduct(edge2, q) / a;
    return Intersection{point, n, distance};
  } else {  // This means that there is a line intersection but not a ray intersection.
    return {};
  }
}

std::optional<Vector> Refract(const Vector& ray, const Vector& normal, double eta) noexcept {
  double cos1 = -DotProduct(normal, ray);
  if (sqrt(1 - pow(cos1, 2)) > 1 / eta) {
    return {};
  }
  double cos2 = sqrt(1 - pow(eta * sqrt(1 - cos1 * cos1), 2));
  Vector refracted = eta * ray + (eta * cos1 - cos2) * normal;
  assert(DotProduct(ray, normal) * DotProduct(refracted, normal) > 0);
  return refracted;
}

Vector Reflect(const Vector& ray, const Vector& normal) noexcept {
  double cos = -DotProduct(normal, ray);
  Vector reflected = ray + 2 * cos * normal;
  assert(DotProduct(ray, normal) * DotProduct(reflected, normal) < 0);
  return reflected;
}

Vector GetBarycentricCoords(const Triangle& triangle, const Vector& point) noexcept {
  double u = Triangle{triangle.GetVertex(2), triangle.GetVertex(0), point}.Area() / triangle.Area();
  double v = Triangle{triangle.GetVertex(0), triangle.GetVertex(1), point}.Area() / triangle.Area();
  double w = Triangle{triangle.GetVertex(1), triangle.GetVertex(2), point}.Area() / triangle.Area();
  return Vector{w, u, v};
}

}  // namespace rt::geom
