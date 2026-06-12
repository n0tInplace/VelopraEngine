#ifndef VE_INPUT_TYPES_H
#define VE_INPUT_TYPES_H

#include "VE_InputAPI.h"

namespace velopraEngine {
namespace input {

// KeyCodes Enum
enum class VELOPRAINPUT_API KeyCode {
  // Letters
  A = 65,
  B,
  C,
  D,
  E,
  F,
  G,
  H,
  I,
  J,
  K,
  L,
  M,
  N,
  O,
  P,
  Q,
  R,
  S,
  T,
  U,
  V,
  W,
  X,
  Y,
  Z,

  // Numbers
  Zero = 48,
  One,
  Two,
  Three,
  Four,
  Five,
  Six,
  Seven,
  Eight,
  Nine,

  // Function Keys
  F1 = 290,
  F2,
  F3,
  F4,
  F5,
  F6,
  F7,
  F8,
  F9,
  F10,
  F11,
  F12,

  // Control Keys
  Escape = 256,
  Enter,
  Tab,
  Backspace,
  Insert,
  Delete,
  RightArrow,
  LeftArrow,
  DownArrow,
  UpArrow,
  PageUp,
  PageDown,
  Home,
  End,

  // Modifier Keys
  LeftShift = 340,
  LeftControl,
  LeftAlt,
  LeftSuper,
  RightShift,
  RightControl,
  RightAlt,
  RightSuper,

  // Numpad Keys
  NumPad0 = 320,
  NumPad1,
  NumPad2,
  NumPad3,
  NumPad4,
  NumPad5,
  NumPad6,
  NumPad7,
  NumPad8,
  NumPad9,
  NumPadDecimal,
  NumPadDivide,
  NumPadMultiply,
  NumPadSubtract,
  NumPadAdd,
  NumPadEnter,
  NumPadEqual,

  // Miscellaneous Keys
  Space = 32,
  CapsLock = 280,
  ScrollLock,
  NumLock,
  PrintScreen,
  Pause
};

// Mouse Button Enum
enum class VELOPRAINPUT_API MouseCode {
  Button0 = 0,
  Button1,
  Button2,
  Button3,
  Button4,
  Button5,
  Button6,
  Button7,
  Button8,

  Left = Button0,
  Right = Button1,
  Middle = Button2
};

} // namespace input
} // namespace velopraEngine

#endif // VE_INPUT_TYPES_H