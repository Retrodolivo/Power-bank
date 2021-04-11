#ifndef OLED_SSD1306_H_
#define OLED_SSD1306_H_

#include "main.h"

#define SSD1306_I2C_ADDR	0x78
#define	DATAS				0b01000000		//transmitt few data bytes
#define	DATA				0b11000000		//transmitt single data byte
#define	CMD					0b10000000		//transmitt single cmd byte

#define SSD1306_I2C_ADDR					 0x78
// size
#define SSD1306_OLEDWIDTH                    128
#define SSD1306_OLEDHEIGHT                   64
#define SSD1306_DEFAULT_SPACE                5
// Commands
#define SSD1306_SETCONTRAST                  0x81
#define SSD1306_DISPLAYALLON_RESUME          0xA4
#define SSD1306_DISPLAYALLON                 0xA5
#define SSD1306_NORMALDISPLAY                0xA6
#define SSD1306_INVERTDISPLAY                0xA7
#define SSD1306_DISPLAYOFF                   0xAE
#define SSD1306_DISPLAYON                    0xAF
#define SSD1306_SETDISPLAYOFFSET             0xD3
#define SSD1306_SETCOMPINS                   0xDA
#define SSD1306_SETVCOMDETECT                0xDB
#define SSD1306_SETDISPLAYCLOCKDIV           0xD5
#define SSD1306_SETPRECHARGE                 0xD9
#define SSD1306_SETMULTIPLEX                 0xA8
#define SSD1306_SETLOWCOLUMN                 0x00
#define SSD1306_SETHIGHCOLUMN                0x10
#define SSD1306_SETSTARTLINE                 0x40
#define SSD1306_MEMORYMODE                   0x20
#define SSD1306_COLUMNADDR                   0x21
#define SSD1306_PAGEADDR                     0x22
#define SSD1306_COMSCANINC                   0xC0
#define SSD1306_COMSCANDEC                   0xC8
#define SSD1306_SEGREMAP                     0xA0
#define SSD1306_CHARGEPUMP                   0x8D
#define SSD1306_EXTERNALVCC                   0x1
#define SSD1306_SWITCHCAPVCC                  0x2
// Scrolling #defines
#define SSD1306_ACTIVATE_SCROLL               0x2F
#define SSD1306_DEACTIVATE_SCROLL             0x2E
#define SSD1306_SET_VERTICAL_SCROLL_AREA      0xA3
#define SSD1306_RIGHT_HORIZONTAL_SCROLL       0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL        0x27
#define SSD1306_VERT_AND_RIGHT_HORIZ_SCROLL   0x29
#define SSD1306_VERT_AND_LEFT_HORIZ_SCROLL    0x2A

void oled_init(void);
void oled_on(void);
void oled_off(void);
void oled_clear(void);
void oled_clear_line_seg(uint8_t x1, uint8_t x2, uint8_t y);
void oled_goto(uint8_t x, uint8_t y);
void oled_print6x8(char *buff);
void oled_print(uint8_t x, uint8_t y, char *buff);
void oled_print8x16(char *buff);


#endif /* OLED_SSD1306_H_ */