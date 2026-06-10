#ifndef VE_CORE_MATH_H
#define VE_CORE_MATH_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace velopraEngine {
namespace core {

// GLM is the engine's math foundation. These aliases keep engine vocabulary
// at the interfaces; all backends (OpenGL, Vulkan, Metal, DirectX) consume
// the same types. Projection matrices are the one backend-specific piece of
// math — each renderer builds its own behind its seam (see CONTEXT.md).
using Vector2 = glm::vec2;
using Vector3 = glm::vec3;
using Matrix4 = glm::mat4;
using Quaternion = glm::quat;

} // namespace core
} // namespace velopraEngine

#endif // VE_CORE_MATH_H
