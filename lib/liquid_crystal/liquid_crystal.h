#ifndef __LIQUID_CRYSTAL_H__
#define __LIQUID_CRYSTAL_H__

#include <stdint.h>

typedef struct {
    uint8_t displayfunction;
    uint8_t displaycontrol;
    uint8_t displaymode;
    uint8_t numlines;
    uint8_t x_max;
    uint8_t y_max;
    uint8_t backlightval;
    void(*delay_ms)(uint32_t);
    int(*write)(uint8_t data);
} liquid_crystal_t;

void LCD_write(liquid_crystal_t* lcd, uint8_t value);
void LCD_init(liquid_crystal_t* lcd, void* delay_ms,
              void* write, uint8_t lcd_cols, uint8_t lcd_rows);
void LCD_clear(liquid_crystal_t* lcd);
void LCD_home(liquid_crystal_t* lcd);
void LCD_display_off(liquid_crystal_t* lcd);
void LCD_display_on(liquid_crystal_t* lcd);
void LCD_blink_off(liquid_crystal_t* lcd);
void LCD_blink_on(liquid_crystal_t* lcd);
void LCD_cursor_off(liquid_crystal_t* lcd);
void LCD_cursor_on(liquid_crystal_t* lcd);
void LCD_scroll_display_left(liquid_crystal_t* lcd);
void LCD_scroll_display_right(liquid_crystal_t* lcd);
void LCD_left_to_right(liquid_crystal_t* lcd);
void LCD_right_to_left(liquid_crystal_t* lcd);
void LCD_backlight_off(liquid_crystal_t* lcd);
void LCD_backlight_on(liquid_crystal_t* lcd);
void LCD_autoscroll_on(liquid_crystal_t* lcd);
void LCD_autoscroll_off(liquid_crystal_t* lcd);
void LCD_create_char(liquid_crystal_t* lcd, uint8_t location, uint8_t charmap[]);
void LCD_set_cursor(liquid_crystal_t* lcd, uint8_t col, uint8_t row);
void LCD_write_str(liquid_crystal_t* lcd, char* str);
void LCD_command(liquid_crystal_t* lcd, uint8_t value);
void LCD_load_custom_character(liquid_crystal_t* lcd, uint8_t char_num, uint8_t *rows);    // alias for createChar()

#endif
