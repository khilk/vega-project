#pragma once

#include <scene/scene.hpp>

#include <string_view>

namespace rt {

Scene ReadScene(std::string_view filename);

}  // namespace rt
