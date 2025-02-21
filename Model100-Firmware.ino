// -*- mode: c++ -*-
// Copyright 2016 Keyboardio, inc. <jesse@keyboard.io>
// See "LICENSE" for license details

#ifndef BUILD_INFORMATION
#define BUILD_INFORMATION "locally built on " __DATE__ " at " __TIME__
#endif

#include "Kaleidoscope.h"
#include "kaleidoscope/KeyEvent.h"
#include "Kaleidoscope-FocusSerial.h"
#include "Kaleidoscope-MouseKeys.h"
#include "Kaleidoscope-Macros.h"
#include "Kaleidoscope-LEDControl.h"
#include "Kaleidoscope-LEDEffect-Rainbow.h"
#include "Kaleidoscope-LED-Stalker.h"
#include "Kaleidoscope-LED-AlphaSquare.h"
#include "Kaleidoscope-LED-Palette-Theme.h"
#include "Kaleidoscope-Steno.h"
#include "Kaleidoscope-Colormap.h"
#include "Kaleidoscope-HardwareTestMode.h"
#include "Kaleidoscope-HostPowerManagement.h"
#include "Kaleidoscope-MagicCombo.h"
#include "Kaleidoscope-USB-Quirks.h"
#include "Kaleidoscope-TapDance.h"


enum { QWERTY, GAMING, FUNCTION }; // layers

enum { LEFT_BRACKET, RIGHT_BRACKET }; // tapdance

/* This comment temporarily turns off astyle's indent enforcement
 *   so we can make the keymaps actually resemble the physical key layout better
 */
// *INDENT-OFF*

KEYMAPS(
  [QWERTY] = KEYMAP_STACKED
  (Key_NoKey,    Key_1, Key_2, Key_3, Key_4, Key_5, Key_Equals,
   Key_Tab,      Key_Q, Key_W, Key_E, Key_R, Key_T, TD(LEFT_BRACKET),
   Key_Escape,   Key_A, Key_S, Key_D, Key_F, Key_G,
   Key_Backtick, Key_Z, Key_X, Key_C, Key_V, Key_B, XXX,

   Key_Tab, Key_Backspace, Key_LeftShift, Key_LeftControl,
   ShiftToLayer(FUNCTION),

   Key_Minus,         Key_6, Key_7, Key_8,     Key_9,         Key_0,         XXX,
   TD(RIGHT_BRACKET), Key_Y, Key_U, Key_I,     Key_O,         Key_P,         Key_Backslash,
                      Key_H, Key_J, Key_K,     Key_L,         Key_Semicolon, Key_Quote,
   XXX,               Key_N, Key_M, Key_Comma, Key_Period,    Key_Slash,     XXX,

   Key_RightAlt, Key_Enter, Key_Spacebar, Key_RightGui,
   ShiftToLayer(FUNCTION)),

  [GAMING] = KEYMAP_STACKED
  (___, ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___, ___,

   ___, Key_Space, ___, ___,
   XXX,

   ___, ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___, ___,
        ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___, ___,

   ___, ___, Key_Backspace, ___,
   ___),

  [FUNCTION] =  KEYMAP_STACKED
  (XXX, Key_F1, Key_F2,     Key_F3,      Key_F4,            Key_F5,        XXX,
   XXX, XXX,    XXX,        Key_mouseUp, Key_mouseScrollUp, Key_mouseBtnR, XXX,
   XXX, XXX,    Key_mouseL, Key_mouseDn, Key_mouseR,        Key_mouseBtnL,
   XXX, XXX,    XXX,        XXX,         Key_mouseScrollDn, Key_mouseBtnM, XXX,

   Key_LeftAlt, Key_Delete, Key_LeftControl, Key_LeftShift,
   XXX,
   XXX, Key_F6,               Key_F7,               Key_F8,             Key_F9,                Key_F10, Key_F11,
   XXX, LCTRL(Key_LeftArrow), LCTRL(Key_DownArrow), LCTRL(Key_UpArrow), LCTRL(Key_RightArrow), XXX,     Key_F12,
        Key_LeftArrow,        Key_DownArrow,        Key_UpArrow,        Key_RightArrow,        XXX,     XXX,
   XXX, Key_Home,             Key_PageDown,         Key_PageUp,         Key_End,               XXX,     XXX,
   Key_RightShift, Key_Enter, Key_Tab, Key_RightGui,
   XXX)
) // KEYMAPS(

/* Re-enable astyle's indent enforcement */
// *INDENT-ON*

/** toggleLedsOnSuspendResume toggles the LEDs off when the host goes to sleep,
 * and turns them back on when it wakes up.
 */
