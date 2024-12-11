#include <raytracer/camera_options.hpp>
#include <raytracer/raytracer.hpp>
#include <raytracer/render_options.hpp>
#include <utils/diff.hpp>

#include <cmath>
#include <string>

#include <gtest/gtest.h>

void CheckImage(const std::string& obj_filename, const std::string& result_filename,
                const CameraOptions& camera_options, const RenderOptions& render_options,
                std::optional<std::string> output_filename = std::nullopt) {
  auto image = rt::Render("../../test/models/" + obj_filename, camera_options, render_options);
  if (output_filename.has_value()) {
    image.Write(output_filename.value());
  }

  rt::image::Image ok_image("../../test/models/" + result_filename);
  Compare(image, ok_image);
}

TEST(ShadingParts, Raytracer) {
  CameraOptions camera_opts(640, 480);
  RenderOptions render_opts{1, RenderMode::kDepth};
  CheckImage("shading_parts_d/scene.obj", "shading_parts_d/depth.png", camera_opts, render_opts);
  render_opts.mode = RenderMode::kNormal;
  CheckImage("shading_parts_d/scene.obj", "shading_parts_d/normal.png", camera_opts, render_opts);
}

TEST(Triangle, Raytracer) {
  CameraOptions camera_opts(640, 480);
  camera_opts.look_from = std::array<double, 3>{0.0, 2.0, 0.0};
  camera_opts.look_to = std::array<double, 3>{0.0, 0.0, 0.0};
  RenderOptions render_opts{1, RenderMode::kDepth};
  CheckImage("triangle_d/scene.obj", "triangle_d/depth.png", camera_opts, render_opts);
  render_opts.mode = RenderMode::kNormal;
  CheckImage("triangle_d/scene.obj", "triangle_d/normal.png", camera_opts, render_opts);
}

TEST(Triangle2, Raytracer) {
  CameraOptions camera_opts(640, 480);
  camera_opts.look_from = std::array<double, 3>{0.0, -2.0, 0.0};
  camera_opts.look_to = std::array<double, 3>{0.0, 0.0, 0.0};
  RenderOptions render_opts{1, RenderMode::kDepth};
  CheckImage("triangle_d/scene.obj", "triangle_d/depth2.png", camera_opts, render_opts);
  render_opts.mode = RenderMode::kNormal;
  CheckImage("triangle_d/scene.obj", "triangle_d/normal2.png", camera_opts, render_opts);
}

TEST(ClassicBox, Raytracer) {
  CameraOptions camera_opts(500, 500);
  camera_opts.look_from = std::array<double, 3>{-0.5, 1.5, 0.98};
  camera_opts.look_to = std::array<double, 3>{0.0, 1.0, 0.0};
  RenderOptions render_opts{4, RenderMode::kDepth};
  CheckImage("classic_box_d/CornellBox-Original.obj", "classic_box_d/depth1.png", camera_opts, render_opts);
  render_opts.mode = RenderMode::kNormal;
  CheckImage("classic_box_d/CornellBox-Original.obj", "classic_box_d/normal1.png", camera_opts, render_opts);
  camera_opts.look_from = std::array<double, 3>{-0.9, 1.9, -1};
  camera_opts.look_to = std::array<double, 3>{0.0, 0.0, 0};
  render_opts.mode = RenderMode::kDepth;
  CheckImage("classic_box_d/CornellBox-Original.obj", "classic_box_d/depth2.png", camera_opts, render_opts);
  render_opts.mode = RenderMode::kNormal;
  CheckImage("classic_box_d/CornellBox-Original.obj", "classic_box_d/normal2.png", camera_opts, render_opts);
}

TEST(BoxWithSpheres, Raytracer) {
  CameraOptions camera_opts(640, 480, M_PI / 3);
  camera_opts.look_from = std::array<double, 3>{0.0, 0.7, 1.75};
  camera_opts.look_to = std::array<double, 3>{0.0, 0.7, 0.0};
  RenderOptions render_opts{4, RenderMode::kDepth};
  CheckImage("box_d/cube.obj", "box_d/depth.png", camera_opts, render_opts);
  render_opts.mode = RenderMode::kNormal;
  CheckImage("box_d/cube.obj", "box_d/normal.png", camera_opts, render_opts);
}

TEST(Deer, Raytracer) {
  CameraOptions camera_opts(500, 500);
  camera_opts.look_from = std::array<double, 3>{100, 200, 150};
  camera_opts.look_to = std::array<double, 3>{0.0, 100.0, 0.0};
  RenderOptions render_opts{1, RenderMode::kDepth};
  CheckImage("deer_d/CERF_Free.obj", "deer_d/depth.png", camera_opts, render_opts);
  render_opts.mode = RenderMode::kNormal;
  CheckImage("deer_d/CERF_Free.obj", "deer_d/normal.png", camera_opts, render_opts);
}
