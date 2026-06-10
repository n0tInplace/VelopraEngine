#ifndef VE_MESH_DATA_H
#define VE_MESH_DATA_H

#include "VE_Math.h"
#include <string>
#include <vector>

namespace velopraEngine {
namespace render {

// The engine's one canonical vertex layout. Every backend's mesh class
// consumes this; ModelLoader produces it.
struct Vertex {
  core::Vector3 position;
  core::Vector3 normal;
  core::Vector2 texCoords;
};

// Backend-agnostic geometry produced by ModelLoader and consumed by the
// active backend's model/mesh classes.
struct MeshData {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<std::string> diffuseTexturePaths;
  std::vector<std::string> specularTexturePaths;
};

} // namespace render
} // namespace velopraEngine

#endif // VE_MESH_DATA_H
