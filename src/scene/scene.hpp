#pragma once

#include <scene/light.hpp>
#include <scene/material.hpp>
#include <scene/object.hpp>

#include <map>
#include <string>
#include <vector>

namespace rt {

class Scene {
 public:
  Scene(std::vector<Object> objects, std::vector<SphereObject> sphere_objects, std::vector<Light> lights,
        std::map<std::string, Material> materials)
    : objects_(std::move(objects)),
      sphere_objects_(std::move(sphere_objects)),
      lights_(std::move(lights)),
      materials_(std::move(materials)) {
  }

  const std::vector<Object>& GetObjects() const {
    return objects_;
  }

  const std::vector<SphereObject>& GetSphereObjects() const {
    return sphere_objects_;
  }

  const std::vector<Light>& GetLights() const {
    return lights_;
  }

  const std::map<std::string, Material>& GetMaterials() const {
    return materials_;
  }

 private:
  std::vector<Object> objects_;
  std::vector<SphereObject> sphere_objects_;
  std::vector<Light> lights_;
  const std::map<std::string, Material> materials_;
};

}  // namespace rt
