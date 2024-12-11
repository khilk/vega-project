#pragma once

#include <iostream>

#include <jpeglib.h>
#include <png.h>

namespace rt::image {

struct RGB {
  int r, g, b;
  bool operator==(const RGB& rhs) const noexcept {
    return r == rhs.r && g == rhs.g && b == rhs.b;
  }
};

class Image {
 public:
  Image(int width, int height) {
    PrepareImage(width, height);
  }

  void PrepareImage(int width, int height);

  explicit Image(const std::string& filename) {
    if (filename.find(".png") != std::string::npos) {
      ReadPng(filename);
    } else {
      ReadJpg(filename);
    }
  }

  void ReadPng(const std::string& filename);
  void ReadJpg(const std::string& filename);
  void Write(const std::string& filename);

  [[nodiscard]] RGB GetPixel(int y, int x) const noexcept {
    auto row = bytes_[y];
    auto px = &row[x * 4];
    return RGB{px[0], px[1], px[2]};
  }

  void SetPixel(const RGB& pixel, int y, int x) noexcept {
    auto row = bytes_[y];
    auto px = &row[x * 4];
    px[0] = pixel.r;
    px[1] = pixel.g;
    px[2] = pixel.b;
  }

  [[nodiscard]] int Height() const noexcept {
    return height_;
  }

  [[nodiscard]] int Width() const noexcept {
    return width_;
  }

  ~Image() {
    for (int i = 0; i < height_; ++i) {
      free(bytes_[i]);
    }
    free(bytes_);
  }

 private:
  int width_, height_;
  png_bytep* bytes_;
};

}  // namespace rt::image
