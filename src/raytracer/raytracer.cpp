
#include <geometry/geometry.hpp>
#include <geometry/ray.hpp>
#include <geometry/vector.hpp>
#include <raytracer/camera_options.hpp>
#include <raytracer/image.hpp>
#include <raytracer/matrix.hpp>
#include <raytracer/render_options.hpp>
#include <scene/reader.hpp>

#include <algorithm>
#include <string>

namespace rt {

namespace details {
template <typename T>
inline constexpr bool always_false_v = false;  // NOLINT

struct Value {
  geom::Vector value;
  bool intersect;
};

class Picture {
 public:
  Picture(int width, int height) : width_(width), height_(height) {
    picture_ = new Value*[height_];
    for (int y = 0; y < height_; y++) {
      picture_[y] = new Value[width_];
    }
  }
  [[nodiscard]] Value GetValue(int y, int x) const noexcept {
    return picture_[y][x];
  }
  void SetValue(const Value& value, int y, int x) noexcept {
    picture_[y][x] = value;
  }
  ~Picture() {
    for (int i = 0; i < height_; ++i) {
      delete[] picture_[i];
    }
    delete[] picture_;
  }

 private:
  int width_, height_;
  Value** picture_;
};

}  // namespace details

namespace {

template <typename T>
[[nodiscard]] auto GetFigure(const T& obj) noexcept {
  if constexpr (std::is_same_v<T, Object>) {
    return obj.polygon;
  } else if constexpr (std::is_same_v<T, SphereObject>) {
    return obj.sphere;
  } else {
    static_assert(details::always_false_v<T>);
  }
}

template <typename T>
[[nodiscard]] bool CheckCover(const std::vector<T>& objects, const geom::Ray& ray, double distance) noexcept {
  bool covered = false;
  for (const auto& obj_to_compare : objects) {
    auto intersection_to_compare = GetIntersection(ray, GetFigure(obj_to_compare));
    if (intersection_to_compare) {
      if (intersection_to_compare->GetDistance() < distance) {
        covered = true;
        break;
      }
    }
  }
  return covered;
}

template <typename T>
[[nodiscard]] geom::Vector GetNormal(const T& obj, const geom::Intersection& intersection) noexcept {
  if constexpr (std::is_same_v<T, Object>) {
    if (!obj.normals[0]) {
      return intersection.GetNormal();
    } else {
      geom::Vector coordinates = GetBarycentricCoords(obj.polygon, intersection.GetPosition());
      return coordinates[0] * obj.normals[0].value() + coordinates[1] * obj.normals[1].value() +
             coordinates[2] * obj.normals[2].value();
    }
  } else if constexpr (std::is_same_v<T, SphereObject>) {
    return intersection.GetNormal();
  } else {
    static_assert(details::always_false_v<T>);
  }
}

template <typename T>
std::optional<T> FindClosest(const std::vector<T>& objects, const geom::Ray& ray, double& min_distance, bool& intersect,
                             const RenderOptions& render_options, const Scene& scene, double* max_distance = nullptr) {
  std::optional<T> closest;
  for (const auto& obj : objects) {
    auto intersection = GetIntersection(ray, GetFigure(obj));
    if (intersection) {
      if (!intersect) {
        min_distance = intersection->GetDistance();
        intersect = true;
        closest = obj;
      } else {
        if (intersection->GetDistance() < min_distance) {
          min_distance = intersection->GetDistance();
          closest = obj;
        }
      }
      if (render_options.mode == RenderMode::kDepth && intersection->GetDistance() > *max_distance) {
        bool covered = CheckCover(scene.GetObjects(), ray, intersection->GetDistance());
        if (!covered) {
          covered = CheckCover(scene.GetSphereObjects(), ray, intersection->GetDistance());
        }
        if (!covered) {
          *max_distance = intersection->GetDistance();
        }
      }
    }
  }
  return closest;
}

template <typename T>
[[nodiscard]] bool LightVisible(const std::vector<T>& objects, const geom::Vector& point, const Light& light) noexcept {
  bool visible = true;
  geom::Vector direction = light.position - point;
  geom::Ray ray{point, direction};
  for (const auto& obj : objects) {
    auto intersection = GetIntersection(ray, GetFigure(obj));
    if (intersection.has_value()) {
      if (intersection->GetDistance() < Length(direction)) {
        visible = false;
        break;
      }
    }
  }
  return visible;
}

[[nodiscard]] geom::Vector Ld(const geom::Vector& point, const Light& light, const geom::Vector& n) noexcept {
  geom::Vector v_l = light.position - point;
  v_l.Normalize();
  return light.intensity * (DotProduct(n, v_l) > 0 ? DotProduct(n, v_l) : 0);
}

[[nodiscard]] geom::Vector Ls(const geom::Ray& ray, const geom::Vector& point, const Light& light,
                              const geom::Vector& n, double ns) noexcept {
  geom::Vector v_l = light.position - point;
  v_l.Normalize();
  geom::Vector v_r = v_l + 2 * (DotProduct(n, v_l) * n - v_l);
  v_r.Normalize();
  geom::Vector v_e = -ray.GetDirection();
  v_e.Normalize();
  return light.intensity * pow(((DotProduct(v_e, v_r) > 0 ? DotProduct(v_e, v_r) : 0)), ns);
}

template <typename T>
[[nodiscard]] geom::Vector ComputeFull(const Scene& scene, const geom::Ray& ray, const RenderOptions& render_options,
                                       const T& object, bool inside = false);

[[nodiscard]] geom::Vector TraceNewRay(double coeff, const std::optional<Object>& closest_object,
                                       const std::optional<SphereObject>& closest_sphere, const Scene& scene,
                                       const geom::Ray& new_ray, const RenderOptions& render_options,
                                       bool inside = false) {
  if (closest_sphere) {
    return coeff * ComputeFull(scene, new_ray, render_options, *closest_sphere, !inside);
  } else {
    return coeff * ComputeFull(scene, new_ray, render_options, *closest_object);
  }
}

template <typename T>
[[nodiscard]] geom::Vector ComputeFull(const Scene& scene, const geom::Ray& ray, const RenderOptions& render_options,
                                       const T& object, bool inside) {
  geom::Vector intensivity = object.material->ambient_color + object.material->intensity;
  auto figure = GetFigure(object);
  geom::Intersection intersection = GetIntersection(ray, figure).value();
  geom::Vector normal = GetNormal(object, intersection);
  if (DotProduct(normal, ray.GetDirection()) > 0) {
    normal = -normal;
  }
  for (const auto& light : scene.GetLights()) {
    geom::Vector point = intersection.GetPosition() + 1e-9 * normal;
    if (LightVisible(scene.GetObjects(), point, light) && LightVisible(scene.GetSphereObjects(), point, light)) {
      intensivity +=
        object.material->diffuse_color * Ld(intersection.GetPosition(), light, normal) * object.material->albedo[0];
      intensivity += object.material->specular_color *
                     Ls(ray, intersection.GetPosition(), light, normal, object.material->specular_exponent) *
                     object.material->albedo[0];
    }
  }
  if (fabs(object.material->albedo[1]) > 1e-9) {  // reflect
    if (render_options.depth > 0 && !inside) {
      geom::Vector point = intersection.GetPosition() + 1e-9 * normal;
      geom::Vector reflect_direction = Reflect(ray.GetDirection(), normal);
      reflect_direction.Normalize();
      geom::Ray reflect_ray{point, reflect_direction};
      double min_distance;
      bool intersect = false;
      std::optional<Object> closest_object =
        FindClosest(scene.GetObjects(), reflect_ray, min_distance, intersect, render_options, scene);
      std::optional<SphereObject> closest_sphere =
        FindClosest(scene.GetSphereObjects(), reflect_ray, min_distance, intersect, render_options, scene);

      if (intersect) {
        intensivity += TraceNewRay(object.material->albedo[1], closest_object, closest_sphere, scene, reflect_ray,
                                   {render_options.depth - 1, RenderMode::kFull});
      }
    }
  }
  if (fabs(object.material->albedo[2]) > 1e-9 && render_options.depth > 0) {  // refract
    double refraction_index = !inside ? 1 / object.material->refraction_index : object.material->refraction_index;
    std::optional<geom::Vector> refract_direction = Refract(ray.GetDirection(), normal, refraction_index);
    if (refract_direction.has_value()) {
      geom::Vector point = intersection.GetPosition() - 1e-9 * normal;
      refract_direction.value().Normalize();
      geom::Ray refract_ray{point, refract_direction.value()};
      double min_distance;
      bool intersect = false;
      std::optional<Object> closest_object =
        FindClosest(scene.GetObjects(), refract_ray, min_distance, intersect, render_options, scene);
      std::optional<SphereObject> closest_sphere =
        FindClosest(scene.GetSphereObjects(), refract_ray, min_distance, intersect, render_options, scene);

      if (intersect) {
        double coeff = !inside ? object.material->albedo[2] : 1;
        intensivity += TraceNewRay(coeff, closest_object, closest_sphere, scene, refract_ray,
                                   {render_options.depth - 1, RenderMode::kFull}, inside);
      }
    }
  }
  return intensivity;
}

[[nodiscard]] details::Value GetPixelValue(const Scene& scene, const geom::Ray& ray,
                                           const RenderOptions& render_options, double* max_distance, double* max_rgb) {
  double min_distance;
  bool intersect = false;
  std::optional<Object> closest_object =
    FindClosest(scene.GetObjects(), ray, min_distance, intersect, render_options, scene, max_distance);
  std::optional<SphereObject> closest_sphere =
    FindClosest(scene.GetSphereObjects(), ray, min_distance, intersect, render_options, scene, max_distance);

  if (intersect) {
    if (render_options.mode == RenderMode::kFull) {
      geom::Vector intensivity;
      if (closest_sphere) {
        intensivity = ComputeFull(scene, ray, render_options, *closest_sphere);
      } else {
        intensivity = ComputeFull(scene, ray, render_options, *closest_object);
      }
      double to_compare = std::max({intensivity[0], intensivity[1], intensivity[2]});
      *max_rgb = *max_rgb > to_compare ? *max_rgb : to_compare;
      return {intensivity, true};

    } else if (render_options.mode == RenderMode::kDepth) {
      double distance;
      if (closest_sphere) {
        distance = GetIntersection(ray, closest_sphere->sphere)->GetDistance();
      } else {
        distance = GetIntersection(ray, closest_object->polygon)->GetDistance();
      }

      return details::Value{geom::Vector{distance, distance, distance}, true};
    }
    geom::Vector normal;
    auto sphere_intersection = GetIntersection(ray, closest_sphere->sphere);
    auto object_intersection = GetIntersection(ray, closest_object->polygon);

    if (closest_sphere) {
      normal = GetNormal(*closest_sphere, *sphere_intersection);
    } else {
      normal = GetNormal(*closest_object, *object_intersection);
    }
    if (DotProduct(normal, ray.GetDirection()) > 0) {
      normal = -normal;
    }
    auto res = (1.0 / 2) * normal + geom::Vector{1.0 / 2, 1.0 / 2, 1.0 / 2};
    return details::Value{res, true};
  }

  if (render_options.mode == RenderMode::kDepth) {
    return details::Value{geom::Vector{1, 1, 1}, false};
  }
  return details::Value{geom::Vector{0, 0, 0}, false};
}

}  // namespace

[[nodiscard]] image::Image Render(const std::string& filename, const CameraOptions& camera_options,
                                  const RenderOptions& render_options) {
  Scene scene = ReadScene(filename);
  image::Image image(camera_options.screen_width, camera_options.screen_height);
  details::Picture picture(camera_options.screen_width, camera_options.screen_height);
  double image_aspect_ratio = static_cast<double>(camera_options.screen_width) / camera_options.screen_height;
  double scale = tan(camera_options.fov / 2);
  Matrix camera_to_world = MakeCameraToWorld(camera_options.look_from, camera_options.look_to);
  geom::Vector origin = camera_to_world.multiply_vector({0, 0, 0});
  double max_distance = 0;
  double max_rgb = 0;
  for (int y = 0; y < camera_options.screen_height; ++y) {
    for (int x = 0; x < camera_options.screen_width; ++x) {
      double px = (2 * ((x + 0.5) / camera_options.screen_width) - 1) * scale * image_aspect_ratio;
      double py = (1 - 2 * (y + 0.5) / camera_options.screen_height) * scale;
      geom::Vector P = camera_to_world.multiply_vector({px, py, -1});
      geom::Ray ray(camera_options.look_from, P - origin);
      picture.SetValue(GetPixelValue(scene, ray, render_options, &max_distance, &max_rgb), y, x);
    }
  }
  for (int y = 0; y < camera_options.screen_height; ++y) {
    for (int x = 0; x < camera_options.screen_width; ++x) {
      geom::Vector intense = picture.GetValue(y, x).value;
      if (render_options.mode == RenderMode::kFull && picture.GetValue(y, x).intersect) {
        auto out = intense * (geom::Vector{1, 1, 1} + intense / pow(max_rgb, 2)) / (geom::Vector{1, 1, 1} + intense);
        intense = {pow(out[0], 1 / 2.2), pow(out[1], 1 / 2.2), pow(out[2], 1 / 2.2)};
      } else if (render_options.mode == RenderMode::kDepth && picture.GetValue(y, x).intersect) {
        intense /= max_distance;
      }
      image::RGB value = {static_cast<int>(intense[0] * 255), static_cast<int>(intense[1] * 255),
                          static_cast<int>(intense[2] * 255)};
      image.SetPixel(value, y, x);
    }
  }
  return image;
}
}  // namespace rt
