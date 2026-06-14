#ifndef VE_UI_MANAGER_H
#define VE_UI_MANAGER_H

#include "VE_UIApi.h"
#include "VE_RenderTypes.h"
#include <memory>

class QApplication;

namespace velopraEngine {
namespace ui {

class MainWindow;
class WindowManager;

class VELOPRAUI_API UIManager {
public:
  UIManager(int argc, char *argv[], RenderType renderType,
            std::shared_ptr<WindowManager> windowManager);
  ~UIManager();
  int Run();

private:
  int storedArgc;
  std::unique_ptr<QApplication> app;
  std::unique_ptr<MainWindow> mainWindow;
};

} // namespace ui
} // namespace velopraEngine

#endif // VE_UI_MANAGER_H
