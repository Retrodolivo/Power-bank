#include "oled_ssd1306.h"

static void oled_char6x8(uint8_t c);
//static void oled_char8x16(uint8_t c);
static void ssd1306_write(uint8_t ctrl_byte, uint8_t data_byte);
static uint8_t oled_x,
			   oled_y;

static void ssd1306_write(uint8_t ctrl_byte, uint8_t data_byte)
{
	i2c_start();
	i2c_sendbyte(SSD1306_I2C_ADDR);
	i2c_sendbyte(ctrl_byte);
	i2c_sendbyte(data_byte);
	i2c_stop();
}

void oled_init(void)
{
	oled_off();
	ssd1306_write(CMD, SSD1306_SETMULTIPLEX);
	ssd1306_write(CMD, 0x3F);							//Set rows
	ssd1306_write(CMD, SSD1306_SETDISPLAYOFFSET);
	ssd1306_write(CMD, 0x00);							//No offset
	ssd1306_write(CMD, SSD1306_SETSTARTLINE | 0x00);
	// Horizontal memory mode
	ssd1306_write(CMD, SSD1306_MEMORYMODE);
	ssd1306_write(CMD, 0x00);
	ssd1306_write(CMD, SSD1306_SEGREMAP | 0x1);
	ssd1306_write(CMD, SSD1306_COMSCANDEC);
	ssd1306_write(CMD, SSD1306_SETCOMPINS);
	ssd1306_write(CMD, 0x12);
	// Max contrast
	ssd1306_write(CMD, SSD1306_SETCONTRAST);
	ssd1306_write(CMD, 0xCC);
	ssd1306_write(CMD, SSD1306_DISPLAYALLON_RESUME);
	ssd1306_write(CMD, SSD1306_NORMALDISPLAY);
	ssd1306_write(CMD, SSD1306_SETDISPLAYCLOCKDIV);
	ssd1306_write(CMD, 0x80);							//Set freq
	// We use internal charge pump
	ssd1306_write(CMD, SSD1306_CHARGEPUMP);
	ssd1306_write(CMD, 0x14);
	oled_on();
	oled_clear();
	oled_goto(0, 0);
}

void oled_on(void)
{
	ssd1306_write(CMD, SSD1306_DISPLAYON);
}

void oled_off(void)
{
	ssd1306_write(CMD, SSD1306_DISPLAYOFF);
}

void oled_clear(void)
{
	uint16_t x = 0,
	y = 0;
	oled_goto(0, 0);
	for(uint16_t i = 0; i < (SSD1306_OLEDWIDTH * SSD1306_OLEDHEIGHT / 8); i++)
	{
		oled_char6x8(' ');
		x++;
		if(x > SSD1306_OLEDWIDTH)
		{
			x = 0;
			y++;
			oled_goto(0, y);
		}
	}
	oled_x = SSD1306_DEFAULT_SPACE;
	oled_y = 0;
}

void oled_clear_line_seg(uint8_t x1, uint8_t x2, uint8_t y)
{
	oled_goto(x1, y);
	uint8_t lenght = x2 - x1;
	
	for(uint16_t i = 0; i < lenght; i++)
	{
		oled_char6x8(' ');
		x1++;
		oled_goto(x1, y);
			
	}
}

void oled_goto(uint8_t x, uint8_t y)
{
	oled_x = x;
	oled_y = y;
	
	ssd1306_write(CMD, 0xB0 + y);		//установить адрес начала координат
	ssd1306_write(CMD, x & 0xF);		//установить нижний адрес столбца
	ssd1306_write(CMD, 0x10 | (x >> 4));//установить верхний адрес столбца
}

static void oled_char6x8(uint8_t c)
{
	i2c_start();
	i2c_sendbyte(SSD1306_I2C_ADDR);
	i2c_sendbyte(DATAS);
	for(uint8_t x = 0; x < 6; x++)
		i2c_sendbyte(oled_font6x8[(c - 32) * 6 + x]);
	i2c_sendbyte(0x00);					//пробел в одну точку между символами
	i2c_stop();

	oled_x += 8;
	if(oled_x > SSD1306_OLEDWIDTH)
		oled_x = SSD1306_DEFAULT_SPACE;
}

//static void oled_char8x16(uint8_t c)
//{
	//i2c_start();
	//i2c_sendbyte(SSD1306_I2C_ADDR);
	//i2c_sendbyte(DATAS);
	//for(uint8_t x = 0; x < 16; x++)
		//i2c_sendbyte(oled_font8x16[(c - 32) * 16 + x]);
	//i2c_sendbyte(0x00);					//пробел в одну точку между символами
	//i2c_stop();
//
	//oled_x += 16;
	//if(oled_x > SSD1306_OLEDWIDTH)
		//oled_x = SSD1306_DEFAULT_SPACE;
//}

void oled_print6x8(char *buff)
{
	while (*buff!=0)
	{
		if((oled_x > SSD1306_OLEDWIDTH) || (oled_x < 5))
			oled_x = SSD1306_DEFAULT_SPACE;
		oled_char6x8(*buff++);
	}
}

//void oled_print8x16(char *buff)
//{
	//while (*buff!=0)
	//{
		//if((oled_x > SSD1306_OLEDWIDTH) || (oled_x < 5))
			//oled_x = SSD1306_DEFAULT_SPACE;
		//oled_char8x16(*buff++);
	//}
//}