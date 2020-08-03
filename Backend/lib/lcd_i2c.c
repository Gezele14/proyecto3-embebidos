/*Adaptado de https://github.com/johnrickman/LiquidCrystal_I2C */
// Based on the work by DFRobot

#include <lcd_i2c.h>
#include <string.h>

// When the display powers up, it is configured as follows:
//
// 1. Display clear
// 2. Function set:
//    DL = 1; 8-bit interface data
//    N = 0; 1-line display
//    F = 0; 5x8 dot character font
// 3. Display on/off control:
//    D = 0; Display off
//    C = 0; Cursor off
//    B = 0; Blinking off
// 4. Entry mode set:
//    I/D = 1; Increment by 1
//    S = 0; No shift
//
// Note, however, that resetting the Arduino doesn't reset the LCD, so we
// can't assume that its in that state when a sketch starts (and the
// LiquidCrystal constructor is called).
uint8_t pattern(char ch);
void LiquidCrystal_I2C(uint8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows)
{
    _Addr = lcd_Addr;
    _cols = lcd_cols;
    _rows = lcd_rows;
    _backlightval = LCD_BACKLIGHT;
}

void init()
{
    _displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
    begin(_cols, _rows, LCD_5x8DOTS);
}

void begin(uint8_t cols, uint8_t lines, uint8_t dotsize)
{
    if (lines > 1)
    {
        _displayfunction |= LCD_2LINE;
    }
    _numlines = lines;

    // for some 1 line displays you can select a 10 pixel high font
    if ((dotsize != 0) && (lines == 1))
    {
        _displayfunction |= LCD_5x10DOTS;
    }

    // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
    // according to datasheet, we need at least 40ms after power rises above 2.7V
    // before sending commands. Arduino can turn on way befer 4.5V so we'll wait 50
    usleep(50000);

    // Now we pull both RS and R/W low to begin commands
    expanderWrite(_backlightval); // reset expanderand turn backlight off (Bit 8 =1)
    usleep(1000000);

    //put the LCD into 4 bit mode
    // this is according to the hitachi HD44780 datasheet
    // figure 24, pg 46

    // we start in 8bit mode, try to set 4 bit mode
    write4bits(0x03 << 4);
    usleep(4500); // wait min 4.1ms

    // second try
    write4bits(0x03 << 4);
    usleep(4500); // wait min 4.1ms

    // third go!
    write4bits(0x03 << 4);
    usleep(150);

    // finally, set to 4-bit interface
    write4bits(0x02 << 4);

    // set # lines, font size, etc.
    command(LCD_FUNCTIONSET | _displayfunction);

    // turn the display on with no cursor or blinking default
    _displaycontrol = LCD_DISPLAYON | LCD_CURSORON | LCD_BLINKON;
    display();

    clear();

    // Initialize to default text direction (for roman languages)
    _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

    // set the entry mode
    // command(LCD_ENTRYMODESET | _displaymode);
    command(0x06);
    usleep(50);
    //
    //_displaycontrol = LCD_DISPLAYON | LCD_CURSORON | LCD_BLINKON;
    //display();

    home();
}

/********** high level commands, for the user! */
void clear()
{
    command(LCD_CLEARDISPLAY); // clear display, set cursor position to zero
    usleep(2000);              // this command takes a long time!
}

void home()
{
    command(LCD_RETURNHOME); // set cursor position to zero
    usleep(3000);            // this command takes a long time!
}

