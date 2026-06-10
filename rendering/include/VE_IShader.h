#ifndef VE_ISHADER_H
#define VE_ISHADER_H

#include "VE_Math.h"
#include <string>

namespace velopraEngine {
namespace render {

class IShader {
public:
  virtual ~IShader() = default;

  virtual void Bind() const = 0;
  virtual void Unbind() const = 0;

  virtual void SetUniform1i(const std::string &name, int value) = 0;
  virtual void SetUniform1f(const std::string &name, float value) = 0;
  virtual void SetUniform4f(const std::string &name, float v0, float v1,
                            float v2, float v3) = 0;
  virtual void SetUniformMat4f(const std::string &name,
                               const core::Matrix4 &matrix) = 0;

  virtual bool ValidateProgram() = 0;

  // False if shader sources failed to load, compile, or link.
  virtual bool IsValid() const = 0;

  // Add more methods as needed
};

} // namespace render
} // namespace velopraEngine

#endif // VE_ISHADER_H
