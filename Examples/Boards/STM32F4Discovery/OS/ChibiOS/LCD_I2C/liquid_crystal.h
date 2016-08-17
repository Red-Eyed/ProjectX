//YWROBOT
#ifndef LiquidCrystal__h

#include <stdint.h>

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

typedef struct {
    uint8_t displayfunction;
    uint8_t displaycontrol;
    uint8_t displaymode;
    uint8_t numlines;
    uint8_t cols;
    uint8_t rows;
    uint8_t backlightval;
    void(*delay_ms)(uint32_t);
    void(*delay_us)(uint32_t);
    int(*write)(uint8_t data);
} liquid_crystal_t;

void LCD_write(liquid_crystal_t* lcd, uint8_t value);
void LCD_init(liquid_crystal_t* lcd, void* delay_ms, void*delay_us,
              void* write, uint8_t lcd_cols, uint8_t lcd_rows);
void LCD_begin(liquid_crystal_t* lcd, uint8_t cols, uint8_t rows);
void LCD_clear(liquid_crystal_t* lcd);
void LCD_home(liquid_crystal_t* lcd);
void LCD_noDisplay(liquid_crystal_t* lcd);
void LCD_display(liquid_crystal_t* lcd);
void LCD_noBlink(liquid_crystal_t* lcd);
void LCD_blink(liquid_crystal_t* lcd);
void LCD_noCursor(liquid_crystal_t* lcd);
void LCD_cursor(liquid_crystal_t* lcd);
void LCD_scrollDisplayLeft(liquid_crystal_t* lcd);
void LCD_scrollDisplayRight(liquid_crystal_t* lcd);
void LCD_printLeft(liquid_crystal_t* lcd);
void LCD_printRight(liquid_crystal_t* lcd);
void LCD_leftToRight(liquid_crystal_t* lcd);
void LCD_rightToLeft(liquid_crystal_t* lcd);
void LCD_shiftIncrement(liquid_crystal_t* lcd);
void LCD_shiftDecrement(liquid_crystal_t* lcd);
// Turn the (optional) backlight off/on
void LCD_noBacklight(liquid_crystal_t* lcd);
void LCD_backlight(liquid_crystal_t* lcd);
void LCD_autoscroll(liquid_crystal_t* lcd);
void LCD_noAutoscroll(liquid_crystal_t* lcd);
void LCD_createChar(liquid_crystal_t* lcd, uint8_t location, uint8_t charmap[]);
void LCD_setCursor(liquid_crystal_t* lcd, uint8_t col, uint8_t row);
void LCD_write_String(liquid_crystal_t* lcd, char* str);
void LCD_command(liquid_crystal_t* lcd, uint8_t value);

////compatibility API function aliases
void LCD_blink_on(liquid_crystal_t* lcd);                                                  // alias for blink()
void LCD_blink_off(liquid_crystal_t* lcd);                                                 // alias for noBlink()
void LCD_cursor_on(liquid_crystal_t* lcd);                                                 // alias for cursor()
void LCD_cursor_off(liquid_crystal_t* lcd);                                                // alias for noCursor()
void LCD_setBacklight(liquid_crystal_t* lcd, uint8_t new_val);                             // alias for backlight() and nobacklight()
void LCD_load_custom_character(liquid_crystal_t* lcd, uint8_t char_num, uint8_t *rows);    // alias for createChar()
void LCD_printstr(const char[]);

void LCD_send(liquid_crystal_t* lcd, uint8_t, uint8_t);
void LCD_write4bits(liquid_crystal_t* lcd, uint8_t);
void LCD_expanderWrite(liquid_crystal_t* lcd, uint8_t);
void LCD_pulseEnable(liquid_crystal_t* lcd, uint8_t);

#endif