void setCursor(uint8_t col, uint8_t row)
{
    int row_offsets[] = {0x00, 0x40, 0x14, 0x54};
    if (row > _numlines)
    {
        row = _numlines - 1; // we count rows starting w/0
    }
    command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

// Turn the display on/off (quickly)
void noDisplay()
{
    _displaycontrol &= ~LCD_DISPLAYON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void display()
{
    // _displaycontrol |= LCD_DISPLAYON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turns the underline cursor on/off
void noCursor()
{
    _displaycontrol &= ~LCD_CURSORON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void cursor()
{
    _displaycontrol |= LCD_CURSORON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turn on and off the blinking cursor
void noBlink()
{
    _displaycontrol &= ~LCD_BLINKON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void blink()
{
    _displaycontrol |= LCD_BLINKON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// These commands scroll the display without changing the RAM
void scrollDisplayLeft(void)
{
    command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void scrollDisplayRight(void)
{
    command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void leftToRight(void)
{
    _displaymode |= LCD_ENTRYLEFT;
    command(LCD_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void rightToLeft(void)
{
    _displaymode &= ~LCD_ENTRYLEFT;
    command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'right justify' text from the cursor
void autoscroll(void)
{
    _displaymode |= LCD_ENTRYSHIFTINCREMENT;
    command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void noAutoscroll(void)
{
    _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
    command(LCD_ENTRYMODESET | _displaymode);
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void createChar(uint8_t location, uint8_t charmap[])
{
    location &= 0x7; // we only have 8 locations 0-7
    command(LCD_SETCGRAMADDR | (location << 3));
    for (int i = 0; i < 8; i++)
    {
        //TODO
        // write(charmap[i]);
    }
}

//createChar with PROGMEM input
void createChar2(uint8_t location, const char *charmap)
{
    location &= 0x7; // we only have 8 locations 0-7
    command(LCD_SETCGRAMADDR | (location << 3));
    for (int i = 0; i < 8; i++)
    {
        //TODO
        //write(pgm_read_byte_near(charmap++));
    }
}

// Turn the (optional) backlight off/on
void noBacklight(void)
{
    _backlightval = LCD_NOBACKLIGHT;
    expanderWrite(0);
}

void backlight(void)
{
    _backlightval = LCD_BACKLIGHT;
    // command(LCD_DISPLAYCONTROL | LCD_BACKLIGHT);
}

/*********** mid level commands, for sending data/cmds */

inline void command(uint8_t value)
{
    lcd_send(value, 0);
}

/************ low level data pushing commands **********/

// write either command or data
void lcd_send(uint8_t value, uint8_t mode)
{
    uint8_t highnib = value & 0xf0;
    uint8_t lownib = (value << 4) & 0xf0;

    write4bits((highnib) | mode);
    write4bits((lownib) | mode);
}

void write4bits(uint8_t value)
{
    expanderWrite(value);
    pulseEnable(value);
}

void expanderWrite(uint8_t _data)
{
    //Wire.beginTransmission(_Addr);
    i2c_write((unsigned int)(_data) | _backlightval);
    //Wire.endTransmission();
}

void pulseEnable(uint8_t _data)
{
    expanderWrite(_data | En); // En high
    usleep(1);                 // enable pulse must be >450ns

    expanderWrite(_data & ~En); // En low
    usleep(50);                 // commands need > 37us to settle
}

// Alias functions

void cursor_on()
{
    cursor();
}

void cursor_off()
{
    noCursor();
}

void blink_on()
{
    blink();
}

void blink_off()
{
    noBlink();
}
/*
void load_custom_character(uint8_t char_num, uint8_t *rows)
{
    createChar(char_num, rows);
}*/

void setBacklight(uint8_t new_val)
{
    if (new_val)
    {
        backlight(); // turn backlight on
    }
    else
    {
        noBacklight(); // turn backlight off
    }
}

void printstr(const char c[])
{
    //This function is not identical to the function used for "real" I2C displays
    //it's here so the user sketch doesn't have to be changed
    //TODO: print(c);
}

void i2c_write_str(const char *str)
{
    size_t l;
    l = strlen(str);

    for (long i = 0; i < l; ++i)
    {
        lcd_send(pattern(str[i]), 0x01);
        usleep(40);
    }
}
uint8_t pattern(char ch)
{
    switch (ch)
    {
    case ' ':
        return 0x20;
    case '!':
        return 0x21;
    case '"':
        return 0x22;
    case '#':
        return 0x23;
    case '$':
        return 0x24;
    case '%':
        return 0x25;
    case '&':
        return 0x26;
    case '\'':
        return 0x27;
    case '(':
        return 0x28;
    case ')':
        return 0x29;
    case '*':
        return 0x2A;
    case '+':
        return 0x2B;
    case ',':
        return 0x2C;
    case '-':
        return 0x2D;
    case '.':
        return 0x2E;
    case '/':
        return 0x2F;
    case '0':
        return 0x30;
    case '1':
        return 0x31;
    case '2':
        return 0x32;
    case '3':
        return 0x33;
    case '4':
        return 0x34;
    case '5':
        return 0x35;
    case '6':
        return 0x36;
    case '7':
        return 0x37;
    case '8':
        return 0x38;
    case '9':
        return 0x39;
    case ':':
        return 0x3A;
    case ';':
        return 0x3B;
    case '<':
        return 0x3C;
    case '=':
        return 0x3D;
    case '>':
        return 0x3E;
    case '?':
        return 0x3F;
    case '@':
        return 0x40;
    case 'A':
        return 0x41;
    case 'B':
        return 0x42;
    case 'C':
        return 0x43;
    case 'D':
        return 0x44;
    case 'E':
        return 0x45;
    case 'F':
        return 0x46;
    case 'G':
        return 0x47;
    case 'H':
        return 0x48;
    case 'I':
        return 0x49;
    case 'J':
        return 0x4A;
    case 'K':
        return 0x4B;
    case 'L':
        return 0x4C;
    case 'M':
        return 0x4D;
    case 'N':
        return 0x4E;
    case 'O':
        return 0x4F;
    case 'P':
        return 0x50;
    case 'Q':
        return 0x51;
    case 'R':
        return 0x52;
    case 'S':
        return 0x53;
    case 'T':
        return 0x54;
    case 'U':
        return 0x55;
    case 'V':
        return 0x56;
    case 'W':
        return 0x57;
    case 'X':
        return 0x58;
    case 'Y':
        return 0x59;
    case 'Z':
        return 0x5A;
    case '[':
        return 0x5B;
    case '\\':
        return 0x5C;
    case ']':
        return 0x5D;
    case '^':
        return 0x5E;
    case '_':
        return 0x5F;
    case '`':
        return 0x60;
    case 'a':
        return 0x61;
    case 'b':
        return 0x62;
    case 'c':
        return 0x63;
    case 'd':
        return 0x64;
    case 'e':
        return 0x65;
    case 'f':
        return 0x66;
    case 'g':
        return 0x67;
    case 'h':
        return 0x68;
    case 'i':
        return 0x69;
    case 'j':
        return 0x6A;
    case 'k':
        return 0x6B;
    case 'l':
        return 0x6C;
    case 'm':
        return 0x6D;
    case 'n':
        return 0x6E;
    case 'o':
        return 0x6F;
    case 'p':
        return 0x70;
    case 'q':
        return 0x71;
    case 'r':
        return 0x72;
    case 's':
        return 0x73;
    case 't':
        return 0x74;
    case 'u':
        return 0x75;
    case 'v':
        return 0x76;
    case 'w':
        return 0x77;
    case 'x':
        return 0x78;
    case 'y':
        return 0x79;
    case 'z':
        return 0x7A;
    case '{':
        return 0x7B;
    case '|':
        return 0x7C;
    case '}':
        return 0x7D;
    default:
        return 0;
    }
}
