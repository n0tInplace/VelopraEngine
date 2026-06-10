#ifndef VE_OPENGLMESH_H
#define VE_OPENGLMESH_H

#include "VE_IMesh.h"
#include "VE_MeshData.h"
#include <GL/glew.h>
#include <vector>

namespace velopraEngine {
namespace render {

class OpenGLMesh : public IMesh {
public:
  OpenGLMesh(const std::vector<Vertex> &vertices,
             const std::vector<GLuint> &indices);
  ~OpenGLMesh() override;

  OpenGLMesh(OpenGLMesh &&other) noexcept;
  OpenGLMesh &operator=(OpenGLMesh &&other) noexcept;

  OpenGLMesh(const OpenGLMesh &) = delete;
  OpenGLMesh &operator=(const OpenGLMesh &) = delete;

  void Draw() const override;

private:
  std::vector<Vertex> vertices;
  std::vector<GLuint> indices;

  GLuint VAO, VBO, EBO;

  void SetupMesh();
  void CleanUp();
  static void CheckOpenGLError(const std::string &operation);
};

} // namespace render
} // namespace velopraEngine

#endif // VE_OPENGLMESH_H
