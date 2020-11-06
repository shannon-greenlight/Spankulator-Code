uint16_t _settings_params[5];
uint16_t _settings_mins[] = {0, 0, 0, 0, 0};
uint16_t _settings_maxs[] = {0, 1, 1, 1, 0};
uint16_t _settings_init_vals[] = {0, 0, 0, 1, 0};
uint16_t *settings_stuff[] = {_settings_params, _settings_mins, _settings_maxs, _settings_init_vals};
String settings_labels[] = {"Version: ", "Clock: ", "Encoder Type: ", "WiFi: ", "Reset: "};
String settings_string_params[] = {VERSION_NUM, "Internal,External", "Normal ,Reverse", "Disabled,Enabled ", "Push Trig"};
SPANK_fxn settings_spanker("Settings", settings_labels, settings_stuff, sizeof(_settings_params) / sizeof(_settings_params[0]), &ui);

void settings_fxn()
{
  the_spanker = &settings_spanker;
  settings_spanker.display();
}

uint16_t settings_get_ext_clk()
{
  return settings_spanker.get_param(1);
}

uint16_t settings_get_encoder_type()
{
  return settings_spanker.get_param(2);
}

bool wifi_enabled()
{
  return settings_spanker.get_param(3);
}

void settings_trigger()
{
  if (settings_spanker.param_num != 4)
  {
    ui.terminal_debug("Initialized: " + String(is_initialized()) + " Pattern: " + String(EEPROM_INIT_PATTERN));
  }
  if (settings_spanker.param_num == 4)
  {
    init_all();
  }
  // ui.terminal_debug("Settings Trigger!");
  // up_spanker.init();
}

void settings_begin()
{
  settings_spanker.begin();
  settings_spanker.string_params = settings_string_params;
  settings_spanker.trigger_fxn = settings_trigger;
}
