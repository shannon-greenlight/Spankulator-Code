#define USER_MAXLEN 10
#define USER_MAX_PARAMS 5

String user_get_sput(char);

EEPROM_String user_string(USER_MAXLEN);

uint16_t user_params_0[USER_MAX_PARAMS];
uint16_t user_params_1[USER_MAX_PARAMS];
uint16_t user_params_2[USER_MAX_PARAMS];
uint16_t user_params_3[USER_MAX_PARAMS];
uint16_t user_params_4[USER_MAX_PARAMS];
uint16_t user_params_5[USER_MAX_PARAMS];
uint16_t user_params_6[USER_MAX_PARAMS];
uint16_t user_params_7[USER_MAX_PARAMS];
uint16_t user_params_8[USER_MAX_PARAMS];
uint16_t user_params_9[USER_MAX_PARAMS];

String user_labels_0[USER_MAX_PARAMS];
String user_labels_1[USER_MAX_PARAMS];
String user_labels_2[USER_MAX_PARAMS];
String user_labels_3[USER_MAX_PARAMS];
String user_labels_4[USER_MAX_PARAMS];
String user_labels_5[USER_MAX_PARAMS];
String user_labels_6[USER_MAX_PARAMS];
String user_labels_7[USER_MAX_PARAMS];
String user_labels_8[USER_MAX_PARAMS];
String user_labels_9[USER_MAX_PARAMS];

uint16_t user_maxs_0[USER_MAX_PARAMS];
uint16_t user_maxs_1[USER_MAX_PARAMS];
uint16_t user_maxs_2[USER_MAX_PARAMS];
uint16_t user_maxs_3[USER_MAX_PARAMS];
uint16_t user_maxs_4[USER_MAX_PARAMS];
uint16_t user_maxs_5[USER_MAX_PARAMS];
uint16_t user_maxs_6[USER_MAX_PARAMS];
uint16_t user_maxs_7[USER_MAX_PARAMS];
uint16_t user_maxs_8[USER_MAX_PARAMS];
uint16_t user_maxs_9[USER_MAX_PARAMS];

uint16_t user_mins_0[USER_MAX_PARAMS];
uint16_t user_mins_1[USER_MAX_PARAMS];
uint16_t user_mins_2[USER_MAX_PARAMS];
uint16_t user_mins_3[USER_MAX_PARAMS];
uint16_t user_mins_4[USER_MAX_PARAMS];
uint16_t user_mins_5[USER_MAX_PARAMS];
uint16_t user_mins_6[USER_MAX_PARAMS];
uint16_t user_mins_7[USER_MAX_PARAMS];
uint16_t user_mins_8[USER_MAX_PARAMS];
uint16_t user_mins_9[USER_MAX_PARAMS];

uint16_t *user_stuff_0[] = {user_params_0, user_mins_0, user_maxs_0};
uint16_t *user_stuff_1[] = {user_params_1, user_mins_1, user_maxs_1};
uint16_t *user_stuff_2[] = {user_params_2, user_mins_2, user_maxs_2};
uint16_t *user_stuff_3[] = {user_params_3, user_mins_3, user_maxs_3};
uint16_t *user_stuff_4[] = {user_params_4, user_mins_4, user_maxs_4};
uint16_t *user_stuff_5[] = {user_params_5, user_mins_5, user_maxs_5};
uint16_t *user_stuff_6[] = {user_params_6, user_mins_6, user_maxs_6};
uint16_t *user_stuff_7[] = {user_params_7, user_mins_7, user_maxs_7};
uint16_t *user_stuff_8[] = {user_params_8, user_mins_8, user_maxs_8};
uint16_t *user_stuff_9[] = {user_params_9, user_mins_9, user_maxs_9};

