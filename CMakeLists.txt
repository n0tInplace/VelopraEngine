# Top-Level CMakeLists.txt
cmake_minimum_required (VERSION 3.8)

set(MODULE_NAMES "core" "input" "rendering" "ui")
set(EXAMPLE_DIR "${CMAKE_SOURCE_DIR}/example")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Use the repo-local vcpkg (cloned by setup.sh / setup.bat) unless the caller
# already provided a toolchain. Dependencies are declared in vcpkg.json and
# installed automatically during configure (manifest mode).
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE
   AND EXISTS "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")
  set(CMAKE_TOOLCHAIN_FILE
      "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "Vcpkg toolchain file")
endif()

# Enable MSVC runtime library flags
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project("VelopraEngine")

# Option to control building of shared or static libraries
option(BUILD_SHARED_LIBS "Build shared libraries (DLLs)" ON)

# Include subdirectories for each module
add_subdirectory("core")
add_subdirectory("input")
add_subdirectory("rendering")
add_subdirectory("ui")
add_subdirectory("application")
