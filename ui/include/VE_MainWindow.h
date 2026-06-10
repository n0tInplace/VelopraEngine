#ifndef VE_MAIN_WINDOW_H
#define VE_MAIN_WINDOW_H

#include "VE_IRenderWidget.h"
#include "VE_UIApi.h"
#include <QDockWidget>
#include <QMainWindow>

namespace velopraEngine {
namespace ui {

class WindowManager;

class VELOPRAUI_API MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr,
             std::shared_ptr<WindowManager> windowManager = nullptr,
             std::unique_ptr<IRenderWidget> rW = nullptr);

private:
  std::unique_ptr<IRenderWidget> renderWidget;
  QDockWidget *rightDockWidget;
  QDockWidget *leftDockWidget;
};

} // namespace ui
} // namespace velopraEngine

#endif // VE_MAIN_WINDOW_H