SPANK_fxn user_spanker_0("User 0", user_labels_0, user_stuff_0, USER_MAX_PARAMS, &ui);
SPANK_fxn user_spanker_1("User 1", user_labels_1, user_stuff_1, USER_MAX_PARAMS, &ui);
SPANK_fxn user_spanker_2("User 2", user_labels_2, user_stuff_2, USER_MAX_PARAMS, &ui);
SPANK_fxn user_spanker_3("User 3", user_labels_3, user_stuff_3, USER_MAX_PARAMS, &ui);
SPANK_fxn user_spanker_4("User 4", user_labels_4, user_stuff_4, USER_MAX_PARAMS, &ui);
SPANK_fxn user_spanker_5("User 5", user_labels_5, user_stuff_5, USER_MAX_PARAMS, &ui);
SPANK_fxn user_spanker_6("User 6", user_labels_6, user_stuff_6, USER_MAX_PARAMS, &ui);
SPANK_fxn user_spanker_7("User 7", user_labels_7, user_stuff_7, USER_MAX_PARAMS, &ui);
SPANK_fxn user_spanker_8("User 8", user_labels_8, user_stuff_8, USER_MAX_PARAMS, &ui);
SPANK_fxn user_spanker_9("User 9", user_labels_9, user_stuff_9, USER_MAX_PARAMS, &ui);

SPANK_fxn *user_spanks[] = {&user_spanker_0, &user_spanker_1, &user_spanker_2, &user_spanker_3, &user_spanker_4, &user_spanker_5, &user_spanker_6, &user_spanker_7, &user_spanker_8, &user_spanker_9};

int user_param_num;
byte user_dig_num = 0;
int user_op_index = 0;
String user_ops = "UDSTML ";
boolean user_doing_trigger = false;

void user_set_params(char c);
void user_display();

char user_get_char()
{
  return user_string.charAt(user_dig_num);
}

void user_print_sput()
{
  ui.printLine(user_get_sput(user_get_char()), LINE_3, 1);
}

void user_stop_trigger()
{
  user_doing_trigger = doing_trigger = false;
}

void debug()
{
  Serial.println("Howdy User Fxn!");
  user_dig_num++;
  if (user_dig_num >= 4)
    user_doing_trigger = doing_trigger = false;
}

void user_init()
{
  user_param_num = 0;
  user_dig_num = 0;
  user_op_index = 0;
  user_doing_trigger = false;
  user_string.put("");
  // also init string_params
}

void user_underline_char()
{
  const char *c = user_param_num == 0 && !triggered ? "-" : "^";
  ui.underline_char(user_dig_num, 0, 1, 8, c); // min(9,user_dig_num)
}

void user_update_user_string(char cmd, String in_str)
{
  if (user_ops.indexOf(cmd) != -1)
  {
    triggered = user_doing_trigger = false;
    user_string.put(in_str);
    for (user_dig_num = 0; user_dig_num < in_str.length(); user_dig_num++)
    {
      user_set_params(user_string.charAt(user_dig_num));
    }
    user_dig_num = 0;
    user_display();
  }
}

SPANK_fxn user_get_spank_obj(char c)
{
  SPANK_fxn s = dn_spanker;
  switch (c)
  {
  case 'U':
    s = up_spanker;
    break;
  case 'D':
    s = dn_spanker;
    break;
  case 'S':
    s = stretch_spanker;
    break;
  case 'T':
    s = toggle_spanker;
    break;
  case 'M':
    s = maytag_spanker;
    break;
  case 'L':
    s = lfo_spanker;
    lfo_set_params();
    break;
  }
  return s;
}

String user_get_sput(char c)
{
  String s = "Down";
  switch (c)
  {
  case 'U':
    s = "Up";
    break;
  case 'D':
    s = "Down";
    break;
  case 'S':
    s = "Stretch";
    break;
  case 'T':
    s = "Toggle";
    break;
  case 'M':
    s = "Maytag";
    break;
  case 'L':
    s = "LFO";
    break;
  }
  return s;
}

String user_params_toJSON()
{
  //SPANK_fxn s = user_get_spank_obj(user_string.charAt(user_dig_num));
  //SPANK_fxn s = *user_spanks[user_dig_num];
  //s.param_num=9999; // this prevents param from being selected
  //Serial.println("Params to JSON -- Spanker: " + user_spanks[user_dig_num]->name + " num_params: " + String(user_spanks[user_dig_num]->num_params));
  return user_spanks[user_dig_num]->params_toJSON();
}

