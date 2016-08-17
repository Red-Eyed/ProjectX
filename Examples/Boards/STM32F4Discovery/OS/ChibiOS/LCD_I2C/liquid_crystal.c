#include "liquid_crystal.h"

//YWROBOT
//last updated on 21/12/2011
//Tim Starling Fix the reset bug (Thanks Tim)
//wiki doc http://www.dfrobot.com/wiki/index.php?title=/TWI_LCD1602_Module_(SKU:_DFR0063)
//Support Forum: http://www.dfrobot.com/forum/
//Compatible with the Arduino IDE 1.0
//Library version:1.1

void LCD_write(liquid_crystal_t* lcd, uint8_t value) {
    LCD_send(lcd, value, Rs);
}

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

void LCD_init(liquid_crystal_t* lcd, void* delay_ms, void*delay_us,
              void* write, uint8_t lcd_cols, uint8_t lcd_rows){
    lcd->cols = lcd_cols;
    lcd->rows = lcd_rows;
    lcd->backlightval = LCD_NOBACKLIGHT;
    lcd->delay_ms = delay_ms;
    lcd->delay_us = delay_us;
    lcd->write = write;
    lcd->displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;

    LCD_begin(lcd, lcd_cols, lcd_rows);
}

void LCD_begin(liquid_crystal_t* lcd, uint8_t cols, uint8_t lines) {
    if (lines > 1) {
        lcd->displayfunction |= LCD_2LINE;
    }
    lcd->numlines = lines;
    lcd->cols = cols;

    // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
    // according to datasheet, we need at least 40ms after power rises above 2.7V
    // before sending commands. Arduino can turn on way befer 4.5V so we'll wait 50
    lcd->delay_us(50);

    // Now we pull both RS and R/W low to begin commands
    LCD_expanderWrite(lcd, lcd->backlightval);// reset expanderand turn backlight off (Bit 8 =1)
    lcd->delay_us(1000);

    //put the LCD into 4 bit mode
    // this is according to the hitachi HD44780 datasheet
    // figure 24, pg 46

    // we start in 8bit mode, try to set 4 bit mode
    LCD_write4bits(lcd, 0x03 << 4);
    lcd->delay_ms(4500); // wait min 4.1ms

    // second try
    LCD_write4bits(lcd, 0x03 << 4);
    lcd->delay_ms(4500); // wait min 4.1ms

    // third go!
    LCD_write4bits(lcd, 0x03 << 4);
    lcd->delay_ms(150);

    // finally, set to 4-bit interface
    LCD_write4bits(lcd, 0x02 << 4);

    // set # lines, font size, etc.
    LCD_command(lcd, LCD_FUNCTIONSET | lcd->displayfunction);

    // turn the display on with no cursor or blinking default
    lcd->displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    LCD_display(lcd);

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
    lcd->delay_ms(3000);  // this command takes a long time!
}

