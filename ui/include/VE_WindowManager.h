#ifndef VE_WINDOW_MANAGER_H
#define VE_WINDOW_MANAGER_H

#include "VE_UIApi.h"
#include <QKeyEvent>
#include <QMouseEvent>

namespace velopraEngine {
namespace ui {

class VELOPRAUI_API WindowManager {
public:
  WindowManager();
  ~WindowManager();

  void ForwardKeyPressedEvent(QKeyEvent *event);
  void ForwardMousePressedEvent(QMouseEvent *event);
  void ForwardMouseMoveEvent(QMouseEvent *event);
};

} // namespace ui
} // namespace velopraEngine

#endif // VE_WINDOW_MANAGER_H
