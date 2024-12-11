#pragma once

#include <array>
#include <cassert>
#include <cmath>
#include <initializer_list>
#include <iostream>

namespace rt::geom {

class Vector {
 public:
  Vector() = default;
  Vector(std::initializer_list<double> list) : data_{} {
    assert(list.size() <= data_.size());
    for (std::size_t i = 0; double item : list) {
      data_[i++] = item;
    }
  }

  Vector(std::array<double, 3> data) noexcept : data_{data} {
  }

  [[nodiscard]] double& operator[](size_t ind) noexcept {
    return data_[ind];
  }

  [[nodiscard]] double operator[](size_t ind) const noexcept {
    return data_[ind];
  }

  void Normalize() noexcept {
    double norm = 0;
    for (double item : data_) {
      norm += item * item;
    }
    norm = sqrt(norm);
    for (double& item : data_) {
      item /= norm;
    }
  }

  Vector& operator-=(const Vector& other) {
    for (std::size_t i = 0; i < 3; ++i) {
      data_[i] -= other[i];
    }
    return *this;
  }

  Vector& operator+=(const Vector& other) {
    for (std::size_t i = 0; i < 3; ++i) {
      data_[i] += other[i];
    }
    return *this;
  }

  Vector& operator*=(double val) {
    for (std::size_t i = 0; i < 3; ++i) {
      data_[i] *= val;
    }
    return *this;
  }

  Vector& operator*=(const Vector& other) {
    for (std::size_t i = 0; i < 3; ++i) {
      data_[i] *= other[i];
    }
    return *this;
  }

  Vector& operator/=(const Vector& other) {
    for (std::size_t i = 0; i < data_.size(); ++i) {
      data_[i] /= other[i];
    }
    return *this;
  }

  Vector& operator/=(double val) {
    for (auto& item : data_) {
      item /= val;
    }
    return *this;
  }

  bool operator==(const Vector& other) {
    return data_ == other.data_;
  }

  Vector operator-() const {
    return Vector{-data_[0], -data_[1], -data_[2]};
  }

 private:
  std::array<double, 3> data_;
};

double DotProduct(const Vector& lhs, const Vector& rhs) noexcept;
Vector CrossProduct(const Vector& a, const Vector& b) noexcept;
double Length(const Vector& vec) noexcept;

Vector operator+(Vector lhs, const Vector& rhs) noexcept;
Vector operator*(Vector lhs, const Vector& rhs) noexcept;
Vector operator/(Vector lhs, const Vector& rhs) noexcept;
Vector operator-(Vector lhs, const Vector& rhs) noexcept;
Vector operator*(Vector v, double val) noexcept;
Vector operator*(double val, Vector v) noexcept;
Vector operator/(Vector v, double val) noexcept;

}  // namespace rt::geom
