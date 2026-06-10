#include "VE_OpenGLShader.h"
#include "VE_LoggerMacros.h"
#include "VE_pch.h"

namespace velopraEngine {
namespace render {

OpenGLShader::OpenGLShader(const std::string &vertexPath,
                           const std::string &fragmentPath)
    : programID(0), valid(false) {
  std::string vertexSrc = ReadFile(vertexPath);
  std::string fragmentSrc = ReadFile(fragmentPath);
  if (vertexSrc.empty() || fragmentSrc.empty()) {
    VELOPRA_CORE_ERROR("Shader source missing: vertex: {}, fragment: {}",
                       vertexPath, fragmentPath);
    return;
  }

  GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSrc);
  GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSrc);
  if (vertexShader == 0 || fragmentShader == 0) {
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return;
  }

  programID = glCreateProgram();
  glAttachShader(programID, vertexShader);
  glAttachShader(programID, fragmentShader);
  glLinkProgram(programID);

  GLint isLinked = 0;
  glGetProgramiv(programID, GL_LINK_STATUS, &isLinked);
  glDetachShader(programID, vertexShader);
  glDetachShader(programID, fragmentShader);
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  if (isLinked == GL_FALSE) {
    GLint maxLength = 0;
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &maxLength);
    std::vector<GLchar> infoLog(maxLength);
    glGetProgramInfoLog(programID, maxLength, &maxLength, &infoLog[0]);
    VELOPRA_CORE_ERROR("Shader link failure: {}", infoLog.data());
    glDeleteProgram(programID);
    programID = 0;
    return;
  }

  valid = true;
  VELOPRA_CORE_INFO("Compiled and linked shader: vertex: {}, fragment: {}",
                    vertexPath, fragmentPath);
}

OpenGLShader::~OpenGLShader() { glDeleteProgram(programID); }

void OpenGLShader::Bind() const { glUseProgram(programID); }

void OpenGLShader::Unbind() const { glUseProgram(0); }

GLuint OpenGLShader::CompileShader(unsigned int type,
                                   const std::string &source) {
  GLuint shader = glCreateShader(type);
  const char *src = source.c_str();
  glShaderSource(shader, 1, &src, nullptr);
  glCompileShader(shader);

  // Check for compile errors
  GLint isCompiled = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
  if (isCompiled == GL_FALSE) {
    GLint maxLength = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
    std::vector<GLchar> infoLog(maxLength);
    glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);
    VELOPRA_CORE_ERROR("Shader compilation failure in {} shader: {}",
                       (type == GL_VERTEX_SHADER ? "vertex" : "fragment"),
                       infoLog.data());
    glDeleteShader(shader);
    return 0;
  }
  return shader;
}

int OpenGLShader::GetUniformLocation(const std::string &name) {
  return glGetUniformLocation(programID, name.c_str());
}

void OpenGLShader::SetUniform1i(const std::string &name, int value) {
  glUniform1i(GetUniformLocation(name), value);
}

void OpenGLShader::SetUniform1f(const std::string &name, float value) {
  glUniform1f(GetUniformLocation(name), value);
}

void OpenGLShader::SetUniform4f(const std::string &name, float v0, float v1,
                                float v2,
                          float v3) {
  glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

std::string OpenGLShader::ReadFile(const std::string &filepath) {
  std::ifstream fileStream(filepath, std::ios::in);
  if (!fileStream.is_open()) {
    VELOPRA_CORE_ERROR("Could not open file {}", filepath);
    return "";
  }
  std::stringstream sstr;
  sstr << fileStream.rdbuf();
  return sstr.str();
}

void OpenGLShader::SetUniformMat4f(const std::string &name,
                                   const core::Matrix4 &matrix) {
  glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

bool OpenGLShader::IsValid() const { return valid; }

bool OpenGLShader::ValidateProgram() {
  glValidateProgram(programID);
  GLint isValid = 0;
  glGetProgramiv(programID, GL_VALIDATE_STATUS, &isValid);
  if (isValid == GL_FALSE) {
    GLint maxLength = 0;
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &maxLength);
    std::vector<GLchar> infoLog(maxLength);
    glGetProgramInfoLog(programID, maxLength, &maxLength, &infoLog[0]);
    VELOPRA_CORE_ERROR("Shader validation failure: {}", infoLog.data());
    return false;
  }
  return true;
}

} // namespace render
} // namespace velopraEngine