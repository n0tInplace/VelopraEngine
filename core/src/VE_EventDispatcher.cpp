#include "VE_EventDispatcher.h"
#include "VE_LoggerMacros.h"
#include <algorithm>

namespace velopraEngine {
namespace core {

// ── SubscriptionToken ────────────────────────────────────────────────────────

SubscriptionToken::SubscriptionToken(EventDispatcher *dispatcher, uint64_t id)
    : dispatcher(dispatcher), id(id) {}

SubscriptionToken::~SubscriptionToken() { Release(); }

SubscriptionToken::SubscriptionToken(SubscriptionToken &&other) noexcept
    : dispatcher(other.dispatcher), id(other.id) {
  other.dispatcher = nullptr;
  other.id = 0;
}

SubscriptionToken &
SubscriptionToken::operator=(SubscriptionToken &&other) noexcept {
  if (this != &other) {
    Release();
    dispatcher = other.dispatcher;
    id = other.id;
    other.dispatcher = nullptr;
    other.id = 0;
  }
  return *this;
}

void SubscriptionToken::Release() {
  if (dispatcher && id != 0) {
    dispatcher->Unsubscribe(id);
    dispatcher = nullptr;
    id = 0;
  }
}

// ── EventDispatcher ──────────────────────────────────────────────────────────

void EventDispatcher::Post(std::shared_ptr<Event> event) {
  pendingEvents.push(std::move(event));
}

void EventDispatcher::ProcessEvents() {
  while (!pendingEvents.empty()) {
    auto event = std::move(pendingEvents.front());
    pendingEvents.pop();
    VELOPRA_CORE_TRACE("Processing event: {}", event->ToString());
    Dispatch(*event);
  }
}

void EventDispatcher::PostImmediate(std::shared_ptr<Event> event) {
  Dispatch(*event);
}

void EventDispatcher::Subscribe(EventType type, EventSubscriber *subscriber) {
  pointerSubscribers[type].push_back(subscriber);
}

void EventDispatcher::Unsubscribe(EventType type, EventSubscriber *subscriber) {
  auto &vec = pointerSubscribers[type];
  vec.erase(std::remove(vec.begin(), vec.end(), subscriber), vec.end());
}

SubscriptionToken EventDispatcher::Subscribe(EventType type,
                                              EventCallback callback) {
  uint64_t id = nextTokenId++;
  callbackSubscribers[type].emplace_back(id, std::move(callback));
  return SubscriptionToken(this, id);
}

void EventDispatcher::Unsubscribe(uint64_t tokenId) {
  for (auto &[type, callbacks] : callbackSubscribers) {
    callbacks.erase(
        std::remove_if(callbacks.begin(), callbacks.end(),
                       [tokenId](const auto &entry) {
                         return entry.first == tokenId;
                       }),
        callbacks.end());
  }
}

void EventDispatcher::Dispatch(const Event &event) {
  EventType type = event.GetEventType();

  auto pit = pointerSubscribers.find(type);
  if (pit != pointerSubscribers.end()) {
    for (auto *sub : pit->second) {
      sub->OnEvent(event);
      if (event.Handled)
        return;
    }
  }

  auto cit = callbackSubscribers.find(type);
  if (cit != callbackSubscribers.end()) {
    for (auto &[id, cb] : cit->second)
      cb(event);
  }
}

} // namespace core
} // namespace velopraEngine
