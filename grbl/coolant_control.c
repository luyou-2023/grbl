/*
  coolant_control.c - coolant control methods
   Grbl的一部分

  版权所有 2011-2016 Sungeun K. Jeon for Gnea Research LLC
  版权所有 2009-2011 Simen Svale Skogsrud
  
  Grbl 是自由软件：你可以在自由软件基金会的GNU 普通公共许可(GPL v3+)条款下发行，或修改它。
  Grbl的发布是希望它能有用，但没有任何保证;甚至没有隐含的保证适销性或适合某一特定目的。
  更多详细信息，请参阅GNU通用公共许可证。

  您应该已经收到GNU通用公共许可证的副本和Grbl一起。如果没有，请参阅<http://www.gnu.org/licenses/>。
*/

#include "grbl.h"


void coolant_init()
{
  COOLANT_FLOOD_DDR |= (1 << COOLANT_FLOOD_BIT); // Configure as output pin
  #ifdef ENABLE_M7
    COOLANT_MIST_DDR |= (1 << COOLANT_MIST_BIT);
  #endif
  coolant_stop();
}


// Returns current coolant output state. Overrides may alter it from programmed state.
uint8_t coolant_get_state()
{
  uint8_t cl_state = COOLANT_STATE_DISABLE;
  #ifdef INVERT_COOLANT_FLOOD_PIN
    if (bit_isfalse(COOLANT_FLOOD_PORT,(1 << COOLANT_FLOOD_BIT))) {
  #else
    if (bit_istrue(COOLANT_FLOOD_PORT,(1 << COOLANT_FLOOD_BIT))) {
  #endif
    cl_state |= COOLANT_STATE_FLOOD;
  }
  #ifdef ENABLE_M7
    #ifdef INVERT_COOLANT_MIST_PIN
      if (bit_isfalse(COOLANT_MIST_PORT,(1 << COOLANT_MIST_BIT))) {
    #else
      if (bit_istrue(COOLANT_MIST_PORT,(1 << COOLANT_MIST_BIT))) {
    #endif
      cl_state |= COOLANT_STATE_MIST;
    }
  #endif
  return(cl_state);
}


// Directly called by coolant_init(), coolant_set_state(), and mc_reset(), which can be at
// an interrupt-level. No report flag set, but only called by routines that don't need it.
void coolant_stop()
{
  #ifdef INVERT_COOLANT_FLOOD_PIN
    COOLANT_FLOOD_PORT |= (1 << COOLANT_FLOOD_BIT);
  #else
    COOLANT_FLOOD_PORT &= ~(1 << COOLANT_FLOOD_BIT);
  #endif
  #ifdef ENABLE_M7
    #ifdef INVERT_COOLANT_MIST_PIN
      COOLANT_MIST_PORT |= (1 << COOLANT_MIST_BIT);
    #else
      COOLANT_MIST_PORT &= ~(1 << COOLANT_MIST_BIT);
    #endif
  #endif
}


// Main program only. Immediately sets flood coolant running state and also mist coolant, 
// if enabled. Also sets a flag to report an update to a coolant state.
// Called by coolant toggle override, parking restore, parking retract, sleep mode, g-code
// parser program end, and g-code parser coolant_sync().
void coolant_set_state(uint8_t mode)
{
  if (sys.abort) { return; } // Block during abort.  
  
	if (mode & COOLANT_FLOOD_ENABLE) {
		#ifdef INVERT_COOLANT_FLOOD_PIN
			COOLANT_FLOOD_PORT &= ~(1 << COOLANT_FLOOD_BIT);
		#else
			COOLANT_FLOOD_PORT |= (1 << COOLANT_FLOOD_BIT);
		#endif
	} else {
	  #ifdef INVERT_COOLANT_FLOOD_PIN
			COOLANT_FLOOD_PORT |= (1 << COOLANT_FLOOD_BIT);
		#else
			COOLANT_FLOOD_PORT &= ~(1 << COOLANT_FLOOD_BIT);
		#endif
	}
  
	#ifdef ENABLE_M7
		if (mode & COOLANT_MIST_ENABLE) {
			#ifdef INVERT_COOLANT_MIST_PIN
				COOLANT_MIST_PORT &= ~(1 << COOLANT_MIST_BIT);
			#else
				COOLANT_MIST_PORT |= (1 << COOLANT_MIST_BIT);
			#endif
		} else {
			#ifdef INVERT_COOLANT_MIST_PIN
				COOLANT_MIST_PORT |= (1 << COOLANT_MIST_BIT);
			#else
				COOLANT_MIST_PORT &= ~(1 << COOLANT_MIST_BIT);
			#endif
		}
	#endif
	
  sys.report_ovr_counter = 0; // Set to report change immediately
}


// G-code parser entry-point for setting coolant state. Forces a planner buffer sync and bails 
// if an abort or check-mode is active.
void coolant_sync(uint8_t mode)
{
  if (sys.state == STATE_CHECK_MODE) { return; }
  protocol_buffer_synchronize(); // Ensure coolant turns on when specified in program.
  coolant_set_state(mode);
}
