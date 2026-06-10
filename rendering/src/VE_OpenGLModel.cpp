#include "VE_OpenGLModel.h"
#include "VE_pch.h"

namespace velopraEngine {
namespace render {

OpenGLModel::OpenGLModel(const std::vector<MeshData> &meshData,
                         ITextureLoader &textureLoader)
    : transform(std::make_unique<OpenGLTransform>()) {
  for (const auto &data : meshData) {
    meshes.push_back(
        std::make_unique<OpenGLMesh>(data.vertices, data.indices));

    for (const auto &path : data.diffuseTexturePaths) {
      if (!textureLoader.LoadTexture(path))
        VELOPRA_CORE_WARN("Diffuse texture failed to load: {}", path);
    }
    for (const auto &path : data.specularTexturePaths) {
      if (!textureLoader.LoadTexture(path))
        VELOPRA_CORE_WARN("Specular texture failed to load: {}", path);
    }
  }
  VELOPRA_CORE_INFO("OpenGLModel created with {} meshes", meshes.size());
}

void OpenGLModel::Draw() const {
  for (const auto &mesh : meshes)
    mesh->Draw();
}

void OpenGLModel::SetTransform(const ITransform &transform) {
  *this->transform = transform;
}

ITransform &OpenGLModel::GetTransform() { return *transform; }

} // namespace render
} // namespace velopraEngine