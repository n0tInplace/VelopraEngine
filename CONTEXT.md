# VelopraEngine — Domain Context

Vocabulary and load-bearing decisions for this codebase. Architecture reviews
and design discussions should use these terms and respect these constraints.

## Constraints

### Multi-backend rendering (decided 2026-06)
The engine will support multiple rendering backends: OpenGL (current), and in
the future Vulkan, Metal, and DirectX. Consequences:

- Backend-facing seams must be narrow interfaces (`IRenderer`, `ITextureLoader`,
  `IModel`, `IMesh`, `IShader`, `ICamera`, `ITransform`) that any backend can
  satisfy.
- Geometry and asset data crossing those seams must be backend-agnostic
  (`MeshData`, `Vertex`, `SceneDescription`).

### GLM is the engine's math foundation (decided 2026-06)
`core::Vector2/Vector3/Matrix4/Quaternion` are aliases of GLM types
(`VE_Math.h`). GLM is backend-neutral math — all backends consume the same
vectors, matrices, and quaternions. Do not reintroduce hand-rolled math types
or conversion layers.

**Exception — projection matrices are backend-specific.** Clip-space
conventions differ per backend (OpenGL: [-1,1] depth; Vulkan/Metal/DirectX:
[0,1], Vulkan flips Y). Each renderer builds its own projection matrix behind
its seam (e.g. `OpenGLRenderer::UpdateProjectionMatrix` uses
`glm::perspective`). Never put a projection builder in core.

## Vocabulary

- **Backend** — a concrete rendering implementation (OpenGL today; Vulkan,
  Metal, DirectX planned). One backend is active at a time, selected via
  `RenderType`.
- **Vertex** — the engine's one canonical vertex layout (position, normal,
  texCoords), defined in `VE_MeshData.h`. Every backend's mesh class consumes
  it.
- **MeshData** — backend-agnostic geometry (vertices, indices, texture paths)
  produced by the ModelLoader and consumed by the active backend's model.
- **ModelLoader** — the only code that knows about Assimp. Lives behind the
  rendering seam; produces `MeshData`.
- **SceneDescription** — plain struct describing what to render (asset paths,
  camera/model placement). The input to `IRenderer::Initialize`.
- **EventDispatcher** — the engine's single event system (queued `Post` +
  `ProcessEvents` per frame; `PostImmediate` for synchronous dispatch;
  pointer-based and token-based subscriptions). There is exactly one — do not
  add parallel event channels.
- **Layer / LayerStack** — per-frame update units ticked by the main loop
  after events are dispatched.