void LCD_setCursor(liquid_crystal_t* lcd, uint8_t col, uint8_t row) {
    int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
    if (row > lcd->numlines) {
        row = lcd->numlines - 1;    // we count rows starting w/0
    }
    LCD_command(lcd, LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

// Turn the display on/off (quickly)
void LCD_noDisplay(liquid_crystal_t* lcd) {
    lcd->displaycontrol &= ~LCD_DISPLAYON;
    LCD_command(lcd, LCD_DISPLAYCONTROL | lcd->displaycontrol);
}

void LCD_display(liquid_crystal_t* lcd) {
    lcd->displaycontrol |= LCD_DISPLAYON;
    LCD_command(lcd, LCD_DISPLAYCONTROL | lcd->displaycontrol);
}

// Turns the underline cursor on/off
void LCD_noCursor(liquid_crystal_t* lcd) {
    lcd->displaycontrol &= ~LCD_CURSORON;
    LCD_command(lcd, LCD_DISPLAYCONTROL | lcd->displaycontrol);
}
void LCD_cursor(liquid_crystal_t* lcd) {
    lcd->displaycontrol |= LCD_CURSORON;
    LCD_command(lcd, LCD_DISPLAYCONTROL | lcd->displaycontrol);
}

// Turn on and off the blinking cursor
void LCD_noBlink(liquid_crystal_t* lcd) {
    lcd->displaycontrol &= ~LCD_BLINKON;
    LCD_command(lcd, LCD_DISPLAYCONTROL | lcd->displaycontrol);
}

void LCD_blink(liquid_crystal_t* lcd) {
    lcd->displaycontrol |= LCD_BLINKON;
    LCD_command(lcd, LCD_DISPLAYCONTROL | lcd->displaycontrol);
}

// These commands scroll the display without changing the RAM
void LCD_scrollDisplayLeft(liquid_crystal_t* lcd) {
    LCD_command(lcd, LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void LCD_scrollDisplayRight(liquid_crystal_t* lcd) {
    LCD_command(lcd, LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void LCD_leftToRight(liquid_crystal_t* lcd) {
    lcd->displaymode |= LCD_ENTRYLEFT;
    LCD_command(lcd, LCD_ENTRYMODESET | lcd->displaymode);
}

// This is for text that flows Right to Left
void LCD_rightToLeft(liquid_crystal_t* lcd) {
    lcd->displaymode &= ~LCD_ENTRYLEFT;
    LCD_command(lcd, LCD_ENTRYMODESET | lcd->displaymode);
}

// This will 'right justify' text from the cursor
void LCD_autoscroll(liquid_crystal_t* lcd) {
    lcd->displaymode |= LCD_ENTRYSHIFTINCREMENT;
    LCD_command(lcd, LCD_ENTRYMODESET | lcd->displaymode);
}

// This will 'left justify' text from the cursor
void LCD_noAutoscroll(liquid_crystal_t* lcd) {
    lcd->displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
    LCD_command(lcd, LCD_ENTRYMODESET | lcd->displaymode);
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void LCD_createChar(liquid_crystal_t* lcd, uint8_t location, uint8_t charmap[]) {
    location &= 0x7; // we only have 8 locations 0-7
    LCD_command(lcd, LCD_SETCGRAMADDR | (location << 3));
    int i;
    for (i = 0; i < 8; i++) {
        LCD_write(lcd, charmap[i]);
    }
}

// Turn the (optional) backlight off/on
void LCD_noBacklight(liquid_crystal_t* lcd) {
    lcd->backlightval = LCD_NOBACKLIGHT;
    LCD_expanderWrite(lcd, 0);
}

void LCD_backlight(liquid_crystal_t* lcd) {
    lcd->backlightval = LCD_BACKLIGHT;
    LCD_expanderWrite(lcd, 0);
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
    LCD_expanderWrite(lcd, value);
    LCD_pulseEnable(lcd, value);
}

void LCD_expanderWrite(liquid_crystal_t* lcd, uint8_t data) {
    lcd->write(data | lcd->backlightval);
}

void LCD_pulseEnable(liquid_crystal_t* lcd, uint8_t _data) {
    LCD_expanderWrite(lcd,_data | En);// En high
    lcd->delay_us(450);// enable pulse must be >450ns

    LCD_expanderWrite(lcd,_data & ~En);// En low
    lcd->delay_us(37);// commands need > 37us to settle
}

// Alias functions

void LCD_cursor_on(liquid_crystal_t* lcd) {
    LCD_cursor(lcd);
}

void LCD_cursor_off(liquid_crystal_t* lcd) {
    LCD_noCursor(lcd);
}

void LCD_blink_on(liquid_crystal_t* lcd) {
    LCD_blink(lcd);
}

void LCD_blink_off(liquid_crystal_t* lcd) {
    LCD_noBlink(lcd);
}

void LCD_load_custom_character(liquid_crystal_t* lcd, uint8_t char_num, uint8_t *rows) {
    LCD_createChar(lcd, char_num, rows);
}

void LCD_setBacklight(liquid_crystal_t* lcd, uint8_t new_val) {
    if (new_val) {
        LCD_backlight(lcd);// turn backlight on
    } else {
        LCD_noBacklight(lcd);// turn backlight off
    }
}

//Функция передачи строки через USART
void LCD_write_String(liquid_crystal_t* lcd, char* str) {
    uint8_t i = 0;
    while (str[i]) {
        LCD_write(lcd, str[i]);
        i++;
    }
}

