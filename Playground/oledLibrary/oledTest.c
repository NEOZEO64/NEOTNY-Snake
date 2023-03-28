#ifndef LCD_H
#define LCD_H

#ifdef __cplusplus
extern "C" {
#endif
    
#if (__GNUC__ * 100 + __GNUC_MINOR__) < 303
#error "This library requires AVR-GCC 3.3 or later, update to newer AVR-GCC compiler !"
#endif
    
#include <inttypes.h>
#include <avr/pgmspace.h>

	/* TODO: define bus */
#define I2C			// I2C or SPI	
    /* TODO: define displaycontroller */
#define SH1106                 // or SSD1306, check datasheet of your display
    /* TODO: define displaymode */
#define TEXTMODE                // TEXTMODE for only text to display,
    // GRAPHICMODE for text and graphic
    /* TODO: define font */
#define FONT            ssd1306oled_font// set font here, refer font-name at font.h/font.c
    
    /* TODO: define I2C-adress for display */
    
    // using 7-bit-adress for lcd-library
    // if you use your own library for twi check I2C-adress-handle
#define LCD_I2C_ADR         (0x78 >> 1)    // 7 bit slave-adress without r/w-bit
    // r/w-bit are set/unset by library
    // e.g. 8 bit slave-adress:
    // 0x78 = adress 0x3C with cleared r/w-bit (write-mode)

    
#ifdef I2C
#include "i2c.h"	// library for I2C-communication
    // if you want to use other lib for I2C
    // edit i2c_xxx commands in this library
    // i2c_start(), i2c_byte(), i2c_stop()
    
#elif defined SPI
	// if you want to use your other lib/function for SPI replace SPI-commands
#define LCD_PORT	PORTB
#define LCD_DDR		DDRB
#define RES_PIN		PB0
#define DC_PIN		PB1
#define CS_PIN		PB2

#endif

#ifndef YES
#define YES        1
#endif

#define NORMALSIZE 1
#define DOUBLESIZE 2
    
#define LCD_DISP_OFF        0xAE
#define LCD_DISP_ON        0xAF
    
#define WHITE            0x01
#define BLACK            0x00
    
#define DISPLAY_WIDTH        128
#define DISPLAY_HEIGHT        64


int main(void){
  lcd_init(LCD_DISP_ON);    // init lcd and turn on
  lcd_puts("Hello World");  // put string from RAM to display (TEXTMODE) or buffer (GRAPHICMODE)
  lcd_gotoxy(0,2);          // set cursor to first column at line 3
  lcd_puts_p(PSTR("String from flash"));  // puts string form flash to display (TEXTMODE) or buffer (GRAPHICMODE)
#if defined GRAPHICMODE
  lcd_drawCircle(64,32,7,WHITE); // draw circle to buffer
  lcd_display();                  // send buffer to display
#endif
  for(;;){
    //main loop
  }
  return 0;
}
