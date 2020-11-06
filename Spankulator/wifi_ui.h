#include <Wire.h> // For I2C comm, but needed for not getting compile error
#include <TerminalVT100.h>
#include <stdlib.h>
#include <WiFiNINA.h>

EEPROM_String wifi_password(64);
EEPROM_String wifi_ssid(64);
EEPROM_Bool wifi_active = EEPROM_Bool(); // sizeof fxn val

int wifi_param = 0;
int wifi_dig_num = 0;
String get_label(int i);
void put_param(int);

uint8_t select_wifi_screen = 0;
int numSsid = 0;
byte wifi_num_params = 0;

void wifi_connect(void);
void wifi_connected(void);

void print_wifi_password()
{
  int pos, too;
  pos = max(0, wifi_dig_num - 9);
  too = min(pos + 10, wifi_password.length());
  ui.printLine(wifi_password.get().substring(pos, too), LINE_2, 2);
}

uint16_t wifi_get_param(int p_num)
{
  return wifi_password.charAt(p_num);
}

void wifi_inc_dig_num_by(int val)
{
  wifi_dig_num += val;
  if (wifi_dig_num > wifi_num_params - 1)
    wifi_dig_num = 0;
  if (wifi_dig_num < 0)
    wifi_dig_num = wifi_num_params;
  wifi_param = wifi_get_param(wifi_dig_num); // ??? not sure if we need to do this
  wifi_param = 'a';
}

void wifi_adjust_param(int e)
{
  if (select_wifi_screen == 2)
  {
    wifi_param += e;
    wifi_param = max(0x21, wifi_param);
    wifi_param = min(0x7f, wifi_param);
    wifi_password.putCharAt(char(wifi_param), wifi_dig_num);
    print_wifi_password();
  }
}

void wifi_display_networks()
{
  String temps;
  if (numSsid == 0)
  {
    // wifi_password.put("a");
    ui.clearDisplay();
    ui.printText("WiFi:", 0, 0, 2);
    ui.printLine("Scanning networks... Please wait.", LINE_1, 1);

    numSsid = numNetworks();
    ui.clearDisplay();

    wifi_num_params = numSsid;
    if (numSsid == -1)
    {
      ui.printText("WiFi:", 0, 0, 2);
      ui.printLine("Couldn't get a WiFi connection", LINE_1, 1);
      //Serial.println("Couldn't get a WiFi connection");
    }
    else
    {
      // print the list of networks seen:
      ui.printText("Select:", 0, 0, 2);
      // Serial.print("number of available networks: ");
      // Serial.println(numSsid);
      // Serial.println();
    }
  }
  for (int thisNet = 0; thisNet < numSsid; thisNet++)
  {
    // Serial.print(thisNet + 1);
    // Serial.print(") ");
    // Serial.print("Signal: ");
    // Serial.print(WiFi.RSSI(thisNet));
    // Serial.print(" dBm");
    // Serial.print("\tChannel: ");
    // Serial.print(WiFi.channel(thisNet));
    byte bssid[6];
    // Serial.print("\t\tBSSID: ");
    printMacAddress(WiFi.BSSID(thisNet, bssid));
    // Serial.print("\tEncryption: ");
    printEncryptionType(WiFi.encryptionType(thisNet));
    // Serial.print("\t\tSSID: ");
    // Serial.println(WiFi.SSID(thisNet));
    // Serial.flush();
  }
  // print the network number and name for each network found:
  // Serial.println("Num params: "+String(wifi_num_params)+ "  Param# "+ String(wifi_dig_num));
  ui.fill(BLACK, LINE_1);
  ui.hiliteLine(min(2, wifi_dig_num));
  int start = max(0, wifi_dig_num - 2);
  int end = max(2, wifi_dig_num);
  int lineNum = 0;
  for (int i = start; i <= end; i++)
  {
    //int thisNet = i + wifi_dig_num/3;
    temps = getSsid(i);
    // Serial.println(String(i) + ". " + temps);
    ui.printLine(temps, ui.lines[lineNum++], 1);
  }
}

void wifi_set_ssid()
{
  // Serial.println("Set SSID: "+String(wifi_dig_num));
  wifi_ssid.put(getSsid(wifi_dig_num, false));
  String enc = getEnc(wifi_dig_num);
  if (enc == "None")
  {
    select_wifi_screen++;
    select_wifi_screen++;
    wifi_connect();
  }
  else
  {
    ui.clearDisplay();
    wifi_password.put("a");
    ui.printText("Password", 0, 0, 2);

    wifi_dig_num = 0;
    wifi_num_params = 64; // max num chars for wpa2 password
    wifi_param = 'a';

    ui.printLine("For: " + wifi_ssid.get(), LINE_1, 1);
    ui.printLine("-", LINE_3, 2);
    print_wifi_password();
    //ui.hiliteLine(LINE_2);
    select_wifi_screen++;
  }
}

