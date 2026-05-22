#ifndef VE_OPENGLMODEL_H
#define VE_OPENGLMODEL_H

#include "VE_IModel.h"
#include "VE_ITextureLoader.h"
#include "VE_ITransform.h"
#include "VE_MeshData.h"
#include "VE_OpenGLMesh.h"
#include "VE_OpenGLTransform.h"
#include <memory>
#include <vector>

namespace velopraEngine {
namespace render {

class OpenGLModel : public IModel {
public:
  OpenGLModel(const std::vector<MeshData> &meshData,
              ITextureLoader &textureLoader);
  ~OpenGLModel() override = default;

  void Draw() const override;
  void SetTransform(const ITransform &transform) override;
  ITransform &GetTransform() override;

private:
  std::vector<std::unique_ptr<OpenGLMesh>> meshes;
  std::unique_ptr<ITransform> transform;
};

} // namespace render
} // namespace velopraEngine

#endif // VE_OPENGLMODEL_H
