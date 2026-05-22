#ifndef VE_EVENT_DISPATCHER_H
#define VE_EVENT_DISPATCHER_H

#include "VE_CoreAPI.h"
#include "VE_Event.h"
#include "VE_EventSubscriber.h"
#include <functional>
#include <memory>
#include <queue>
#include <unordered_map>
#include <vector>

namespace velopraEngine {
namespace core {

class EventDispatcher;

// RAII handle for callback-based subscriptions. Unsubscribes automatically on
// destruction. Move-only — transfer ownership to extend the subscription lifetime.
class VELOPRACORE_API SubscriptionToken {
public:
  SubscriptionToken() = default;
  SubscriptionToken(EventDispatcher *dispatcher, uint64_t id);
  ~SubscriptionToken();
  SubscriptionToken(SubscriptionToken &&) noexcept;
  SubscriptionToken &operator=(SubscriptionToken &&) noexcept;
  SubscriptionToken(const SubscriptionToken &) = delete;
  SubscriptionToken &operator=(const SubscriptionToken &) = delete;

  void Release();

private:
  EventDispatcher *dispatcher = nullptr;
  uint64_t id = 0;
};

class VELOPRACORE_API EventDispatcher {
public:
  using EventCallback = std::function<void(const Event &)>;

  // Queued dispatch — events are delivered when ProcessEvents() is called
  void Post(std::shared_ptr<Event> event);
  void ProcessEvents();

  // Immediate dispatch — fires synchronously, bypasses the queue
  void PostImmediate(std::shared_ptr<Event> event);

  // Pointer-based subscription (EventSubscriber interface)
  void Subscribe(EventType type, EventSubscriber *subscriber);
  void Unsubscribe(EventType type, EventSubscriber *subscriber);

  // Callback-based subscription; returned token unsubscribes on destruction
  SubscriptionToken Subscribe(EventType type, EventCallback callback);
  void Unsubscribe(uint64_t tokenId);

private:
  void Dispatch(const Event &event);

  std::queue<std::shared_ptr<Event>> pendingEvents;
  std::unordered_map<EventType, std::vector<EventSubscriber *>> pointerSubscribers;
  std::unordered_map<EventType, std::vector<std::pair<uint64_t, EventCallback>>> callbackSubscribers;
  uint64_t nextTokenId = 1;
};

} // namespace core
} // namespace velopraEngine

#endif // VE_EVENT_DISPATCHER_H