void enter_wifi_password()
{
  // Serial.println("Entering password for: "+wifi_ssid.get());
  String prefix = "";
  if (wifi_dig_num > wifi_password.length())
    wifi_dig_num = wifi_password.length();
  if (wifi_password.length() <= wifi_dig_num)
    wifi_password.append("a");

  // backspace removes chars
  if (wifi_password.length() - 1 > wifi_dig_num)
  {
    wifi_password.removeCharAt(wifi_dig_num + 1);
    ui.printLine("          ", LINE_2, 2);
  }
  print_wifi_password();
  for (int i = 0; i < min(9, wifi_dig_num); i++)
  {
    prefix += " ";
  }
  ui.fill(BLACK, LINE_3);
  ui.printLine(prefix + "-", LINE_3, 2);
}

String IpAddress2String(const IPAddress &ipAddress)
{
  return String(ipAddress[0]) + String(".") +
         String(ipAddress[1]) + String(".") +
         String(ipAddress[2]) + String(".") +
         String(ipAddress[3]);
}

void wifi_connect()
{
  wifi_active.reset();
  pass = wifi_password.get();
  ssid = wifi_ssid.get();
  //Serial.println("Connecting to: "+wifi_ssid.get() + " Password: " + wifi_password.get());
  // Serial.println("Connecting to: "+wifi_ssid.get());
  ui.clearDisplay();
  ui.printText(wifi_ssid.get(), 0, 0, 2);
  ui.printLine("Status: ", LINE_1, 1);
  wifi_status = WL_IDLE_STATUS;
  do
  {
    if (server.status() == 0 && millis() > wifi_chk_time)
      WIFI_setup();
    ui.printLine("Status: " + getConnectionStatus(wifi_status), LINE_1, 1);
  } while (wifi_status != WL_CONNECTED && !keypress);
  if (wifi_status == WL_CONNECTED)
  {
    WIFI_setup();
    select_wifi_screen = 4;
    wifi_connected();
  }
  else
  {
    ui.fill(BLACK, LINE_1);
    ui.printLine("Cancelled", LINE_1, 1);
    select_wifi_screen = 0;
  }
}

void wifi_attempt_connect(boolean force)
{
  if (wifi_enabled())
  {
    if (force)
    { // true forces WiFi
      wifi_ssid.put(SECRET_SSID);
      wifi_password.put(SECRET_PASS);
      wifi_active.set();
    }
    else
    {
      //wifi_active.reset();
    }
    if (wifi_active.get() && wifi_status != WL_CONNECTED)
    {
      wifi_connect();
      delay(2000);
    }
  }
}

void wifi_connected()
{
  wifi_active.set();
  // Serial.println("Connected to: "+wifi_ssid.get());
  ui.clearDisplay();
  ui.printLine("Status: " + getConnectionStatus(wifi_status), LINE_1, 1);
  ui.printText(wifi_ssid.get(), 0, 0, 2);
  ui.printLine("IP: " + IpAddress2String(WiFi.localIP()), LINE_2, 1);
  //ui.printLine(WiFi.localIP(),LINE_2,1);
  ui.printLine("Signal: " + String(WiFi.RSSI()) + " dBm", LINE_3, 1);
}

String wifi_params_toJSON()
{
  String out = "";
  out += "{ ";
  out += toJSON("label", "Connected to: ");
  out += ", ";
  out += toJSON("type", "text");
  out += ", ";
  out += toJSON("value", String(wifi_ssid.get()));
  out += ", ";
  out += toJSON("selected", "false");
  out += " }";
  out += ",";
  out += "{ ";
  out += toJSON("label", "Status");
  out += ", ";
  out += toJSON("type", "text");
  out += ", ";
  out += toJSON("value", getConnectionStatus(wifi_status));
  out += ", ";
  out += toJSON("selected", "false");
  out += " }";
  out += ",";
  out += "{ ";
  out += toJSON("label", "IP");
  out += ", ";
  out += toJSON("type", "text");
  out += ", ";
  out += toJSON("value", IpAddress2String(WiFi.localIP()));
  out += ", ";
  out += toJSON("selected", "false");
  out += " }";
  out += ",";
  out += "{ ";
  out += toJSON("label", "Signal");
  out += ", ";
  out += toJSON("type", "text");
  out += ", ";
  out += toJSON("value", String(WiFi.RSSI()) + " dBm");
  out += ", ";
  out += toJSON("selected", "false");
  out += " }";
  return "[" + out + "]";
}

// trigger action
void select_wifi_ok()
{
  select_wifi_screen++;
  if (select_wifi_screen > 4)
    select_wifi_screen = 0;
}

void select_wifi_fxn()
{
  if (wifi_enabled())
  {
    disable_trigger(true);
    switch (select_wifi_screen)
    {
    case 0:
      wifi_display_networks();
      break;
    case 1:
      wifi_set_ssid();
      break;
    case 2:
      enter_wifi_password();
      break;
    case 3:
      wifi_connect();
      break;
    case 4:
      wifi_connected();
      break;
    }
  }
  else
  {
    ui.clearDisplay();
    ui.printLine("WiFi:", 0);
    ui.printLine("Disabled", LINE_1, 1);
    ui.printLine("See Settings", LINE_2, 1);
  }
}

