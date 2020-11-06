uint16_t _toggle_params[1];
uint16_t _toggle_mins[] = {0};
uint16_t _toggle_maxs[] = {9999};
uint16_t _toggle_init_vals[] = {100};
uint16_t *toggle_stuff[] = {_toggle_params, _toggle_mins, _toggle_maxs, _toggle_init_vals};
String toggle_labels[] = {"Delay: ", "State: "};
SPANK_fxn toggle_spanker("Toggle", toggle_labels, toggle_stuff, sizeof(_toggle_params) / sizeof(_toggle_params[0]), &ui);

void toggle_trigger()
{
  reset_trigger();
  triggered = true;
  doing_trigger = true;
  int delay = the_spanker->get_param(0);
  int aval1 = gate.get() ? 1023 : 0;
  scale_and_offset(&aval1);
  // Serial.print("Toggle Trigger, aval1: ");
  // Serial.println(aval1);
  write_dac(aval1);
  do_delay(delay);
  reset_trigger();
  gate.toggle();
  do_toggle();
  doing_trigger = false;
}

void toggle_fxn()
{
  the_spanker = &toggle_spanker;
  // possibly init toggle state here
  toggle_spanker.display();
}

void toggle_begin()
{
  toggle_spanker.begin();
  toggle_spanker.trigger_fxn = toggle_trigger;
}
