#include <scene/reader.hpp>

#include <gtest/gtest.h>

namespace {

TEST(Scene, Raytracer) {
  const auto scene = rt::ReadScene("../../test/models/box/cube.obj");  // TODO(khilk): make config file
  const double eps = 1e-6;

  const auto& materials_map = scene.GetMaterials();
  EXPECT_EQ(materials_map.size(), 9);

  // objects
  const auto& objects = scene.GetObjects();
  EXPECT_EQ(objects.size(), 10);

  const rt::geom::Vector& vertex_coord_check = objects[0].polygon.GetVertex(0);
  EXPECT_LT(std::fabs(vertex_coord_check[0] - 1.), eps);
  EXPECT_LT(std::fabs(vertex_coord_check[1] - 0.), eps);
  EXPECT_LT(std::fabs(vertex_coord_check[2] - (-1.04)), eps);

  const rt::geom::Vector& normal_check = *objects[1].GetNormal(1);
  EXPECT_LT(std::fabs(normal_check[0] - 0.), eps);
  EXPECT_LT(std::fabs(normal_check[1] - 1.), eps);
  EXPECT_LT(std::fabs(normal_check[2] - 0.), eps);

  for (const auto& object : objects) {
    EXPECT_TRUE(materials_map.find(object.material->name) != materials_map.end());
  }

  // spheres
  const auto& spheres = scene.GetSphereObjects();
  EXPECT_EQ(spheres.size(), 2);

  const rt::geom::Vector& center = spheres[0].sphere.GetCenter();
  EXPECT_LT(std::fabs(center[0] - (-0.4)), eps);
  EXPECT_LT(std::fabs(center[1] - (0.3)), eps);
  EXPECT_LT(std::fabs(center[2] - (-0.4)), eps);
  EXPECT_LT(std::fabs(spheres[0].sphere.GetRadius() - 0.3), eps);
  for (const auto& sphere : spheres) {
    EXPECT_TRUE(materials_map.find(sphere.material->name) != materials_map.end());
  }

  // lights
  const auto& lights = scene.GetLights();
  EXPECT_EQ(lights.size(), 2);
  EXPECT_LT(std::fabs(lights[0].position[1] - 1.5899), eps);
  EXPECT_LT(std::fabs(lights[0].intensity[0] - 1.), eps);
  EXPECT_LT(std::fabs(lights[1].position[2] - 1.98), eps);
  EXPECT_LT(std::fabs(lights[1].intensity[1] - 0.5), eps);
  EXPECT_LT(std::fabs(lights[1].intensity[2] - 0.5), eps);

  // materials
  const rt::Material& right_sphere = materials_map.at("rightSphere");
  EXPECT_LT(std::fabs(right_sphere.albedo[0] - 0.), eps);
  EXPECT_LT(std::fabs(right_sphere.albedo[1] - 0.3), eps);
  EXPECT_LT(std::fabs(right_sphere.albedo[2] - 0.7), eps);
  EXPECT_LT(std::fabs(right_sphere.specular_exponent - 1024), eps);
  EXPECT_LT(std::fabs(right_sphere.refraction_index - 1.8), eps);

  const rt::Material& light = materials_map.at("light");
  EXPECT_LT(std::fabs(light.ambient_color[1] - 0.78), eps);
  EXPECT_LT(std::fabs(light.diffuse_color[2] - 0.78), eps);
  EXPECT_LT(std::fabs(light.specular_color[1] - 0.), eps);
  EXPECT_LT(std::fabs(light.intensity[2] - 1.), eps);

  const rt::geom::Vector& wall_behind_diffuse = materials_map.at("wallBehind").diffuse_color;
  EXPECT_LT(std::fabs(wall_behind_diffuse[0] - 0.2), eps);
  EXPECT_LT(std::fabs(wall_behind_diffuse[1] - 0.7), eps);
  EXPECT_LT(std::fabs(wall_behind_diffuse[2] - 0.8), eps);
}

}  // namespace