void user_begin()
{
  //user_spanker_0.trigger_fxn=debug;
  user_string.begin(false);
  user_string.xfer();
  for (int i = 0; i < USER_MAXLEN; i++)
  {
    the_spanker = user_spanks[i];
    the_spanker->begin();
    //user_set_params(user_string.charAt(i));
    //*user_spanks[i].xfer();
    SPANK_fxn f = user_get_spank_obj(user_string.charAt(i));
    if (f.name > "")
    {
      // Serial.println("User Begin: "+user_spanks[i]->name);
      user_spanks[i]->num_params = f.num_params;
      user_spanks[i]->check_params = f.check_params;
      user_spanks[i]->trigger_fxn = f.trigger_fxn;
      user_spanks[i]->string_params = f.string_params;
      for (int j = 0; j < f.num_params; j++)
      {
        user_spanks[i]->labels[j] = f.labels[j];
        user_spanks[i]->mins[j] = f.get_min(j);
        user_spanks[i]->maxs[j] = f.get_max(j);
        //Serial.println("Setting min: "+String(j)+ " val: "+String(f.get_min(j))+ f.name);
      }
      // if (user_string.charAt(i) == 'L')
      // {
      //   user_spanks[i]->string_params = lfo_string_params;
      // }
    }
  }
  //*user_spanks[0].trigger_fxn=up_trigger;
}

void user_display()
{
  ui.fill(BLACK, 16);
  ui.t.clrDown("9");
  ui.printLine(user_string.get(), LINE_1, 1);
  if (true || user_param_num == 0)
  {
    user_underline_char();
  }
  if (user_string.charAt(user_dig_num) == ' ')
  {
    ui.printLine("< or > to delete", LINE_3, 1);
  }
  else
  {
    if (user_dig_num < user_string.length())
    {
      user_spanks[user_dig_num]->enable_hilight = user_param_num == 1;
      user_spanks[user_dig_num]->display_offset = 1;
      user_spanks[user_dig_num]->printParams();
    }
  }
}

void user_fxn()
{
  trigger_params_set = false;
  user_doing_trigger = false;
  ui.newFxn("User");
  // ui.clearDisplay();
  // ui.printText("User",0,0,2);
  //user_string.put("UDU      ");
  //user_string.put("DUSDC     ");
  user_display();
}

void user_inc_param_num()
{
  // Serial.println("Char: " + String(user_string.charAt(user_dig_num)) + "Dig# " + String(user_dig_num));
  if (user_string.charAt(user_dig_num))
  {
    if (user_param_num == 0)
    {
      user_param_num = 1;
    }
    else
    {
      user_spanks[user_dig_num]->inc_param_num_by(1);
      if (user_spanks[user_dig_num]->param_num == 0)
      {
        user_param_num = 0;
      }
    }
    user_display();
  }
}

void user_dec_param_num()
{
  if (user_string.charAt(user_dig_num))
  {
    if (user_param_num == 0)
    {
      user_param_num = 1;
      user_spanks[user_dig_num]->param_num = user_spanks[user_dig_num]->num_params - 1;
    }
    else
    {
      if (user_spanks[user_dig_num]->param_num == 0)
      {
        user_param_num = 0;
      }
      else
      {
        user_spanks[user_dig_num]->inc_param_num_by(-1);
      }
    }
    user_display();
  }
}

void user_remove_blank()
{
  String s = user_string.get();
  int indx = s.indexOf(" ");
  if (indx != -1)
  {
    for (int i = indx; i < s.length() - 1; i++)
    {
      user_spanks[i] = user_spanks[i + 1];
    }
    s.remove(indx, 1);
    user_string.put(s);
    //user_update_user_string('U', s);
  }
}

void user_inc_dig_num_by(byte n)
{
  if (user_param_num == 0)
  {
    user_dig_num += n;
    user_dig_num = max(0, user_dig_num);
    if (user_dig_num > USER_MAXLEN)
      user_dig_num = 0;
    user_remove_blank();
    user_display();
  }
  else
  {
    user_spanks[user_dig_num]->inc_dig_num_by(n);
  }
  // Serial.print("User Inc Dig Num: ");
  // Serial.println(user_dig_num);
}

