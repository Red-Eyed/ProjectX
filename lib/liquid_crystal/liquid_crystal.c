#include "liquid_crystal.h"

// commands
#define LCD_CLEARDISPLAY        0x01
#define LCD_RETURNHOME          0x02
#define LCD_ENTRYMODESET        0x04
#define LCD_DISPLAYCONTROL      0x08
#define LCD_CURSORSHIFT         0x10
#define LCD_FUNCTIONSET         0x20
#define LCD_SETCGRAMADDR        0x40
#define LCD_SETDDRAMADDR        0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT          0x00
#define LCD_ENTRYLEFT           0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON           0x04
#define LCD_DISPLAYOFF          0x00
#define LCD_CURSORON            0x02
#define LCD_CURSOROFF           0x00
#define LCD_BLINKON             0x01
#define LCD_BLINKOFF            0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE         0x08
#define LCD_CURSORMOVE          0x00
#define LCD_MOVERIGHT           0x04
#define LCD_MOVELEFT            0x00

// flags for function set
#define LCD_8BITMODE            0x10
#define LCD_4BITMODE            0x00
#define LCD_2LINE               0x08
#define LCD_1LINE               0x00
#define LCD_5x10DOTS            0x04
#define LCD_5x8DOTS             0x00

// flags for backlight control
#define LCD_BACKLIGHT           0x08
#define LCD_NOBACKLIGHT         0x00

#define En 0x04  // Enable bit
#define Rw 0x02  // Read/Write bit
#define Rs 0x01  // Register select bit

void LCD_send(liquid_crystal_t* lcd, uint8_t, uint8_t);
void LCD_write4bits(liquid_crystal_t* lcd, uint8_t);
void LCD_expander_write(liquid_crystal_t* lcd, uint8_t);
void LCD_pulse_on(liquid_crystal_t* lcd, uint8_t);
void LCD_begin(liquid_crystal_t* lcd, uint8_t cols, uint8_t rows);

void LCD_write(liquid_crystal_t* lcd, uint8_t value) {
    LCD_send(lcd, value, Rs);
}

void LCD_init(liquid_crystal_t* lcd, void* delay_ms,
              void* write, uint8_t x_max, uint8_t y_max){
    lcd->x_max = x_max;
    lcd->y_max = y_max;
    lcd->backlightval = LCD_NOBACKLIGHT;
    lcd->delay_ms = delay_ms;
    lcd->write = write;
    lcd->displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;

    LCD_begin(lcd, x_max, y_max);
}

void LCD_begin(liquid_crystal_t* lcd, uint8_t cols, uint8_t lines) {
    if (lines > 1) {
        lcd->displayfunction |= LCD_2LINE;
    }
    lcd->numlines = lines;
    lcd->x_max = cols;

    // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
    // according to datasheet, we need at least 40ms after power rises above 2.7V
    // before sending commands
    lcd->delay_ms(40);

    // Now we pull both RS and R/W low to begin commands
    LCD_expander_write(lcd, lcd->backlightval);// reset expanderand turn backlight off (Bit 8 =1)
    lcd->delay_ms(1);

    // put the LCD into 4 bit mode
    // this is according to the hitachi HD44780 datasheet
    // figure 24, pg 46

    // we start in 8bit mode, try to set 4 bit mode
    LCD_write4bits(lcd, 0x03 << 4);
    lcd->delay_ms(5); // wait min 4.1ms

    // second try
    LCD_write4bits(lcd, 0x03 << 4);
    lcd->delay_ms(5); // wait min 4.1ms

    // third go!
    LCD_write4bits(lcd, 0x03 << 4);
    lcd->delay_ms(150);

    // finally, set to 4-bit interface
    LCD_write4bits(lcd, 0x02 << 4);

    // set # lines, font size, etc.
    LCD_command(lcd, LCD_FUNCTIONSET | lcd->displayfunction);

    // turn the display on with no cursor or blinking default
    lcd->displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    LCD_display_on(lcd);

    // clear it off
    LCD_clear(lcd);

    // Initialize to default text direction (for roman languages)
    lcd->displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

    // set the entry mode
    LCD_command(lcd, LCD_ENTRYMODESET | lcd->displaymode);

    LCD_home(lcd);
}

/********** high level commands, for the user! */
void LCD_clear(liquid_crystal_t* lcd) {
    LCD_command(lcd, LCD_CLEARDISPLAY); // clear display, set cursor position to zero
    lcd->delay_ms(300);  // this command takes a long time!
}

void LCD_home(liquid_crystal_t* lcd) {
    LCD_command(lcd, LCD_RETURNHOME);  // set cursor position to zero
    lcd->delay_ms(30);  // this command takes a long time!
}

void LCD_set_cursor(liquid_crystal_t* lcd, uint8_t x_pos, uint8_t y_pos) {
    int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
    if (y_pos > lcd->numlines) {
        y_pos = lcd->numlines - 1;    // we count rows starting w/0
    }
    LCD_command(lcd, LCD_SETDDRAMADDR | (x_pos + row_offsets[y_pos]));
}

