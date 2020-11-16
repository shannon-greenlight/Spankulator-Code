// declarations for later
String fxn_name(void);
String list_fxns(void);
void process_cmd(String);
int get_dig_num(void);
byte user_get_dig_num(void);
uint16_t get_param_num(void);
void init_all(void);
bool wifi_enabled(void);

// useful fxns
const char *getfield(char *line, int num)
{
  const char *tok;
  for (tok = strtok(line, ",");
       tok && *tok;
       tok = strtok(NULL, ",\n"))
  {
    if (!--num)
      return tok;
  }
  return NULL;
}

boolean isDifferent(float a, float b)
{
  int ai = int(100 * a);
  int bi = int(100 * b);
  return (ai != bi);
}

String enquote(String value)
{
  return "\"" + value + "\"";
}

String toJSON(String key, String value)
{
  return "\"" + key + "\" :\"" + value + "\"";
}

// hardware primitives
void set_adj()
{
  static int saved;
  int temp = analogRead(ain1_pin);
  if (abs(temp - saved) > 40)
  {
    //Serial.println("Setting new adj: " + String(adj));
    adj = saved = temp;
  }
  scale = (adj / float(1023));
  offset = (512 - adj / 2);
}

boolean read_trigger_pin()
{
  return !digitalRead(trigger_button_pin);
}

boolean all_buttons_up()
{
  //Serial.println(digitalRead(trigger_button_pin));
  return digitalRead(up_button_pin) &&
         digitalRead(dn_button_pin) &&
         digitalRead(repeat_button_pin) &&
         digitalRead(trigger_button_pin) &&
         digitalRead(param_up_button_pin) &&
         digitalRead(param_dn_button_pin) &&
         digitalRead(left_button_pin) &&
         digitalRead(right_button_pin);
}

void reset_trigger()
{
  digitalWrite(disable_trigger_pin, LOW);
  delay(1);
  digitalWrite(disable_trigger_pin, HIGH);
}

void disable_trigger(boolean disable)
{
  //digitalWrite(disable_trigger_pin,disable);
}

void do_toggle()
{
  tog.toggle();
}

// void write_dac(uint16_t val) {
//   val = ( val << 2 ) | 0x3000;
//   digitalWrite(dac_select_pin, LOW); //initialize the chip
//   SPI.transfer16(val); // write to device
//   digitalWrite(dac_select_pin, HIGH); // de-initialize the chip
// }

void write_dac(uint16_t val)
{
  analogWrite(aout_pin, val);
}

boolean event_pending = false;
void do_delay(unsigned int d)
{
  // Serial.println("");
  // Serial.println("Do delay: " + String(d) + " millis: " + String(millis()));
  if (d < 200)
  {
    delay(d);
    // Serial.println("Delay done at: " + String(millis()));
  }
  else
  {
    boolean ser_avail, adjusting;
    unsigned long targetMillis = millis() + d;
    do
    {
      adjusting = e.is_adjusting();
      ser_avail = false && Serial.available() > 0;
      event_pending = adjusting || ser_avail || keypress;
      // Serial.println("millis: " + String(millis()));
    } while (millis() < targetMillis && !event_pending && triggered);
  }
}

unsigned int calc_delay(unsigned int d, int rnd)
{
  float delay_adj = rnd * d / 200.0;
  // reuse rnd here
  rnd = random(int(-delay_adj), int(delay_adj));
  return d + rnd;
}

void do_pulse(unsigned int my_delay, int aval1, int aval2)
{
  write_dac(aval1);
  gate.set();
  do_delay(my_delay);
  gate.reset();
  write_dac(aval2);
  do_delay(my_delay);
}

void scale_and_offset(int *val)
{
  *val *= scale;
  *val += offset;
  *val = min(1023, *val);
  *val = max(0, *val);
}

void send_one_pulse(float the_delay, float longest_pulse, float shortest_pulse, int rnd, float the_swell)
{
  unsigned int my_delay = calc_delay(the_delay, rnd);
  int span = longest_pulse - shortest_pulse;
  int normalized_delay = my_delay - shortest_pulse;
  int aval1 = max(0, normalized_delay * 1023 / span);
  int aval2 = max(0, (normalized_delay + the_swell / 2) * 1023 / span);
  aval1 = min(1023, aval1);
  aval2 = min(1023, aval2);
  scale_and_offset(&aval1);
  scale_and_offset(&aval2);
  do_pulse(my_delay, aval1, aval2);
}
