#ifndef VE_UI_MANAGER_H
#define VE_UI_MANAGER_H

#include "VE_MainWindow.h"
#include "VE_UIApi.h"
#include "VE_RenderTypes.h"
#include <QApplication>

namespace velopraEngine {
namespace ui {

class WindowManager;

class VELOPRAUI_API UIManager {
public:
  UIManager(int argc, char *argv[], RenderType renderType,
            std::shared_ptr<WindowManager> windowManager);
  ~UIManager();
  int Run();

private:
  QApplication app;
  MainWindow mainWindow;
};

} // namespace ui
} // namespace velopraEngine

#endif // VE_UI_MANAGER_H