// Turn the display on/off (quickly)
void LCD_display_off(liquid_crystal_t* lcd) {
    lcd->displaycontrol &= ~LCD_DISPLAYON;
    LCD_command(lcd, LCD_DISPLAYCONTROL | lcd->displaycontrol);
}

void LCD_display_on(liquid_crystal_t* lcd) {
    lcd->displaycontrol |= LCD_DISPLAYON;
    LCD_command(lcd, LCD_DISPLAYCONTROL | lcd->displaycontrol);
}

// Turns the underline cursor on/off
void LCD_cursor_off(liquid_crystal_t* lcd) {
    lcd->displaycontrol &= ~LCD_CURSORON;
    LCD_command(lcd, LCD_DISPLAYCONTROL | lcd->displaycontrol);
}
void LCD_cursor_on(liquid_crystal_t* lcd) {
    lcd->displaycontrol |= LCD_CURSORON;
    LCD_command(lcd, LCD_DISPLAYCONTROL | lcd->displaycontrol);
}

// Turn on and off the blinking cursor
void LCD_blink_off(liquid_crystal_t* lcd) {
    lcd->displaycontrol &= ~LCD_BLINKON;
    LCD_command(lcd, LCD_DISPLAYCONTROL | lcd->displaycontrol);
}

void LCD_blink_on(liquid_crystal_t* lcd) {
    lcd->displaycontrol |= LCD_BLINKON;
    LCD_command(lcd, LCD_DISPLAYCONTROL | lcd->displaycontrol);
}

// These commands scroll the display without changing the RAM
void LCD_scroll_display_left(liquid_crystal_t* lcd) {
    LCD_command(lcd, LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void LCD_scroll_display_right(liquid_crystal_t* lcd) {
    LCD_command(lcd, LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void LCD_left_to_right(liquid_crystal_t* lcd) {
    lcd->displaymode |= LCD_ENTRYLEFT;
    LCD_command(lcd, LCD_ENTRYMODESET | lcd->displaymode);
}

// This is for text that flows Right to Left
void LCD_right_to_left(liquid_crystal_t* lcd) {
    lcd->displaymode &= ~LCD_ENTRYLEFT;
    LCD_command(lcd, LCD_ENTRYMODESET | lcd->displaymode);
}

// This will 'right justify' text from the cursor
void LCD_autoscroll_on(liquid_crystal_t* lcd) {
    lcd->displaymode |= LCD_ENTRYSHIFTINCREMENT;
    LCD_command(lcd, LCD_ENTRYMODESET | lcd->displaymode);
}

// This will 'left justify' text from the cursor
void LCD_autoscroll_off(liquid_crystal_t* lcd) {
    lcd->displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
    LCD_command(lcd, LCD_ENTRYMODESET | lcd->displaymode);
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void LCD_create_char(liquid_crystal_t* lcd, uint8_t location, uint8_t charmap[]) {
    location &= 0x7; // we only have 8 locations 0-7
    LCD_command(lcd, LCD_SETCGRAMADDR | (location << 3));
    int i;
    for (i = 0; i < 8; i++) {
        LCD_write(lcd, charmap[i]);
    }
}

// Turn the (optional) backlight off/on
void LCD_backlight_off(liquid_crystal_t* lcd) {
    lcd->backlightval = LCD_NOBACKLIGHT;
    LCD_expander_write(lcd, 0);
}

void LCD_backlight_on(liquid_crystal_t* lcd) {
    lcd->backlightval = LCD_BACKLIGHT;
    LCD_expander_write(lcd, 0);
}

/*********** mid level commands, for sending data/cmds */

void LCD_command(liquid_crystal_t* lcd, uint8_t value) {
    LCD_send(lcd, value, 0);
}

/************ low level data pushing commands **********/

// write either command or data
void LCD_send(liquid_crystal_t* lcd, uint8_t value, uint8_t mode) {
    uint8_t highnib = value & 0xf0;
    uint8_t lownib = (value << 4) & 0xf0;
    LCD_write4bits(lcd, (highnib) | mode);
    LCD_write4bits(lcd, (lownib) | mode);
}

void LCD_write4bits(liquid_crystal_t* lcd, uint8_t value) {
    LCD_expander_write(lcd, value);
    LCD_pulse_on(lcd, value);
}

void LCD_expander_write(liquid_crystal_t* lcd, uint8_t data) {
    lcd->write(data | lcd->backlightval);
}

void LCD_pulse_on(liquid_crystal_t* lcd, uint8_t _data) {
    LCD_expander_write(lcd,_data | En);// En high
    lcd->delay_ms(1);// enable pulse must be >450ns

    LCD_expander_write(lcd,_data & ~En);// En low
    lcd->delay_ms(1);// commands need > 37us to settle
}

void LCD_load_custom_character(liquid_crystal_t* lcd, uint8_t char_num, uint8_t *rows) {
    LCD_create_char(lcd, char_num, rows);
}

void LCD_write_str(liquid_crystal_t* lcd, char* str) {
    while (*str) {
        LCD_write(lcd, *str++);
    }
}

