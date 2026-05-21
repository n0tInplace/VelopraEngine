#include "VE_InputEventGenerator.h"
#include "VE_InputTypes.h"
#include "VE_Core.h"
#include "VE_EventQueue.h"
#include "VE_KeyboardEvent.h"
#include "VE_MouseEvent.h"

namespace velopraEngine {
namespace input {

InputEventGenerator::InputEventGenerator() {
  auto &queue = core::EventQueue::Instance();
  queue.Subscribe(core::EventType::KeyPressed, this);
  queue.Subscribe(core::EventType::KeyReleased, this);
  queue.Subscribe(core::EventType::MouseButtonPressed, this);
  queue.Subscribe(core::EventType::MouseButtonReleased, this);
  queue.Subscribe(core::EventType::MouseMoved, this);
}

InputEventGenerator::~InputEventGenerator() {
  auto &queue = core::EventQueue::Instance();
  queue.Unsubscribe(core::EventType::KeyPressed, this);
  queue.Unsubscribe(core::EventType::KeyReleased, this);
  queue.Unsubscribe(core::EventType::MouseButtonPressed, this);
  queue.Unsubscribe(core::EventType::MouseButtonReleased, this);
  queue.Unsubscribe(core::EventType::MouseMoved, this);
}

void InputEventGenerator::OnEvent(const core::Event &event) {
  switch (event.GetEventType()) {
  case core::EventType::KeyPressed: {
    const auto &e = static_cast<const core::KeyPressedEvent &>(event);
    UpdateKeyState(TranslateKeyCode(e.KeyCode), true);
    break;
  }
  case core::EventType::KeyReleased: {
    const auto &e = static_cast<const core::KeyReleasedEvent &>(event);
    UpdateKeyState(TranslateKeyCode(e.KeyCode), false);
    break;
  }
  case core::EventType::MouseButtonPressed: {
    const auto &e = static_cast<const core::MouseButtonPressedEvent &>(event);
    UpdateMouseButtonState(TranslateMouseCode(e.Button), true);
    break;
  }
  case core::EventType::MouseButtonReleased: {
    const auto &e = static_cast<const core::MouseButtonReleasedEvent &>(event);
    UpdateMouseButtonState(TranslateMouseCode(e.Button), false);
    break;
  }
  case core::EventType::MouseMoved: {
    const auto &e = static_cast<const core::MouseMovedEvent &>(event);
    UpdateMousePosition(e.MouseX, e.MouseY);
    break;
  }
  default:
    break;
  }
}

KeyCode InputEventGenerator::TranslateKeyCode(int qtKey) {
  // ASCII printable range matches directly (A-Z, 0-9, Space, etc.)
  if (qtKey >= 32 && qtKey <= 126)
    return static_cast<KeyCode>(qtKey);

  // Qt special key values (0x01000000 base) mapped to engine KeyCodes
  static const std::unordered_map<int, KeyCode> map = {
      {0x01000000, KeyCode::Escape},      {0x01000001, KeyCode::Tab},
      {0x01000003, KeyCode::Backspace},   {0x01000005, KeyCode::Enter},
      {0x01000006, KeyCode::Insert},      {0x01000007, KeyCode::Delete},
      {0x01000008, KeyCode::Pause},       {0x01000010, KeyCode::Home},
      {0x01000011, KeyCode::End},         {0x01000012, KeyCode::LeftArrow},
      {0x01000013, KeyCode::UpArrow},     {0x01000014, KeyCode::RightArrow},
      {0x01000015, KeyCode::DownArrow},   {0x01000016, KeyCode::PageUp},
      {0x01000017, KeyCode::PageDown},    {0x01000020, KeyCode::LeftShift},
      {0x01000021, KeyCode::LeftControl}, {0x01000023, KeyCode::LeftAlt},
      {0x01000024, KeyCode::CapsLock},    {0x01000025, KeyCode::NumLock},
      {0x01000026, KeyCode::ScrollLock},  {0x01000027, KeyCode::PrintScreen},
      {0x01000030, KeyCode::F1},          {0x01000031, KeyCode::F2},
      {0x01000032, KeyCode::F3},          {0x01000033, KeyCode::F4},
      {0x01000034, KeyCode::F5},          {0x01000035, KeyCode::F6},
      {0x01000036, KeyCode::F7},          {0x01000037, KeyCode::F8},
      {0x01000038, KeyCode::F9},          {0x01000039, KeyCode::F10},
      {0x0100003A, KeyCode::F11},         {0x0100003B, KeyCode::F12},
  };

  auto it = map.find(qtKey);
  return it != map.end() ? it->second : static_cast<KeyCode>(0);
}

MouseCode InputEventGenerator::TranslateMouseCode(int qtButton) {
  // Qt::LeftButton=1, Qt::RightButton=2, Qt::MiddleButton=4
  static const std::unordered_map<int, MouseCode> map = {
      {0x1, MouseCode::Left},
      {0x2, MouseCode::Right},
      {0x4, MouseCode::Middle},
  };
  auto it = map.find(qtButton);
  return it != map.end() ? it->second : MouseCode::Button0;
}

void InputEventGenerator::Update() {}

bool InputEventGenerator::IsKeyPressed(KeyCode key) const {
  auto it = keyStates.find(key);
  return it != keyStates.end() && it->second;
}

bool InputEventGenerator::IsKeyReleased(KeyCode key) const {
  auto it = keyStates.find(key);
  return it != keyStates.end() && !it->second;
}

bool InputEventGenerator::IsMouseButtonPressed(MouseCode button) const {
  auto it = mouseButtonStates.find(button);
  return it != mouseButtonStates.end() && it->second;
}

std::pair<float, float> InputEventGenerator::GetMousePosition() const {
  return {mouseX, mouseY};
}

float InputEventGenerator::GetMouseX() const { return mouseX; }

float InputEventGenerator::GetMouseY() const { return mouseY; }

void InputEventGenerator::UpdateKeyState(KeyCode key, bool pressed) {
  keyStates[key] = pressed;
}

void InputEventGenerator::UpdateMouseButtonState(MouseCode button,
                                                 bool pressed) {
  mouseButtonStates[button] = pressed;
}

void InputEventGenerator::UpdateMousePosition(float x, float y) {
  mouseX = x;
  mouseY = y;
}

} // namespace input
} // namespace velopraEngine