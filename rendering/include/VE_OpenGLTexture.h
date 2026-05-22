#ifndef VE_OPENGL_TEXTURE_H
#define VE_OPENGL_TEXTURE_H

#include "VE_ITexture.h"
#include <GL/glew.h>

namespace velopraEngine {
namespace render {

class OpenGLTexture : public ITexture {
public:
  OpenGLTexture(GLuint id) : textureId(id) {}
  virtual ~OpenGLTexture() {
    if (textureId != 0) {
      glDeleteTextures(1, &textureId);
    }
  }

  GLuint GetTextureId() const { return textureId; }

  void Bind(int slot) override {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, textureId);
  }

  void Unbind() override { glBindTexture(GL_TEXTURE_2D, 0); }

private:
  GLuint textureId;
};

} // namespace render
} // namespace velopraEngine

#endif // VE_OPENGL_TEXTURE_H
