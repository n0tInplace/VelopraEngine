#include "VE_MainWindow.h"
#include "VE_IRenderWidget.h"
#include "VE_LoggerMacros.h"
#include <QEvent>
#include <QKeyEvent>
#include <QLabel>
#include <QMouseEvent>

namespace velopraEngine {
namespace ui {

MainWindow::MainWindow(QWidget *parent,
                       std::shared_ptr<WindowManager> windowManager,
                       std::unique_ptr<IRenderWidget> rW)
    : QMainWindow(parent), renderWidget(std::move(rW)) {

  if (renderWidget) {
    setCentralWidget(dynamic_cast<QWidget *>(renderWidget.get()));
  }

  // Left Dock Widget
  leftDockWidget = new QDockWidget(tr("Left Panel"), this);
  leftDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea);
  leftDockWidget->setWidget(new QLabel("Left Panel Content", leftDockWidget));
  addDockWidget(Qt::LeftDockWidgetArea, leftDockWidget);

  // Right Dock Widget
  rightDockWidget = new QDockWidget(tr("Right Panel"), this);
  rightDockWidget->setAllowedAreas(Qt::RightDockWidgetArea);
  rightDockWidget->setWidget(
      new QLabel("Right Panel Content", rightDockWidget));
  addDockWidget(Qt::RightDockWidgetArea, rightDockWidget);

  showMaximized();
  VELOPRA_CORE_INFO("QT Main window created");
}

MainWindow::~MainWindow() = default;

} // namespace ui
} // namespace velopraEngine