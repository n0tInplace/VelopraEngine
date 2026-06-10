#ifndef VE_QT_OPENGL_WIDGET_H
#define VE_QT_OPENGL_WIDGET_H

#include "VE_IRenderWidget.h"
#include "VE_IRenderer.h"
#include "VE_UIApi.h"
#include "VE_WindowManager.h"
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <memory>

namespace velopraEngine {
namespace ui {

class VELOPRAUI_API QtOpenGLWidget : public QOpenGLWidget,
                                     protected QOpenGLFunctions,
                                     public IRenderWidget {
  Q_OBJECT

public:
  QtOpenGLWidget(std::shared_ptr<render::IRenderer> renderer,
                 QWidget *parent = nullptr,
                 std::shared_ptr<WindowManager> windowManager = nullptr);
  ~QtOpenGLWidget();

  void InitializeRenderer() override;
  void ResizeRenderer(int width, int height) override;
  void Render() override;

  void keyPressEvent(QKeyEvent *event) override;
  void keyReleaseEvent(QKeyEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;

protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

private:
  std::shared_ptr<render::IRenderer> renderer;
  std::shared_ptr<WindowManager> windowManager;
};

} // namespace ui
} // namespace velopraEngine

#endif // VE_QT_OPENGL_WIDGET_H
