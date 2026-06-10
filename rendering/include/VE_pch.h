// pch.h
#ifndef VE_RENDERING_PCH_H
#define VE_RENDERING_PCH_H

// External libraries
#include <GL/glew.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

// Standard Library headers
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

// The PCH carries external libraries only. Internal engine headers are
// included explicitly by the files that use them, so include lists state
// each file's real dependencies.

#endif // VE_RENDERING_PCH_H
