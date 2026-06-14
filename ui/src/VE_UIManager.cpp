#include "VE_UIManager.h"
#include "VE_MainWindow.h"
#include "VE_RenderWidgetFactory.h"
#include <QApplication>
#include <QSurfaceFormat>

namespace velopraEngine {
namespace ui {

UIManager::UIManager(int argc, char *argv[], RenderType renderType,
                     std::shared_ptr<WindowManager> windowManager)
    : storedArgc(argc) {
  QSurfaceFormat fmt;
  fmt.setVersion(3, 3);
  fmt.setProfile(QSurfaceFormat::CoreProfile);
  QSurfaceFormat::setDefaultFormat(fmt);

  app = std::make_unique<QApplication>(storedArgc, argv);
  mainWindow = std::make_unique<MainWindow>(
      nullptr, windowManager,
      RenderWidgetFactory::CreateRenderWidget(renderType, windowManager));
  mainWindow->show();
}

UIManager::~UIManager() = default;

int UIManager::Run() { return app->exec(); }

} // namespace ui
} // namespace velopraEngine