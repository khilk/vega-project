#pragma once

#include <raytracer/camera_options.hpp>
#include <raytracer/image.hpp>
#include <raytracer/render_options.hpp>

#include <string>

namespace rt {

image::Image Render(const std::string& filename, const CameraOptions& camera_options,
                    const RenderOptions& render_options);

}  // namespace rt
