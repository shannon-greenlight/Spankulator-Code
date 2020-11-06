#include "Arduino.h"
#include <TerminalVT100.h>
#include <Greenface_EEPROM.h>
#include "SPANK_ui.h"

#include "SPANK_fxn.h"

// vector<string> split (const string &s, char delim) {
//     vector<string> result;
//     stringstream ss (s);
//     string item;

//     while (getline (ss, item, delim)) {
//         result.push_back (item);
//     }

//     return result;
// }

String csv_elem(String s, char delim, int instance)
{
    //vector <String> out;
    int indx = 0;
    int found = 0;
    for (int i = 0; i < s.length() + 1; i++)
    {
        if (s.charAt(i) == delim || i == s.length())
        {
            if (found == instance)
            {
                return s.substring(indx, i);
            }
            found++;
            indx = i + 1;
        }
    }
    return "Not found!";
}

SPANK_fxn::SPANK_fxn(String _name, String *_labels, uint16_t **_params, uint16_t _num_params, SPANK_ui *_ui) : params(*_params, _num_params)
{
    name = _name;
    labels = _labels;
    ui = _ui;
    num_params = _num_params;
    param_num = 0;
    digit_num = 0;
    params = EEPROM_Arr16(_params[0], _num_params);
    mins = _params[1];
    maxs = _params[2];
    init_vals = _params[3];
    display_fxn = &SPANK_fxn::default_display;
}

void SPANK_fxn::begin()
{
    params.begin(false);
    params.xfer();
    // Serial.println("");
    // Serial.print(name + " Beginning "+labels[0]);
    // Serial.println(params.get(0));
}

int SPANK_fxn::offset()
{
    return params.offset;
}

uint16_t SPANK_fxn::get_param(int16_t indx)
{
    if (indx == -1)
        indx = param_num;
    return params.get(indx);
    //return params[param_num];
}

uint16_t SPANK_fxn::get_min(int16_t indx)
{
    if (indx == -1)
        indx = param_num;
    return mins[indx];
}

uint16_t SPANK_fxn::get_max(int16_t indx)
{
    if (indx == -1)
        indx = param_num;
    return maxs[indx];
}

void SPANK_fxn::adjust_param(int e, unsigned long delta)
{
    uint16_t the_param = get_param();
    int multiplier = 1;
    uint8_t ndigs = get_num_digits(param_num);
    int dig = (ndigs - digit_num) - 1;
    // Serial.println("mult: "+String(dig));
    multiplier = pow(10, dig);

    the_param += e * multiplier;

    put_param(the_param);
}

int SPANK_fxn::check_param(int param)
{
    // Serial.println("Param OK param#: "+ String(param_num));
    switch (param_num)
    {
    case LONGEST_PULSE:
        param = max(param, get_param(SHORTEST_PULSE) + 1);
        break;
    case SHORTEST_PULSE:
        param = min(param, get_param(LONGEST_PULSE) - 1);
        break;
    }
    return param;
}

void SPANK_fxn::print_param(uint16_t p_num, uint16_t line_num)
{
    boolean is_string = false;
    String s;
    if (string_params)
    {
        if (string_params[p_num].length())
        {
            s = csv_elem(string_params[p_num], ',', get_param(p_num));
            is_string = true;
        }
    }
    if (is_string)
    {
        (*ui).printLine(get_label(p_num) + s, (*ui).lines[line_num], 1);
    }
    else
    {
        uint8_t ndigs = get_num_digits(p_num);
        String fmt = calc_format(ndigs);
        const char *format = fmt.c_str();
        (*ui).printParam(get_label(p_num), get_param(p_num), ndigs, format, line_num);
        // (*ui).terminal_debug("Print param: " + String(line_num));
    }
}

// TODO fix data type to allow negative numbers
void SPANK_fxn::put_param(uint16_t val)
{
    boolean is_string = false;
    String s;
    int16_t val1 = val; // deals with 'negative' numbers
    // Serial.print("Put param: ");
    // Serial.println(val1);
    if (val > get_max())
        val = get_max();
    if (val1 < get_min())
        val = get_min();
    // Serial.print("final val: ");
    // Serial.println(val);
    if (check_params)
        val = check_param(val);
    params.put(val, param_num);
    printParams();
}

void SPANK_fxn::hilight_param()
{
    if (enable_hilight)
    {
        uint8_t dig_offset = get_label(param_num).length();
        int char_num = digit_num + dig_offset;
        (*ui).underline_char(char_num, min(2, param_num + display_offset), 1, 8);
        // Serial.print ("Dig offset: "+String(dig_offset));
        // Serial.print (" Digit #: "+String(digit_num));
        // Serial.println(" Char Num: "+String(char_num));
    }
}

