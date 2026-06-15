#ifndef VE_ICAMERA_H
#define VE_ICAMERA_H

#include "VE_Math.h"

namespace velopraEngine {
namespace render {

enum Camera_Movement { FORWARD, BACKWARD, LEFT, RIGHT };

class ICamera {
public:
  virtual ~ICamera() = default;

  virtual core::Matrix4 GetViewMatrix() = 0;
  virtual core::Vector3 GetPosition() const = 0;
  virtual void ProcessKeyboard(Camera_Movement direction, float deltaTime) = 0;
  virtual void ProcessMouseMovement(float xoffset, float yoffset,
                                    bool constrainPitch = true) = 0;
  virtual void ProcessMouseScroll(float yoffset) = 0;

  // Additional common camera functionalities can be added here
};

} // namespace render
} // namespace velopraEngine

#endif // VE_ICAMERA_H
