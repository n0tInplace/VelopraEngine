#include "VE_OpenGLMesh.h"
#include "VE_LoggerMacros.h"
#include "VE_pch.h"

namespace velopraEngine {
namespace render {

OpenGLMesh::OpenGLMesh(const std::vector<Vertex> &vertices,
                       const std::vector<GLuint> &indices)
    : vertices(vertices), indices(indices) {
  SetupMesh();
}

OpenGLMesh::OpenGLMesh(OpenGLMesh &&other) noexcept
    : VAO(other.VAO), VBO(other.VBO), EBO(other.EBO),
      vertices(std::move(other.vertices)), indices(std::move(other.indices)) {
  other.VAO = 0;
  other.VBO = 0;
  other.EBO = 0;
}

OpenGLMesh &OpenGLMesh::operator=(OpenGLMesh &&other) noexcept {
  if (this != &other) {
    CleanUp(); // Clean up existing resources

    // Transfer ownership
    VAO = other.VAO;
    VBO = other.VBO;
    EBO = other.EBO;
    vertices = std::move(other.vertices);
    indices = std::move(other.indices);

    // Reset other's resource handles
    other.VAO = 0;
    other.VBO = 0;
    other.EBO = 0;
  }
  return *this;
}

OpenGLMesh::~OpenGLMesh() {
  VELOPRA_CORE_INFO("Destroying OpenGLMesh with VAO: {}", VAO);
  CleanUp();
}

void OpenGLMesh::CleanUp() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  VELOPRA_CORE_INFO("Mesh resources cleaned up.");
}

void OpenGLMesh::SetupMesh() {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  // Load data into vertex buffer
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
               vertices.data(), GL_STATIC_DRAW);

  // Load data into element buffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
               indices.data(), GL_STATIC_DRAW);

  // Set the vertex attribute pointers
  // Vertex Positions
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);

  // Vertex Normals
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, normal));

  // Vertex Texture Coords
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, texCoords));

  glBindVertexArray(0); // Unbind VAO

  VELOPRA_CORE_INFO("Mesh setup complete.");
}


void OpenGLMesh::Draw() const {
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()),
                 GL_UNSIGNED_INT, 0);
  glBindVertexArray(0); // Unbinding is optional

  CheckOpenGLError("Mesh Draw");
}

void OpenGLMesh::CheckOpenGLError(const std::string &operation) {
  GLenum err;
  while ((err = glGetError()) != GL_NO_ERROR) {
    VELOPRA_CORE_CRITICAL("OpenGL error during {}: {}", operation, err);
  }
}

} // namespace render
} // namespace velopraEngine
