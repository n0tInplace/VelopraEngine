#include "VE_QtOpenGLWidget.h"
#include "VE_Core.h"
#include "VE_LoggerMacros.h"
#include "VE_SceneDescription.h"
#include "VE_Time.h"
#include <QCoreApplication>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QTimer>

namespace velopraEngine {
namespace ui {

QtOpenGLWidget::QtOpenGLWidget(std::shared_ptr<render::IRenderer> renderer,
                               QWidget *parent,
                               std::shared_ptr<WindowManager> windowManager)
    : QOpenGLWidget(parent), renderer(std::move(renderer)),
      windowManager(windowManager) {

  QOpenGLWidget::setMouseTracking(true);
  setFocusPolicy(Qt::StrongFocus);

  QTimer *timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, [this]() {
    core::Time::Instance().Update();
    core::Core::Instance().GetEventDispatcher().ProcessEvents();
    for (auto *layer : core::Core::Instance())
      layer->OnUpdate();

    if (this->renderer) {
      float dt = static_cast<float>(core::Time::Instance().GetDeltaTime());
      if (wDown) this->renderer->OnCameraMove(render::Camera_Movement::FORWARD,  dt);
      if (sDown) this->renderer->OnCameraMove(render::Camera_Movement::BACKWARD, dt);
      if (aDown) this->renderer->OnCameraMove(render::Camera_Movement::LEFT,     dt);
      if (dDown) this->renderer->OnCameraMove(render::Camera_Movement::RIGHT,    dt);
    }

    update();
  });
  timer->start(16); // approximately 60 fps
  VELOPRA_CORE_INFO("QT OpenGLWidget created");
}

QtOpenGLWidget::~QtOpenGLWidget() {}

// IRenderWidget — the render-surface lifecycle. Qt callbacks delegate here so
// the interface carries the real behaviour.

void QtOpenGLWidget::InitializeRenderer() {
  std::string assetDir =
      QCoreApplication::applicationDirPath().toStdString() + "/";
  render::SceneDescription scene;
  scene.modelPath          = assetDir + "model.obj";
  scene.vertexShaderPath   = assetDir + "vertex_shader.glsl";
  scene.fragmentShaderPath = assetDir + "fragment_shader.glsl";
  if (renderer && !renderer->Initialize(scene)) {
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
  switch (event->key()) {
    case Qt::Key_W: wDown = true; break;
    case Qt::Key_A: aDown = true; break;
    case Qt::Key_S: sDown = true; break;
    case Qt::Key_D: dDown = true; break;
    default: break;
  }
  if (windowManager)
    windowManager->ForwardKeyPressedEvent(event);
}

void QtOpenGLWidget::keyReleaseEvent(QKeyEvent *event) {
  switch (event->key()) {
    case Qt::Key_W: wDown = false; break;
    case Qt::Key_A: aDown = false; break;
    case Qt::Key_S: sDown = false; break;
    case Qt::Key_D: dDown = false; break;
    default: break;
  }
  if (windowManager)
    windowManager->ForwardKeyPressedEvent(event);
}

void QtOpenGLWidget::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::RightButton) {
    rightMouseDown = true;
    lastMouseX = static_cast<float>(event->x());
    lastMouseY = static_cast<float>(event->y());
  }
  if (windowManager)
    windowManager->ForwardMousePressedEvent(event);
}

void QtOpenGLWidget::mouseReleaseEvent(QMouseEvent *event) {
  if (event->button() == Qt::RightButton)
    rightMouseDown = false;
}

void QtOpenGLWidget::mouseMoveEvent(QMouseEvent *event) {
  if (rightMouseDown && renderer) {
    float x = static_cast<float>(event->x());
    float y = static_cast<float>(event->y());
    renderer->OnCameraRotate(x - lastMouseX, lastMouseY - y);
    lastMouseX = x;
    lastMouseY = y;
  }
  if (windowManager)
    windowManager->ForwardMouseMoveEvent(event);
}

} // namespace ui
} // namespace velopraEngine