byte user_get_dig_num()
{
  if (user_param_num == 0)
  {
    return user_dig_num;
  }
  else
  {
    return user_spanks[user_dig_num]->digit_num;
  }
}

void user_home()
{
  user_dig_num = 0;
  user_display();
}

void user_end()
{
  user_dig_num = 0;
  do
  {
    user_dig_num++;
  } while (user_string.charAt(user_dig_num) != ' ' && user_dig_num < USER_MAXLEN);
  user_display();
}

void user_set_params(char c)
{
  SPANK_fxn f = user_get_spank_obj(c);
  f.copy_to(*user_spanks[user_dig_num]);
  if (c == 'L')
  {
    user_spanks[user_dig_num]->string_params = lfo_string_params;
  }
}

int user_get_param_num()
{
  return user_param_num;
  // return user_spanks[user_dig_num]->param_num;
}

void user_select_sequence()
{
  user_param_num = 0;
  user_display();
}

void user_put_param_num(uint16_t pnum)
{
  user_param_num = 1;
  user_spanks[user_dig_num]->param_num = pnum;
  user_display();
  // Serial.println("User put param num: " + user_spanks[user_dig_num]->name);
  // Serial.println("!param_num: " + String(user_spanks[user_dig_num]->param_num));
}

void user_put_param(int param)
{
  //SPANK_fxn g = *user_spanks[user_dig_num];
  // Serial.println("User put param: " + user_spanks[user_dig_num]->name);
  // Serial.println("param_num: " + String(user_spanks[user_dig_num]->param_num));
  // Serial.println("max: " + String(user_spanks[user_dig_num]->get_max()));
  // Serial.println("min: " + String(user_spanks[user_dig_num]->get_min()));
  user_spanks[user_dig_num]->put_param(param);
  ui.clearDisplay();
  ui.printText("User", 0, 0, 2);
  user_display();
}

void user_adjust_param(int encoder_val)
{
  //user_stop_trigger();
  if (triggered)
    return;
  if (user_param_num == 0)
  {
    // char c = user_ops[user_op_index];
    // Serial.print("User Adjust Param: ");
    // Serial.println(user_string.length());
    // Serial.println(user_dig_num);
    user_op_index += encoder_val;
    if (user_op_index < 0)
      user_op_index = user_ops.length() - 1;
    if (user_op_index >= user_ops.length())
      user_op_index = 0;

    char c = user_ops[user_op_index];

    if (user_string.length() > user_dig_num)
    {
      user_string.putCharAt(c, user_dig_num);
    }
    else
    {
      user_string.append(String(c));
    }
    user_set_params(c);
    ui.fill(BLACK, 16);
    ui.t.clrDown("9");
    ui.printLine(user_string.get(), LINE_1, 1);
    user_underline_char();
    if (user_string.charAt(user_dig_num) == ' ')
    {
      ui.printLine("< or > to delete", LINE_3, 1);
    }
    else
    {
      user_print_sput();
    }
    //user_display();
  }
  else
  {
    user_spanks[user_dig_num]->adjust_param(encoder_val, 1);
  }
  // Serial.println(user_spanks[user_dig_num]->get_label(0));
}

void user_do_trigger()
{
  // Serial.println("Doing user trigger!");
  if (!user_doing_trigger)
  {
    // initialize  ptr and set flag
    user_doing_trigger = true;
    user_dig_num = 0;
    user_param_num = 0;
    user_underline_char();
    // user_print_sput();
    // the_spanker->debug();
  }
  the_spanker = user_spanks[user_dig_num];
  the_spanker->trigger_fxn();
  if (!doing_trigger)
  {
    do
    {
      user_dig_num++;
    } while (user_string.charAt(user_dig_num) == ' ');
    if (user_dig_num >= user_string.length())
    {
      user_stop_trigger();
      // user_doing_trigger = doing_trigger = false;
    }
    else
    {
      user_underline_char();
      // user_print_sput();
    }
  }
}
