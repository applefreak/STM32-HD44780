#include "stm32f10x.h"
#include "hd44780.h"

#define LCD_Port GPIOA
#define RS GPIO_Pin_0
#define EN GPIO_Pin_1
#define D4 GPIO_Pin_2
#define D5 GPIO_Pin_3
#define D6 GPIO_Pin_4
#define D7 GPIO_Pin_5

void strobeEN(void);
void upNib(uint8_t c);
void downNib(uint8_t c);
void Delay(uint32_t nCount);

void lcdInit(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	//Init GPIOs
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin   = EN | RS | D4 | D5 | D6 | D7; 
	GPIO_ResetBits(LCD_Port, EN | RS | D4 | D5 | D6 | D7);
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(LCD_Port, &GPIO_InitStructure);
	GPIO_ResetBits(LCD_Port, EN | RS | D4 | D5 | D6 | D7);
	Delay(0xffff);
	sendCMD(0x02);
	Delay(0x3FFFC);  //wait 20ms
	sendCMD(0x28);  //LCD configs
	sendCMD(0x06);
	sendCMD(0x01);
	sendCMD(0x0E);
	Delay(0xffff);
}

void strobeEN(void) {
	Delay(0xffff);
	GPIO_SetBits(LCD_Port, EN);
	Delay(0xffff);
	GPIO_ResetBits(LCD_Port, EN);
}

void upNib(uint8_t c) {
	if(c & 0x80)
		GPIO_SetBits(LCD_Port, D7);
	else
		GPIO_ResetBits(LCD_Port, D7);
	if(c & 0x40)
		GPIO_SetBits(LCD_Port, D6);
	else
		GPIO_ResetBits(LCD_Port, D6);
	if(c & 0x20)
		GPIO_SetBits(LCD_Port, D5);
	else
		GPIO_ResetBits(LCD_Port, D5);
	if(c & 0x10)
		GPIO_SetBits(LCD_Port, D4);
	else
		GPIO_ResetBits(LCD_Port, D4);
}

void downNib(uint8_t c) {
	if(c & 0x8)
		GPIO_SetBits(LCD_Port, D7);
	else
		GPIO_ResetBits(LCD_Port, D7);
	if(c & 0x4)
		GPIO_SetBits(LCD_Port, D6);
	else
		GPIO_ResetBits(LCD_Port, D6);
	if(c & 0x2)
		GPIO_SetBits(LCD_Port, D5);
	else
		GPIO_ResetBits(LCD_Port, D5);
	if(c & 0x1)
		GPIO_SetBits(LCD_Port, D4);
	else
		GPIO_ResetBits(LCD_Port, D4);
}

void sendCMD(uint8_t c) {
	GPIO_ResetBits(LCD_Port, RS);
	upNib(c);
	strobeEN();
	downNib(c);
	strobeEN();
}

void printChar(uint8_t c) {
	if(((c>=0x20)&&(c<=0x7F)) || ((c>=0xA0)&&(c<=0xFF))) {	//check if 'c' is within display boundry
		GPIO_SetBits(LCD_Port, RS);
		upNib(c);
		strobeEN();
		downNib(c);
		strobeEN();
		GPIO_ResetBits(LCD_Port, RS);
	}
}

void printString(uint8_t *s) {
	uint8_t i=0;
	
	while(s[i] != '\0') {
		printChar(s[i]);
		i++;
	}
}

void clearLCD(void) {
	sendCMD(0x01);
}

void toLine1(void) {
	sendCMD(0x80);
}

void toLine2(void) {
	sendCMD(0xC0);
}

/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : Delay
* Description    : Inserts a delay time.
* Input          : nCount: specifies the delay time length.
* Output         : None
* Return         : None
* Note					 : ffff=5mS
*******************************************************************************/
void Delay(uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}
