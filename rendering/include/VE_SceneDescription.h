#ifndef VE_SCENE_DESCRIPTION_H
#define VE_SCENE_DESCRIPTION_H

#include "VE_Math.h"
#include <string>

namespace velopraEngine {
namespace render {

struct SceneDescription {
  std::string modelPath          = "model.obj";
  std::string vertexShaderPath   = "vertex_shader.glsl";
  std::string fragmentShaderPath = "fragment_shader.glsl";
  core::Vector3 cameraPosition   = core::Vector3(0.0f, 0.0f, 3.0f);
  core::Vector3 modelPosition    = core::Vector3(0.0f, 0.0f, 0.0f);
  int initialWidth               = 800;
  int initialHeight              = 600;
};

} // namespace render
} // namespace velopraEngine

#endif // VE_SCENE_DESCRIPTION_H
