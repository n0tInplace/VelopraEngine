#ifndef VE_OPENGL_SHADER_H
#define VE_OPENGL_SHADER_H

#include "VE_IShader.h"
#include <GL/glew.h>
#include <unordered_map>

namespace velopraEngine {
namespace render {

class OpenGLShader : public IShader {
public:
  OpenGLShader(const std::string &vertexPath, const std::string &fragmentPath);
  virtual ~OpenGLShader();

  void Bind() const override;
  void Unbind() const override;

  void SetUniform1i(const std::string &name, int value) override;
  void SetUniform1f(const std::string &name, float value) override;
  void SetUniform4f(const std::string &name, float v0, float v1, float v2,
                    float v3) override;
  void SetUniformMat4f(const std::string &name,
                       const core::Matrix4 &matrix) override;

  bool ValidateProgram() override;
  bool IsValid() const override;

private:
  GLuint programID;
  bool valid;
  mutable std::unordered_map<std::string, GLint> uniformLocationCache;

  GLuint CompileShader(unsigned int type, const std::string &source);
  int GetUniformLocation(const std::string &name);
  std::string ReadFile(const std::string &filepath);
};

} // namespace render
} // namespace velopraEngine

#endif // VE_OPENGL_SHADER_H
