/*
  coolant_control.h - spindle control methods
   Grbl的一部分

  版权所有 2011-2016 Sungeun K. Jeon for Gnea Research LLC
  版权所有 2009-2011 Simen Svale Skogsrud
  
  Grbl 是自由软件：你可以在自由软件基金会的GNU 普通公共许可(GPL v3+)条款下发行，或修改它。
  Grbl的发布是希望它能有用，但没有任何保证;甚至没有隐含的保证适销性或适合某一特定目的。
  更多详细信息，请参阅GNU通用公共许可证。

  您应该已经收到GNU通用公共许可证的副本和Grbl一起。如果没有，请参阅<http://www.gnu.org/licenses/>。
*/

#ifndef coolant_control_h
#define coolant_control_h

#define COOLANT_NO_SYNC     false
#define COOLANT_FORCE_SYNC  true

#define COOLANT_STATE_DISABLE   0  // Must be zero
#define COOLANT_STATE_FLOOD     PL_COND_FLAG_COOLANT_FLOOD
#define COOLANT_STATE_MIST      PL_COND_FLAG_COOLANT_MIST


// Initializes coolant control pins.
void coolant_init();

// Returns current coolant output state. Overrides may alter it from programmed state.
uint8_t coolant_get_state();

// Immediately disables coolant pins.
void coolant_stop();

// Sets the coolant pins according to state specified.
void coolant_set_state(uint8_t mode);

// G-code parser entry-point for setting coolant states. Checks for and executes additional conditions.
void coolant_sync(uint8_t mode);

#endif
