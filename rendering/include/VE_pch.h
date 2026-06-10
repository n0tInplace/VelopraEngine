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
// ... any other standard library headers ...

// Internal headers
// Include any internal headers that are used across multiple files
#include "VE_Core.h"
#include "VE_LoggerMacros.h"
#include "VE_Math.h"

#endif // VE_RENDERING_PCH_H
