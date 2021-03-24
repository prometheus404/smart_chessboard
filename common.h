#define NUM_LEDS 	64
#define TOP 		2
#define BOTTOM 		3
#define LED_PIN 	4
#define SHIFT_MODE 	5
#define SHIFT_DATA 	6
#define SHIFT_CLK 	7
#define LCD_RS 		8
#define LCD_EN 		9
#define LCD_D4 		10
#define LCD_D5 		11
#define LCD_D6 		12
#define LCD_D7 		13

#define LED_GREEN 	CRGB::GREEN
#define LED_RED 	CRGB::RED
#define LED_YELLOW 	CRGB::YELLOW
#define LED_PURPLE 	CRGB::PURPLE
#define LED_BLUE 	CRGB::BLUE
#define LED_WHITE 	CRGB::WHITE
#define LED_BLACK 	CRGB::BLACK

#define SET_LED(color, row, col) 	(game.ch_led[(row*8) + col] = color)
#define LED_IS(color, row, col) 	(game.ch_led[(row*8) + col] == color)
