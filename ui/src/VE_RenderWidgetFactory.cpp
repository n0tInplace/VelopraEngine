#include "VE_RenderWidgetFactory.h"
#include "VE_LoggerMacros.h"
#include "VE_OpenGLRenderer.h"
#include "VE_QtOpenGLWidget.h"
#include "VE_WindowManager.h"

namespace velopraEngine {
namespace ui {

// The factory is the single place that knows concrete backend types. The
// widget itself depends only on IRenderer.
std::unique_ptr<IRenderWidget>
RenderWidgetFactory::CreateRenderWidget(RenderType type,
                                        std::shared_ptr<WindowManager> wM) {
  switch (type) {
  case RenderType::OpenGL:
    return std::make_unique<QtOpenGLWidget>(
        std::make_shared<render::OpenGLRenderer>(), nullptr, wM);
  default:
    VELOPRA_CORE_WARN(
        "Requested render backend not implemented; falling back to OpenGL");
    return std::make_unique<QtOpenGLWidget>(
        std::make_shared<render::OpenGLRenderer>(), nullptr, wM);
  }
}

} // namespace ui
} // namespace velopraEngine
