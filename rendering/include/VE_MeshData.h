#ifndef VE_MESH_DATA_H
#define VE_MESH_DATA_H

#include "VE_OpenGLMesh.h"
#include <string>
#include <vector>

namespace velopraEngine {
namespace render {

// Backend-agnostic geometry produced by ModelLoader and consumed by OpenGLModel.
// Uses GLMVertex for now; will align with engine math types when Candidate 5 lands.
struct MeshData {
  std::vector<GLMVertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<std::string> diffuseTexturePaths;
  std::vector<std::string> specularTexturePaths;
};

} // namespace render
} // namespace velopraEngine

#endif // VE_MESH_DATA_H
