uint16_t _maytag_params[3];
uint16_t _maytag_mins[] = {1, 0, 1};
uint16_t _maytag_maxs[] = {9999, 9999, 99};
uint16_t _maytag_init_vals[] = {100, 500, 20};
uint16_t *maytag_stuff[] = {_maytag_params, _maytag_mins, _maytag_maxs, _maytag_init_vals};
SPANK_fxn maytag_spanker("Maytag", stretch_labels, maytag_stuff, sizeof(_maytag_params) / sizeof(_maytag_params[0]), &ui);

void maytag_trigger()
{
  // Serial.println("Doing Maytag trigger");
  // reset_trigger();
  // return;
  reset_trigger();
  doing_trigger = true;
  float max_len = the_spanker->get_param(0);
  int delay = the_spanker->get_param(1);
  int num_pulses = the_spanker->get_param(2);
  // Serial.print("Pulse len:");
  // Serial.println(max_len);
  // Serial.print("Delay:");
  // Serial.println(delay);
  // Serial.print("Num Pulses:");
  // Serial.println(num_pulses);
  do_delay(delay);
  for (int i = 0; i < num_pulses && !event_pending; i++)
  {
    int my_len = random(10, max_len);
    int aval1 = max(0, my_len * 1023 / max_len);
    aval1 = min(1023, aval1);
    scale_and_offset(&aval1);
    aval1 *= scale;
    aval1 += offset;
    do_pulse(my_len, aval1, aval1);
    // Serial.print("Num Pulses left:");
    // Serial.println(num_pulses - i);
  }
  doing_trigger = false;
  do_toggle();
}

void maytag_fxn()
{
  the_spanker = &maytag_spanker;
  maytag_spanker.display();
}

void maytag_begin()
{
  maytag_spanker.begin();
  maytag_spanker.trigger_fxn = maytag_trigger;
}
