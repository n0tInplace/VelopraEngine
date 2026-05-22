#include "VE_WindowManager.h"
#include "VE_Core.h"
#include "VE_Event.h"
#include "VE_KeyboardEvent.h"
#include "VE_LoggerMacros.h"
#include "VE_MouseEvent.h"

namespace velopraEngine {
namespace ui {

WindowManager::WindowManager() {
  // Constructor implementation
}

WindowManager::~WindowManager() {
  // Destructor implementation
}

void WindowManager::ForwardKeyPressedEvent(QKeyEvent *event) {
  std::shared_ptr<core::Event> veEvent;

  if (event->type() == QEvent::KeyPress) {
    veEvent = std::make_shared<core::KeyPressedEvent>(event->key(), 0);
    VELOPRA_CORE_INFO("keyPressed");
  } else if (event->type() == QEvent::KeyRelease) {
    veEvent = std::make_shared<core::KeyReleasedEvent>(event->key());
    VELOPRA_CORE_INFO("keyReleased");
  }

  if (veEvent) {
    core::Core::Instance().GetEventDispatcher().Post(veEvent);
  }
}

void WindowManager::ForwardMouseMoveEvent(QMouseEvent *event) {
  auto veEvent =
      std::make_shared<core::MouseMovedEvent>(event->x(), event->y());
  VELOPRA_CORE_INFO("Mouse moved coords X:{} Y:{}", event->x(), event->y());
  core::Core::Instance().GetEventDispatcher().Post(veEvent);
}

void WindowManager::ForwardMousePressedEvent(QMouseEvent *event) {
  std::shared_ptr<core::Event> veEvent;
  if (event->type() == QEvent::MouseButtonPress) {
    veEvent = std::make_shared<core::MouseButtonPressedEvent>(event->button());
    VELOPRA_CORE_INFO("MousePressed");
  } else if (event->type() == QEvent::MouseButtonRelease) {
    veEvent = std::make_shared<core::MouseButtonReleasedEvent>(event->button());
    VELOPRA_CORE_INFO("MouseReleased");
  }
  if (veEvent) {
    core::Core::Instance().GetEventDispatcher().Post(veEvent);
  }
}

void WindowManager::RegisterWindowSizeObserver(
    render::IWindowSizeObserver *observer) {
  sizeObservers.push_back(observer);
}

void WindowManager::UnregisterWindowSizeObserver(
    render::IWindowSizeObserver *observer) {
  sizeObservers.erase(
      std::remove(sizeObservers.begin(), sizeObservers.end(), observer),
      sizeObservers.end());
}

void WindowManager::NotifyWindowSizeChanged(int width, int height) {
  for (auto *observer : sizeObservers) {
    observer->OnWindowSizeChanged(width, height);
  }
}

} // namespace ui
} // namespace velopraEngine
