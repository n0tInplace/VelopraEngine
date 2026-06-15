#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include "VE_OpenGLRenderer.h"
#include "VE_MeshData.h"
#include "VE_OpenGLCamera.h"
#include "VE_OpenGLModel.h"
#include "VE_OpenGLTexture.h"
#include "VE_OpenGlShader.h"
#include "VE_LoggerMacros.h"
#include "VE_pch.h"
#include "stb_image.h"

namespace {

// Loads geometry from a file using Assimp and returns backend-agnostic MeshData.
// All Assimp knowledge is contained here; callers only see MeshData.
class ModelLoader {
public:
  std::vector<velopraEngine::render::MeshData> Load(const std::string &path) {
    Assimp::Importer importer;
    const aiScene *scene =
        importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs |
                                    aiProcess_GenNormals |
                                    aiProcess_JoinIdenticalVertices);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !scene->mRootNode) {
      VELOPRA_CORE_ERROR("Assimp error: {}", importer.GetErrorString());
      return {};
    }
    directory = path.substr(0, path.find_last_of('/'));
    std::vector<velopraEngine::render::MeshData> result;
    ProcessNode(scene->mRootNode, scene, result);
    return result;
  }

private:
  std::string directory;

  void ProcessNode(aiNode *node, const aiScene *scene,
                   std::vector<velopraEngine::render::MeshData> &out) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
      out.push_back(ProcessMesh(scene->mMeshes[node->mMeshes[i]], scene));
    for (unsigned int i = 0; i < node->mNumChildren; i++)
      ProcessNode(node->mChildren[i], scene, out);
  }

  velopraEngine::render::MeshData ProcessMesh(aiMesh *mesh,
                                              const aiScene *scene) {
    velopraEngine::render::MeshData data;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
      velopraEngine::render::Vertex v;
      v.position = {mesh->mVertices[i].x, mesh->mVertices[i].y,
                    mesh->mVertices[i].z};
      if (mesh->HasNormals())
        v.normal = {mesh->mNormals[i].x, mesh->mNormals[i].y,
                    mesh->mNormals[i].z};
      v.texCoords = mesh->mTextureCoords[0]
                        ? glm::vec2(mesh->mTextureCoords[0][i].x,
                                    mesh->mTextureCoords[0][i].y)
                        : glm::vec2(0.0f);
      data.vertices.push_back(v);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
      const aiFace &face = mesh->mFaces[i];
      for (unsigned int j = 0; j < face.mNumIndices; j++)
        data.indices.push_back(face.mIndices[j]);
    }

    if (mesh->mMaterialIndex >= 0) {
      aiMaterial *mat = scene->mMaterials[mesh->mMaterialIndex];
      CollectTexturePaths(mat, aiTextureType_DIFFUSE, data.diffuseTexturePaths);
      CollectTexturePaths(mat, aiTextureType_SPECULAR,
                          data.specularTexturePaths);
    }

    return data;
  }

  void CollectTexturePaths(aiMaterial *mat, aiTextureType type,
                           std::vector<std::string> &out) {
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
      aiString str;
      mat->GetTexture(type, i, &str);
      out.push_back(directory + '/' + str.C_Str());
    }
  }
};

} // anonymous namespace

namespace velopraEngine {
namespace render {

OpenGLRenderer::OpenGLRenderer()
    : shader(nullptr), camera(nullptr), aspectRatio(1.0f),
      projectionMatrix(1.0f) {}

bool OpenGLRenderer::Initialize(const SceneDescription &scene) {
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    VELOPRA_CORE_ERROR("Failed to initialize GLEW");
    return false;
  }

  camera = std::make_unique<OpenGLCamera>(scene.cameraPosition);

  ModelLoader loader;
  for (const auto &obj : scene.objects) {
    auto meshData = loader.Load(obj.modelPath);
    if (meshData.empty()) {
      VELOPRA_CORE_ERROR("No geometry loaded from model: {}", obj.modelPath);
      continue;
    }
    auto m = std::make_unique<OpenGLModel>(meshData, *this);
    m->GetTransform().SetPosition(obj.position);
    models.push_back(std::move(m));
  }
  if (models.empty()) {
    VELOPRA_CORE_ERROR("No models loaded; check scene objects list.");
    return false;
  }

  shader = std::make_unique<OpenGLShader>(scene.vertexShaderPath,
                                          scene.fragmentShaderPath);
  if (!shader->IsValid()) {
    VELOPRA_CORE_ERROR("Shader setup failed: vertex: {}, fragment: {}",
                       scene.vertexShaderPath, scene.fragmentShaderPath);
    return false;
  }

  UpdateProjectionMatrix(scene.initialWidth, scene.initialHeight);
  VELOPRA_CORE_INFO("OpenGL Renderer initialized successfully.");
  return true;
}

void OpenGLRenderer::BeginFrame() {
  // Set the clear color and depth
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderer::RenderFrame() {
  glEnable(GL_DEPTH_TEST);

  shader->Bind();
  shader->SetUniformMat4f("u_View", camera->GetViewMatrix());
  shader->SetUniformMat4f("u_Projection", projectionMatrix);

  shader->SetUniform3f("u_LightPos",    5.0f, 5.0f, 5.0f);
  shader->SetUniform3f("u_LightColor",  1.0f, 1.0f, 1.0f);
  shader->SetUniform3f("u_ObjectColor", 1.0f, 0.5f, 0.2f);
  auto camPos = camera->GetPosition();
  shader->SetUniform3f("u_ViewPos", camPos.x, camPos.y, camPos.z);

  for (const auto &m : models) {
    shader->SetUniformMat4f("u_Model", m->GetTransform().GetModelMatrix());
    m->Draw();
  }
  shader->Unbind();
}

std::shared_ptr<ITexture>
OpenGLRenderer::LoadTexture(const std::string &filePath) {
  VELOPRA_CORE_INFO("Loading texture: {}", filePath);

  auto it = textureCache.find(filePath);
  if (it != textureCache.end()) {
    VELOPRA_CORE_TRACE("Texture loaded from cache");
    return it->second;
  }

  int width, height, nrChannels;
  unsigned char *data =
      stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
  if (!data) {
    VELOPRA_CORE_ERROR("Failed to load texture: {}", filePath);
    return nullptr;
  }

  GLuint textureID;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  GLenum format = (nrChannels == 3) ? GL_RGB : GL_RGBA;
  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
               GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(data);

  auto texture = std::make_shared<OpenGLTexture>(textureID);
  textureCache[filePath] = texture;

  VELOPRA_CORE_INFO("Texture loaded successfully: {}", filePath);
  return texture;
}

void OpenGLRenderer::OnWindowSizeChanged(int width, int height) {
  UpdateProjectionMatrix(width, height);
}

void OpenGLRenderer::OnCameraMove(Camera_Movement dir, float deltaTime) {
  if (camera) camera->ProcessKeyboard(dir, deltaTime);
}

void OpenGLRenderer::OnCameraRotate(float xOffset, float yOffset) {
  if (camera) camera->ProcessMouseMovement(xOffset, yOffset);
}

void OpenGLRenderer::UpdateProjectionMatrix(int width, int height) {
  aspectRatio = static_cast<float>(width) / static_cast<float>(height);
  // Projection conventions are backend-specific (GL clip depth is [-1,1]),
  // so each renderer builds its own projection behind its seam.
  projectionMatrix =
      glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 1000.0f);
  glViewport(0, 0, width, height);
}

} // namespace render
} // namespace velopraEngine

#endif