void SPANK_fxn::inc_param_num_by(int val)
{
    param_num += val;
    if (param_num < 0)
        param_num = num_params;
    if (param_num > num_params - 1)
        param_num = 0;
    digit_num = 0;
    printParams();
}

void SPANK_fxn::inc_dig_num_by(int val)
{
    // uint8_t num_digs = log10(get_max(param_num))+1;
    uint16_t num_digs = get_num_digits(param_num);
    digit_num += val;
    if (digit_num < 0)
        digit_num = 0;
    if (digit_num > num_digs - 1)
        digit_num = 0;
    // Serial.println("Dig# "+String(digit_num)+" Digs "+String(num_digs));
    printParams();
}

String SPANK_fxn::get_label(int label_num)
{
    return labels[label_num];
}

void SPANK_fxn::printParams()
{
    (*ui).fill(BLACK, 16 + (16 * display_offset));
    (*ui).t.clrDown(display_offset == 1 ? "10" : "8");
    hilight_param();
    // Serial.println("Num params: "+String(num_params) + " Param# "+String(param_num));
    int start = max(0, param_num - (2 - display_offset));
    int end = min(3 - display_offset, num_params);
    for (int i = 0; i < end; i++)
    {
        print_param(i + start, i + display_offset);
        // uint8_t ndigs = get_num_digits(i+start);
        // String fmt = calc_format(ndigs);
        // const char* format = fmt.c_str();
        // (*ui).printParam(get_label(i+start),get_param(i+start),ndigs,format,i);
    }
}

String SPANK_fxn::toJSON(String key, String value)
{
    return "\"" + key + "\" :\"" + value + "\"";
}

String SPANK_fxn::params_toJSON()
{
    String out = "";
    boolean is_string = false;
    String s;
    for (int i = 0; i < num_params; i++)
    {
        if (out > "")
            out += ",";
        out += "{ ";
        out += toJSON("label", get_label(i));
        out += ", ";

        if (string_params)
        {
            if (string_params[i].length())
            {
                is_string = true;
            }
        }

        if (is_string)
        {
            out += toJSON("type", "select");
            out += ", ";
            out += toJSON("values", string_params[i]);
            out += ", ";
            out += toJSON("value", csv_elem(string_params[i], ',', get_param(i)));
        }
        else
        {
            out += toJSON("type", "number");
            out += ", ";
            out += toJSON("value", String(get_param(i)));
        }

        out += ", ";
        out += toJSON("param_num", String(i));
        out += ", ";
        out += toJSON("min", String(get_min(i)));
        out += ", ";
        out += toJSON("max", String(get_max(i)));
        out += ", ";
        out += toJSON("num_digits", String(get_num_digits(i)));
        out += ", ";
        out += toJSON("selected", enable_hilight && param_num == i ? "true" : "false");
        out += " }";
    }
    return "[" + out + "]";
}

void SPANK_fxn::default_display()
{
    //   Serial.println("Name: "+name);
    //   (*ui).clearDisplay();
    //   (*ui).printLine(name,0,2);
    (*ui).newFxn(name);
    printParams();
}

void SPANK_fxn::display()
{
    (this->*display_fxn)();
}

void SPANK_fxn::copy_to(SPANK_fxn &target)
{
    for (int i = 0; i < 5; i++)
    {
        target.params.put(0, i);
        target.labels[i] = "";
        target.mins[i] = 0;
        target.maxs[i] = 0;
    }
    for (int i = 0; i < num_params; i++)
    {
        target.params.put(get_param(i), i);
        target.labels[i] = get_label(i);
        target.mins[i] = get_min(i);
        target.maxs[i] = get_max(i);
    }
    target.name = name;
    target.trigger_fxn = trigger_fxn;
    target.num_params = num_params;
    target.string_params = string_params;
    //Serial.println("Copying: " + target.name + " Num params: " + String(target.num_params));
}

void SPANK_fxn::init()
{
    //(*ui).terminal_debug("Initializing: " + name);
    Serial.println("\nInitializing: " + name);
    param_num = 0;
    digit_num = 0;
    for (int i = 0; i < num_params; i++)
    {
        Serial.println(get_label(i) + String(init_vals[i]));
        params.put(init_vals[i], i);
    }
}

void SPANK_fxn::debug()
{
    Serial.println("Howdy I'm " + name);
    Serial.println("Initial Delay: " + String(get_param(INITIAL_DELAY)));
}

// private fxns
uint8_t SPANK_fxn::get_num_digits(int this_param_num)
{
    // Serial.println("Getting ndigs: "+String(this_param_num));
    return log10(get_max(this_param_num)) + 1;
}

String SPANK_fxn::calc_format(uint8_t ndigs)
{
    // uint8_t ndigs = get_num_digits(indx);
    return "%0" + String(ndigs) + "d";
    // const char* format = fmt.c_str();
    // return format;
}
