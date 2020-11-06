typedef void (*FunctionPointer)();

// Load 3rd party libraries
#include <stdlib.h>
#include <Wire.h> // For I2C comm, but needed for not getting compile error
#include <Timer5.h>
#include <Adafruit_FRAM_I2C.h>
#include <WiFiNINA.h>
#include <arduino_secrets.h>

// Load Greenface libraries
#include "version_num.h"
#include "TerminalVT100.h"
#include "RotaryEncoder.h"
#include "Greenface_EEPROM.h"
#include "EEPROM_Arr16.h"
#include "EEPROM_Int.h"
#include "EEPROM_Bool.h"
#include "EEPROM_String.h"
#include "WIFI_Util.h"
#include "SPANK_ui.h"

int volatile keypress = 0;
boolean esc_mode = false;
boolean triggered = false;
boolean doing_trigger = false;
// bool toggle_state = false;
int adj;
int init_adj;
float scale;
int offset;
String in_str = ""; // for serial input
boolean cmd_available = false;

#include "hardware_defs.h"
#include "general_fxns.h"

#include "SPANK_fxn.h"

#include "face1.h" // greenface logo art

SPANK_fxn *the_spanker;

// display screen
ArducamSSD1306 display(OLED_RESET);
SPANK_ui ui(&display);

#define EEPROM_DATA_START 16
#define EEPROM_INIT_FLAG 14
#define EEPROM_INIT_PATTERN 0x55aa

int Greenface_EEPROM::eeprom_offset = EEPROM_DATA_START;
EEPROM_Int fxn = EEPROM_Int(0, 1000);  // set max to real max when num_fxns has been defined
EEPROM_Bool repeat_on = EEPROM_Bool(); // sizeof fxn val

bool is_initialized()
{
  return fxn.read_int(EEPROM_INIT_FLAG) == EEPROM_INIT_PATTERN;
}

#define UP_FXN 0
#define DN_FXN 1
#define STRETCH_FXN 2
#define TOGGLE_FXN 3
#define MAYTAG_FXN 4
#define LFO_FXN 5
#define USER_FXN 6
#define DVM_FXN 7
#define WIFI_FXN 8
#define SETTINGS_FXN 9

#include "up_spanker.h"
#include "dn_spanker.h"
#include "stretch_spanker.h"
#include "toggle_spanker.h"
#include "maytag_spanker.h"

#include "lfo.h"
#include "user_fxn.h"
#include "dvm_fxn.h"
#include "wifi_ui.h"
#include "settings_fxn.h"
#include "wdt.h"

#include "hilevel_fxns.h"

void setup(void)
{
  begin_all();

  set_encoder(); // sets msb,lsb for two types of encoder

  // connect if wifi is active
  wifi_attempt_connect(false);

  ui.splash();

  while (keypress == 0)
  {
    if (wifi_active.get())
    {
      do_server();
    }
  }

  //fxn.put(0); // in case fxn is afu
  if (is_initialized())
  {
    exe_fxn();
  }
  else
  {
    init_all();
  }
}

byte wifi_dly_ctr = 0;

void loop()
{
  static boolean trigger_reset = false;
  // static int loop_count = 0;
  // ui.terminal_debug("Loop: " + String(loop_count++));
  if (keypress)
  {
    process_keypress();
  }
  else
  {
    housekeep();
  }
  if (triggered)
  {
    // digitalWrite(triggered_led_pin,HIGH);
    if (!trigger_reset)
    {
      reset_trigger();
      trigger_reset = true;
    }
    do_trigger();
    if (settings_get_ext_clk() == 1 || fxn.get() == SETTINGS_FXN)
    {
      triggered = false;
    }
    else
    {
      triggered = repeat_on.get() || doing_trigger || user_doing_trigger;
    }
    // disable_trigger(triggered);
    //Serial.println("Triggered? "+String(triggered));
  }
  else
  {
    if (settings_get_ext_clk() == 0)
    {
      trigger_reset = false;
      digitalWrite(triggered_led_pin, LOW);
      terminal_print_status();
    }
  }

  if (e.is_adjusting())
  {
    adjust_param(e.getEncoderValue());
    e.resetEncoder();
  }

  if (cmd_available)
  {
    process_cmd(in_str);
    in_str = "";
    cmd_available = false;
  }
  if (wifi_active.get())
  {
    if (true || wifi_dly_ctr++ == 0)
      do_server();
  }
}
