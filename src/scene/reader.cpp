#include <geometry/vector.hpp>
#include <scene/reader.hpp>

#include <filesystem>
#include <fstream>
#include <map>
#include <optional>
#include <sstream>
#include <string>

namespace rt {

namespace {

[[nodiscard]] std::size_t GetIndex(int ind, std::size_t size) noexcept {
  return ind > 0 ? ind - 1 : size + ind;
}

void ReadOne(std::istringstream& ss, int& ind, std::optional<int>& tx, std::optional<int>& n) noexcept {
  ss >> ind;
  if (ss.get() == '/') {
    if (ss.peek() != '/') {
      int tx_tmp;
      ss >> tx_tmp;
      tx = tx_tmp;
    }
    if (ss.get() == '/') {
      int n_tmp;
      ss >> n_tmp;
      n = n_tmp;
    }
  }
}

Object MakeObject(const int& ind_0, const std::optional<int>& n_0, const int& ind_1, const std::optional<int>& n_1,
                  const int& ind_2, const std::optional<int>& n_2, std::vector<geom::Vector>& vertices,
                  std::vector<geom::Vector>& normals, const Material* current_material) {
  geom::Triangle tr({vertices[GetIndex(ind_0, vertices.size())], vertices[GetIndex(ind_1, vertices.size())],
                     vertices[GetIndex(ind_2, vertices.size())]});
  std::array<std::optional<geom::Vector>, 3> norm;
  if (n_0.has_value()) {
    norm[0] = normals[GetIndex(n_0.value(), normals.size())];
    norm[1] = normals[GetIndex(n_1.value(), normals.size())];
    norm[2] = normals[GetIndex(n_2.value(), normals.size())];
  }
  return Object{norm, current_material, tr};
}

std::map<std::string, Material> ReadMaterials(std::string_view filename) {
  std::fstream file(std::string(filename), std::ios::in);
  std::string str;

  std::map<std::string, Material> materials;
  std::string name;
  geom::Vector ambient_color{0, 0, 0};
  geom::Vector diffuse_color{0, 0, 0};
  geom::Vector specular_color{0, 0, 0};
  geom::Vector intensity{0, 0, 0};
  double specular_exponent = 0;
  double refraction_index = 1;
  std::array<double, 3> albedo{1, 0, 0};
  bool flag = false;
  while (std::getline(file, str)) {
    std::istringstream ss(str);
    std::string w;
    ss >> w;
    if (w == "newmtl") {
      if (flag) {
        materials[name] = {name,      ambient_color,     diffuse_color,    specular_color,
                           intensity, specular_exponent, refraction_index, albedo};
        ambient_color = {0, 0, 0};
        diffuse_color = {0, 0, 0};
        specular_color = {0, 0, 0};
        intensity = {0, 0, 0};
        specular_exponent = 0;
        refraction_index = 1;
        albedo = {1, 0, 0};
      }
      ss >> name;
      flag = true;
    } else if (w == "Ka") {
      double x, y, z;
      ss >> x >> y >> z;
      ambient_color = {x, y, z};
    } else if (w == "Kd") {
      double x, y, z;
      ss >> x >> y >> z;
      diffuse_color = {x, y, z};
    } else if (w == "Ks") {
      double x, y, z;
      ss >> x >> y >> z;
      specular_color = {x, y, z};
    } else if (w == "Ke") {
      double x, y, z;
      ss >> x >> y >> z;
      intensity = {x, y, z};
    } else if (w == "Ns") {
      ss >> specular_exponent;
    } else if (w == "Ni") {
      ss >> refraction_index;
    } else if (w == "al") {
      double x, y, z;
      ss >> x >> y >> z;
      albedo = {x, y, z};
    }
  }
  materials[name] = {name,      ambient_color,     diffuse_color,    specular_color,
                     intensity, specular_exponent, refraction_index, albedo};
  return materials;
}

}  // namespace

Scene ReadScene(std::string_view filename) {
  std::vector<Object> objects;
  std::vector<SphereObject> sphere_objects;
  std::vector<Light> lights;
  std::map<std::string, Material> materials;

  const Material* current_material = nullptr;
  std::string mtl_filename;
  std::vector<geom::Vector> vertices;
  std::vector<geom::Vector> normals;
  std::fstream file(std::string(filename), std::ios::in);
  if (!file.good()) {
    throw std::runtime_error("file is not open");
  }
  std::string str;
  while (std::getline(file, str)) {
    std::istringstream ss(str);
    std::string w;
    ss >> w;
    if (w == "v" || w == "vn") {
      double x, y, z;
      ss >> x >> y >> z;
      if (w == "v") {
        vertices.push_back(geom::Vector{x, y, z});
      } else {
        geom::Vector n{x, y, z};
        n.Normalize();
        normals.push_back(n);
      }
    } else if (w == "mtllib") {
      ss >> mtl_filename;
      std::filesystem::path p(filename);
      materials = ReadMaterials(p.parent_path().string() + "/" + mtl_filename);
    } else if (w == "usemtl") {
      std::string material;
      ss >> material;
      current_material = &materials[material];
    } else if (w == "f") {
      int ind_0;
      std::optional<int> tx_0, n_0;
      ReadOne(ss, ind_0, tx_0, n_0);
      int ind_1;
      std::optional<int> tx_1, n_1;
      ReadOne(ss, ind_1, tx_1, n_1);
      int ind_2;
      std::optional<int> tx_2, n_2;

      do {
        ReadOne(ss, ind_2, tx_2, n_2);
        if (ind_1 != ind_2) {
          objects.emplace_back(MakeObject(ind_0, n_0, ind_1, n_1, ind_2, n_2, vertices, normals, current_material));
        }
        ind_1 = ind_2;
        tx_1 = tx_2;
        n_1 = n_2;

      } while (!ss.eof());

    } else if (w == "P") {
      double x, y, z, r, g, b;
      ss >> x >> y >> z >> r >> g >> b;
      lights.emplace_back(geom::Vector{x, y, z}, geom::Vector{r, g, b});
    } else if (w == "S") {
      double x, y, z, r;
      ss >> x >> y >> z >> r;
      sphere_objects.emplace_back(current_material, geom::Vector{x, y, z}, r);
    }
  }
  return Scene{std::move(objects), std::move(sphere_objects), std::move(lights), std::move(materials)};
}

}  // namespace rt
