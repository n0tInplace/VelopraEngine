#include "VE_Core.h"

namespace velopraEngine {
namespace core {

Core &Core::Instance() {
  static Core instance;
  return instance;
}

EventDispatcher &Core::GetEventDispatcher() { return eventDispatcher; }

void Core::PushLayer(Layer *layer) {
  layerStack.PushLayer(layer);
  layer->OnAttach();
}

void Core::PushOverlay(Layer *overlay) {
  layerStack.PushOverlay(overlay);
  overlay->OnAttach();
}

LayerStack::iterator Core::begin() { return layerStack.begin(); }

LayerStack::iterator Core::end() { return layerStack.end(); }

LayerStack::reverse_iterator Core::rbegin() { return layerStack.rbegin(); }

LayerStack::reverse_iterator Core::rend() { return layerStack.rend(); }

} // namespace core
} // namespace velopraEngine