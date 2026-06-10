#include "VE_QtOpenGLWidget.h"
#include "VE_Core.h"
#include "VE_LoggerMacros.h"
#include "VE_SceneDescription.h"
#include <QTimer>

namespace velopraEngine {
namespace ui {

QtOpenGLWidget::QtOpenGLWidget(std::shared_ptr<render::IRenderer> renderer,
                               QWidget *parent,
                               std::shared_ptr<WindowManager> windowManager)
    : QOpenGLWidget(parent), renderer(std::move(renderer)),
      windowManager(windowManager) {

  QOpenGLWidget::setMouseTracking(true);

  QTimer *timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, [this]() {
    core::Core::Instance().GetEventDispatcher().ProcessEvents();
    for (auto *layer : core::Core::Instance())
      layer->OnUpdate();
    update();
  });
  timer->start(16); // approximately 60 fps
  VELOPRA_CORE_INFO("QT OpenGLWidget created");
}

QtOpenGLWidget::~QtOpenGLWidget() {}

// IRenderWidget — the render-surface lifecycle. Qt callbacks delegate here so
// the interface carries the real behaviour.

void QtOpenGLWidget::InitializeRenderer() {
  if (renderer && !renderer->Initialize(render::SceneDescription{})) {
    VELOPRA_CORE_CRITICAL(
        "Renderer initialization failed; rendering disabled.");
    renderer.reset();
  }
}

void QtOpenGLWidget::ResizeRenderer(int width, int height) {
  if (renderer) {
    renderer->OnWindowSizeChanged(width, height);
  }
}

void QtOpenGLWidget::Render() {
  if (renderer) {
    renderer->BeginFrame();
    renderer->RenderFrame();
  }
}

// Qt lifecycle — delegates to the IRenderWidget seam.

void QtOpenGLWidget::initializeGL() {
  initializeOpenGLFunctions();
  InitializeRenderer();
}

void QtOpenGLWidget::resizeGL(int w, int h) { ResizeRenderer(w, h); }

void QtOpenGLWidget::paintGL() { Render(); }

void QtOpenGLWidget::keyPressEvent(QKeyEvent *event) {
  if (windowManager)
    windowManager->ForwardKeyPressedEvent(event);
}

void QtOpenGLWidget::keyReleaseEvent(QKeyEvent *event) {
  if (windowManager)
    windowManager->ForwardKeyPressedEvent(event);
}

void QtOpenGLWidget::mousePressEvent(QMouseEvent *event) {
  if (windowManager)
    windowManager->ForwardMousePressedEvent(event);
}

void QtOpenGLWidget::mouseMoveEvent(QMouseEvent *event) {
  if (windowManager)
    windowManager->ForwardMouseMoveEvent(event);
}

} // namespace ui
} // namespace velopraEngine