void wifi_new_fxn()
{
  select_wifi_screen = (wifi_status == WL_CONNECTED) ? 4 : 0;

  wifi_dig_num = numSsid = 0;
  wifi_param = wifi_get_param(wifi_dig_num);
}

void wifi_begin()
{
  wifi_password.begin(false);
  wifi_ssid.begin(false);
  wifi_active.begin(false);
  wifi_password.xfer();
  wifi_ssid.xfer();
  wifi_active.xfer();
  // wifi_param = wifi_get_param(wifi_dig_num);
}

String params_toJSON()
{
  switch (fxn.get())
  {
  // case LFO_FXN:
  //   return lfo_params_toJSON();
  //   break;
  case DVM_FXN:
    return dvm_params_toJSON();
    break;
  case USER_FXN:
    // Serial.println("User params to JSON!");
    return user_params_toJSON();
    break;
  case WIFI_FXN:
    return wifi_params_toJSON();
    break;
  default:
    return the_spanker->params_toJSON();
  }
}

void send_data_to_client(WiFiClient client, char cmd)
{
  if (cmd == '[')
    return;
  client.print(toJSON("fxn", fxn_name()));
  client.print(",");
  if (cmd == ' ' || cmd == 'f' || cmd == '+' || cmd == '-')
    client.print(list_fxns());
  client.print(toJSON("repeat_on", repeat_on.get() ? "ON" : "OFF"));
  client.print(",");
  client.print(toJSON("triggered", triggered ? "ON" : "OFF"));
  client.print(",");
  client.print(toJSON("cmd", String(cmd)));
  client.print(",");
  client.print(toJSON("digit_num", String(get_dig_num())));
  client.print(",");
  client.print(toJSON("param_num", String(get_param_num())));
  client.print(",");
  client.print(toJSON("adj", String(adj)));
  client.print(",");
  client.print(toJSON("offset", String(offset)));
  client.print(",");

  if (fxn_name() == "User")
  {
    // Serial.println("Sending: "+fxn_name());
    client.print("\"sequence\" : {");
    client.print(toJSON("label", "Sequence"));
    client.print(",");
    client.print(toJSON("type", "sequence"));
    client.print(",");
    client.print(toJSON("value", user_string.get()));
    client.print(",");
    client.print(toJSON("legal_values", user_ops));
    client.print(",");
    client.print(toJSON("dig_num", String(user_dig_num)));
    client.print(",");
    client.print(toJSON("selected", user_param_num == 0 ? "true" : "false"));
    client.print("},");
  }

  if (fxn_name() == "DVM")
  {
    client.print(toJSON("meas", dvm_meas));
    client.print(",");
  }

  client.print("\"params\" : [");
  // fix this for other fxns
  client.print(params_toJSON());
  client.print("]");
}

void _do_server()
{
  char cmd;
  WiFiClient client = server.available(); // listen for incoming clients

  if (client)
  { // if you get a client,
    // Serial.println("new client"); // print a message out the serial port
    String currentLine = ""; // make a String to hold incoming data from the client
    while (client.connected())
    { // loop while the client's connected
      if (client.available())
      {                         // if there's bytes to read from the client,
        char c = client.read(); // read a byte, then
        //Serial.write(c);                    // print it out the serial monitor
        if (c == '\n')
        { // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0)
          {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            //client.println("Content-type:application/json");
            client.println("Access-Control-Allow-Origin: *");
            client.println();

            client.print("{");
            send_data_to_client(client, cmd);
            client.print("}");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          }
          else
          { // if you got a newline, then clear currentLine:
            if (strstr(currentLine.c_str(), "POST"))
            {
              cmd = currentLine[6];
              if (cmd == '[')
              {
                esc_mode = true;
                cmd = currentLine[7];
              }

              // Serial.println("Command: "+String(cmd));
              //int c = cmd;
              String in_str = "";
              int i = 0;
              char inchar;
              do
              {
                inchar = currentLine[i + 6];
                in_str += inchar;
                i++;
              } while (String(inchar) != " " && i < 20);
              //Serial.println("Receiving: " + in_str);

              process_cmd(in_str);
            }
            currentLine = "";
          }
        }
        else if (c != '\r')
        {                   // if you got anything else but a carriage return character,
          currentLine += c; // add it to the end of the currentLine
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);

    // close the connection:
    client.stop();
    // Serial.println("client disonnected");
  }
  if (!triggered && fxn.get() != LFO_FXN && wifi_status == WL_CONNECTED)
    ui.printWiFiBars(WiFi.RSSI());
}

void do_server()
{
  if (wifi_enabled())
  {
    _do_server();
  }
}
