<h1 align="center">Velopra Engine </h1>
<p align="center">
  <a href="https://opensource.org/licenses/Apache-2.0">
    <img src="https://img.shields.io/badge/License-Apache_2.0-blue.svg" alt="License">
  </a>
</p>

<p align="center">
  <img src="https://github.com/AlexHikari/VelopraEngine/assets/15806836/45cb56e1-a686-4917-90ca-aaf4185f6b4d" alt="VelopraLogo">
</p>

The Velopra Engine is a versatile game engine designed for developing both 2D and 3D games. This project is primarily built for research purposes and as a learning platform for game engine architecture and advanced C++ techniques.

## Core Features

- **GLM-based Math Foundation**: Engine math (`core::Vector2/Vector3/Matrix4/Quaternion`) is built on [GLM](https://github.com/g-truc/glm) — one shared, backend-neutral math currency across all modules.
- **Unified Event System**: A single `EventDispatcher` with queued per-frame dispatch (`Post` + `ProcessEvents`), synchronous dispatch (`PostImmediate`), and both pointer-based (`EventSubscriber`) and RAII token-based callback subscriptions.
- **Integrated Logging System**: Streamlined error reporting and debugging via spdlog, with engine-wide logging macros.
- **Rendering Module**: OpenGL renderer behind narrow backend seams (`IRenderer`, `ITextureLoader`, `IModel`, `IMesh`, `IShader`, `ICamera`, `ITransform`), designed for future Vulkan, Metal, and DirectX backends.
- **Qt for UI Development**: Qt-based main window and render surface, with the renderer injected behind `IRenderer` so the UI never depends on a concrete backend.
- **Platform Agnostic Design**: Cross-platform CMake presets and vcpkg manifest-mode dependencies for Windows, macOS, and Linux.

## Core Components

### Core Module (VelopraCore)
- Central part of the engine, handling core functionalities.
- Singleton `Core` class exposing the `EventDispatcher` and the `LayerStack`.
- Layers (`Layer`/`LayerStack`) are per-frame update units ticked by the main loop after events are dispatched.
- `VE_Logger` for logging, utilizing spdlog.
- `Time` class for delta time calculations and frame time management.

### Rendering Module (VelopraRenderer)
- OpenGL renderer implementing `IRenderer` and `ITextureLoader`.
- Scene assets (model, shaders, camera placement) are described by a `SceneDescription` passed to `IRenderer::Initialize`, which reports failure instead of rendering in an undefined state.
- Model loading via Assimp is fully contained in a `ModelLoader` that produces backend-agnostic `MeshData`/`Vertex` geometry.
- External libraries: GLEW, Assimp, GLM, stb_image.

### Input System Module (VelopraInputSystem)
- `InputEventGenerator` subscribes to the `EventDispatcher` and tracks key/mouse state; query with `IsKeyPressed`/`IsMouseButtonPressed`/`GetMousePosition`.
- Translates Qt key/mouse codes to engine `KeyCode`/`MouseCode` values.
- `InputUpdateLayer` plugs input updates into the engine's `LayerStack`.

### UI Module (VelopraUI)
- Qt main window with a render surface behind the `IRenderWidget` seam.
- `RenderWidgetFactory` is the single place that knows concrete backends: it creates the renderer for the requested `RenderType` and injects it.
- `WindowManager` converts Qt input events to engine events and posts them to the `EventDispatcher`.

### Application Module
- The executable: initializes Core, pushes the input layer, and starts the Qt-driven main loop (events → layer updates → render, ~60 FPS).

## Event Handling
- One `EventDispatcher` for the whole engine: events are queued with `Post` and dispatched once per frame by `ProcessEvents`; `PostImmediate` is available for time-sensitive synchronous dispatch.
- Subscribe long-lived systems via the `EventSubscriber` interface, or ad-hoc listeners via callbacks with auto-unsubscribing RAII tokens.

## Logging System
- Utilizes spdlog for logging, integrated into the Core module.
- Offers various logging levels (trace, info, warn, error, critical).
- Macros for easy logging throughout the engine.

## Architecture Notes
- `CONTEXT.md` records the engine's domain vocabulary and load-bearing decisions (multi-backend constraint, GLM math foundation, projection-behind-the-seam rule).

## Asset Management Module (Planned)
- Future plans to include an asset management module for handling game assets (textures, models, audio files, scripts).
- Will manage loading, caching, and unloading of resources.

## Future Enhancements
- Integration of **physics**, **audio**, and advanced input handling.
- Expansion of the rendering module for more graphics features.
- Editor tooling built on the Qt UI module.
- **Support for Multiple Rendering Backends**: Vulkan, Metal, and DirectX renderers behind the existing `IRenderer` seam.

## Development Environment
- Developed with C++ (C++20 standard).
- CMake with cross-platform presets (`CMakePresets.json`) for Windows, macOS, and Linux.
- Dependencies managed by vcpkg in manifest mode (`vcpkg.json`).

## Getting Started

### Prerequisites

Before starting, ensure you have:
- A C++20 compliant compiler (e.g., GCC, Clang, MSVC).
- [CMake](https://cmake.org/download/) for managing the build process.

### Setting Up Dependencies

Dependencies (`spdlog`, `GLEW`, `GLM`, `Assimp`, `Qt5`) are declared in `vcpkg.json` and installed automatically by vcpkg manifest mode the first time you configure with CMake — the setup script only bootstraps vcpkg itself.

1. Clone the Velopra Engine repository:
   ```bash
   git clone https://github.com/AlexHikari/VelopraEngine.git
   cd VelopraEngine
   ```
2. Run the setup script:
   - On Windows:
   ```bash
   setup.bat
   ```
   - On macOS/Linux:
   ```bash
   sh setup.sh
   ```

### Building the Project

Configure and build with the preset for your platform:

| Platform | Configure | Build |
|---|---|---|
| Windows | `cmake --preset x64-debug` | `cmake --build --preset x64-debug` |
| macOS | `cmake --preset macos-debug` | `cmake --build --preset macos-debug` |
| Linux | `cmake --preset linux-debug` | `cmake --build --preset linux-debug` |

Each platform also has a `-release` variant. The first configure compiles Qt and Assimp from source via vcpkg and can take a while; later configures reuse vcpkg's binary cache.

**Note for macOS:** make sure a compiler toolchain, CMake, and pkg-config are available: `xcode-select --install` and `brew install cmake pkg-config`. (`setup.sh` checks for these and tells you what's missing.)

**Note for Linux:** building Qt through vcpkg requires X11 development headers. On Debian/Ubuntu: `sudo apt install build-essential cmake libgl1-mesa-dev libxi-dev libxext-dev libx11-dev libxrandr-dev libxrender-dev libxkbcommon-dev libxkbcommon-x11-dev libfontconfig1-dev libfreetype6-dev`.

**Note for Windows:** Visual Studio 2022 with the "Desktop development with C++" workload is required (the presets use its generator).

## Contributing

Contributions are currently closed but will open in the future as the engine evolves.

## Distribution

Pre-compiled binaries for each release will be available soon.
