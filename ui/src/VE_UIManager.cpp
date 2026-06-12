#include "VE_UIManager.h"
#include "VE_RenderWidgetFactory.h"

namespace velopraEngine {
namespace ui {

UIManager::UIManager(int argc, char *argv[], RenderType renderType,
                     std::shared_ptr<WindowManager> windowManager)
    : app(argc, argv),
      mainWindow(nullptr, windowManager,
                                  RenderWidgetFactory::CreateRenderWidget(
                                      renderType, windowManager)) {
  mainWindow.show();
}

UIManager::~UIManager() = default;

int UIManager::Run() { return app.exec(); }

} // namespace ui
} // namespace velopraEngine