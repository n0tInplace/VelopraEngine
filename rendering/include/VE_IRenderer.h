#ifndef VE_IRENDERER_H
#define VE_IRENDERER_H

#include "VE_RendererAPI.h"
#include "VE_SceneDescription.h"

namespace velopraEngine {
namespace render {

class VELOPRARENDERER_API IRenderer {
public:
  virtual ~IRenderer() = default;

  virtual void Initialize(const SceneDescription &scene) = 0;
  virtual void BeginFrame() = 0;
  virtual void RenderFrame() = 0;
  virtual void OnWindowSizeChanged(int width, int height) = 0;
};

} // namespace render
} // namespace velopraEngine

#endif // VE_IRENDERER_H