void toggleLedsOnSuspendResume(kaleidoscope::plugin::HostPowerManagement::Event event) {
  switch (event) {
  case kaleidoscope::plugin::HostPowerManagement::Suspend:
    LEDControl.disable();
    break;
  case kaleidoscope::plugin::HostPowerManagement::Resume:
    LEDControl.enable();
    break;
  case kaleidoscope::plugin::HostPowerManagement::Sleep:
    break;
  }
}

/** hostPowerManagementEventHandler dispatches power management events (suspend,
 * resume, and sleep) to other functions that perform action based on these
 * events.
 */
void hostPowerManagementEventHandler(kaleidoscope::plugin::HostPowerManagement::Event event) {
  toggleLedsOnSuspendResume(event);
}

/** This 'enum' is a list of all the magic combos used by the Model 01's
 * firmware The names aren't particularly important. What is important is that
 * each is unique.
 *
 * These are the names of your magic combos. They will be used by the
 * `USE_MAGIC_COMBOS` call below.
 */
enum {
  // Toggle between Boot (6-key rollover; for BIOSes and early boot) and NKRO
  // mode.
  COMBO_TOGGLE_NKRO_MODE,
  // Enter test mode
  COMBO_ENTER_TEST_MODE
};

/** Wrappers, to be used by MagicCombo. **/

/**
 * This simply toggles the keyboard protocol via USBQuirks, and wraps it within
 * a function with an unused argument, to match what MagicCombo expects.
 */
static void toggleKeyboardProtocol(uint8_t combo_index) {
  USBQuirks.toggleKeyboardProtocol();
}

void tapDanceAction(uint8_t tap_dance_index, KeyAddr key_addr, uint8_t tap_count, kaleidoscope::plugin::TapDance::ActionType tap_dance_action) {
  switch (tap_dance_index) {
    case LEFT_BRACKET:
      return tapDanceActionKeys(tap_count, tap_dance_action, Key_LeftParen, Key_LeftCurlyBracket, Key_LeftBracket);
    case RIGHT_BRACKET:
      return tapDanceActionKeys(tap_count, tap_dance_action, Key_RightParen, Key_RightCurlyBracket, Key_RightBracket);
  }
}

enum { SWITCH_TO_GAMING, REBOOT_TO_BOOTLOADER };

void switchToGaming(uint8_t combo_index) {
  if (Layer.isActive(GAMING)) {
    Layer.deactivate(GAMING);
  } else {
    Layer.activate(GAMING);
  }
}

void rebootToBootloader(uint8_t combo_index) {
  Kaleidoscope.rebootBootloader();
}

USE_MAGIC_COMBOS(
  [SWITCH_TO_GAMING] = {
    .action = switchToGaming,
    .keys = {R3C6, R3C9}
  },
  [REBOOT_TO_BOOTLOADER] = {
    .action = rebootToBootloader,
    .keys = {R0C0, R0C15}
  }
);

namespace kaleidoscope {
namespace plugin {

class FDEscape : public kaleidoscope::Plugin {
  public: 
    FDEscape() {}
    
    EventHandlerResult onKeyswitchEvent(KeyEvent &event) {
      if (Layer.isActive(QWERTY) && !Layer.isActive(GAMING) && !Layer.isActive(FUNCTION) && !keyIsInjected(event.state)) {
        if (keyToggledOn(event.state)) {
          if (event.addr == KeyAddr(2, 4)) {
            start_time = Kaleidoscope.millisAtCycleStart();
            f_handled = true;
            if (f_stored != Key_NoKey) {
              f_stored = Runtime.lookupKey(event.addr);
              return EventHandlerResult::OK;
            } else {
              f_stored = Runtime.lookupKey(event.addr);
              return EventHandlerResult::EVENT_CONSUMED;
            }
          }

          if (event.addr == KeyAddr(2, 3)) {
            if (f_stored != Key_NoKey) {
              f_stored = Key_NoKey;
              d_handled = true;
              
              Runtime.handleKeyEvent(KeyEvent{KeyAddr::none(), IS_PRESSED | INJECTED, Key_Escape});
              Runtime.handleKeyEvent(KeyEvent{KeyAddr::none(), WAS_PRESSED | INJECTED, Key_Escape});
              return EventHandlerResult::EVENT_CONSUMED;
            } else {
              return EventHandlerResult::OK;
            }
          }

          // interrupted
          if (f_stored != Key_NoKey) {
            Runtime.handleKeyEvent(KeyEvent{KeyAddr::none(), IS_PRESSED | INJECTED, f_stored});
            Runtime.handleKeyEvent(KeyEvent{KeyAddr::none(), WAS_PRESSED | INJECTED, f_stored});
            f_stored = Key_NoKey;
          }
          return EventHandlerResult::OK;
        } else if (keyIsPressed(event.state) && keyWasPressed(event.state)) {
          if (event.addr == KeyAddr(2, 4)) {
            if (f_handled) {
              return EventHandlerResult::EVENT_CONSUMED;
            }
          } else if (event.addr == KeyAddr(2, 3)) {
            if (d_handled) {
              return EventHandlerResult::EVENT_CONSUMED;
            }
          }
        } else if (keyToggledOff(event.state)) {
          if (event.addr == KeyAddr(2, 4)) {
            if (f_stored != Key_NoKey) {
              Runtime.handleKeyEvent(KeyEvent{KeyAddr::none(), IS_PRESSED | INJECTED, f_stored});
              Runtime.handleKeyEvent(KeyEvent{KeyAddr::none(), WAS_PRESSED | INJECTED, f_stored});
              f_stored = Key_NoKey;
            }
            f_handled = false;
          } else if (event.addr == KeyAddr(2, 3)) {
            d_handled = false;
          }
        }
      }
      return EventHandlerResult::OK;
    }

