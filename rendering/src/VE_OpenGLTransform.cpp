#include "VE_pch.h"
#include "VE_OpenGLTransform.h"

namespace velopraEngine {
namespace render {

OpenGLTransform::OpenGLTransform()
    : position(0.0f), rotation(glm::quat()), scale(1.0f) {}

core::Matrix4 OpenGLTransform::GetModelMatrix() const {
  glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
  model *= glm::toMat4(rotation);
  model = glm::scale(model, scale);
  return model;
}

void OpenGLTransform::SetPosition(const core::Vector3 &pos) { position = pos; }

void OpenGLTransform::SetRotation(const core::Quaternion &rot) {
  rotation = rot;
}

void OpenGLTransform::SetScale(const core::Vector3 &scale) {
  this->scale = scale;
}

core::Vector3 OpenGLTransform::GetPosition() const { return position; }

core::Quaternion OpenGLTransform::GetRotation() const { return rotation; }

core::Vector3 OpenGLTransform::GetScale() const { return scale; }

core::Vector3 OpenGLTransform::GetForwardDirection() const {
  return rotation * glm::vec3(0, 0, -1);
}

core::Vector3 OpenGLTransform::GetUpDirection() const {
  return rotation * glm::vec3(0, 1, 0);
}

core::Vector3 OpenGLTransform::GetRightDirection() const {
  return rotation * glm::vec3(1, 0, 0);
}

} // namespace render
} // namespace velopraEngine