    EventHandlerResult afterEachCycle() {
      if (Kaleidoscope.millisAtCycleStart() - start_time > time_out) {
        f_handled = false;
        if (f_stored != Key_NoKey) {
          Runtime.handleKeyEvent(KeyEvent{KeyAddr::none(), IS_PRESSED | INJECTED, f_stored});
          f_stored = Key_NoKey;
        }
      }
    }

  private:
    Key f_stored = Key_NoKey;
    bool f_handled = false;
    bool d_handled = false;
    uint32_t start_time;
    uint16_t time_out = 200;
};
  

class LEDStatus : public Plugin {
  public:
    LEDStatus() {}
    EventHandlerResult afterEachCycle() {
      cRGB alert_color = CRGB(0, 0, 0);
      bool draw = true;
      if (Layer.isActive(FUNCTION)) {
        alert_color = CRGB(255, 255, 255);
      } else if (Layer.isActive(GAMING)) {
        alert_color = hsvToRgb(247, 95, 245);
      } else {
        draw = false;
      }

      if (draw) {
        for (int i = 0; i < 16; i++) {
          if (i != 7 && i != 8) { // Zero row and 7/8 col are thumb keys
            LEDControl::setCrgbAt(KeyAddr(0, i), alert_color);
          }
        }
      }
    }
};

class Trans : public StalkerEffect::ColorComputer {
 public:
  Trans() {}

  cRGB compute(uint8_t *rev_step) {
    if (*rev_step > 0)
      *rev_step -= 1;
    else
      *rev_step = 0;

    uint8_t step = 255 - *rev_step;

    // Define transition points in HSV
    struct HSV {
        uint8_t h, s, v;
    };

    const HSV colors[] = {
        {138, 95, 253}, // Light Blue
        {247, 95, 245}, // Pink
        {247, 0, 255},     // White
        {247, 95, 245}, // Pink
        {138, 95, 253},  // Light Blue
        {247, 0, 0}     // Black
    };

    const int num_colors = (sizeof(colors) / sizeof(colors[0])) - 1;
    int segment = (step * num_colors) / 256; // Determine which segment we're in
    float t = ((step * num_colors) % 256) / 256.0f; // Interpolation factor

    HSV c1 = colors[segment];
    HSV c2 = colors[segment + 1];

    // Interpolate each HSV component
    uint8_t h = c1.h + (c2.h - c1.h) * t;
    uint8_t s = c1.s + (c2.s - c1.s) * t;
    uint8_t v = c1.v + (c2.v - c1.v) * t;

    return hsvToRgb(h, s, v);
  }
};


} // namespace kaleidoscope
} // namespace plugin

kaleidoscope::plugin::FDEscape FDEscape;
kaleidoscope::plugin::LEDStatus LEDStatus;
kaleidoscope::plugin::Trans Trans;


// First, tell Kaleidoscope which plugins you want to use.
// The order can be important. For example, LED effects are
// added in the order they're listed here.
KALEIDOSCOPE_INIT_PLUGINS(
  // Plugin for communicating with the os over the serial port
  Focus,

  FDEscape,

  // LED plugins
  LEDControl,
  LEDStatus,
  StalkerEffect,

  MagicCombo,

  // The macros plugin adds support for macros
  Macros,

  // The MouseKeys plugin lets you add keys to your keymap which move the mouse.
  MouseKeys,

  // The HostPowerManagement plugin allows us to turn LEDs off when then host
  // goes to sleep, and resume them when it wakes up.
  HostPowerManagement,

  // Used to create braces key
  TapDance,

  // The USBQuirks plugin lets you do some things with USB that we aren't
  // comfortable - or able - to do automatically, but can be useful
  // nevertheless. Such as toggling the key report protocol between Boot (used
  // by BIOSes) and Report (NKRO).
  USBQuirks

  // Steno
  // GeminiPR
);

void setup() {
  Kaleidoscope.setup();
  StalkerEffect.variant = &Trans;
  StalkerEffect.activate();
}

void loop() {
  Kaleidoscope.loop();
}