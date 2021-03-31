#include<FastLED.h>
#include<TaskScheduler.h>
#include <LiquidCrystal.h>
#include "struct.h"

#define NUM_LEDS 	64
#define TOP 		2
#define BOTTOM 		3
#define LED_PIN 	4
#define SHIFT_MODE 	5
#define SHIFT_CLK 	6
#define SHIFT_DATA 	7
#define LCD_RS 		8
#define LCD_EN 		9
#define LCD_D4 		10
#define LCD_D5 		11
#define LCD_D6 		12
#define LCD_D7 		13

LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
CRGB leds[NUM_LEDS];
Game game;

#define LED_GREEN 	CRGB::Green
#define LED_RED 	CRGB::Red
#define LED_YELLOW 	CRGB::Yellow
#define LED_PURPLE 	CRGB::Purple
#define LED_BLUE 	CRGB::Blue
#define LED_WHITE 	CRGB::White
#define LED_BLACK 	CRGB::Black


#define SET_LED(color, row, col) 	(leds[(row*8) + col] = color)
#define LED_IS(color, row, col) 	(leds[(row*8) + col] == color)

//including rules
#include"test.h"
#include"test_green.h"
//#include"chess.h"


volatile int selected = 0;
int n_games = 2;
typedef void (*setup_func)();
setup_func setup_game[] = {test_setup, test_green_setup};
String game_text[] = {"test", "test green"};


//TODO scheduler
//void ch_monitoring();
//void led_show();

//Scheduler scheduler;
//Task t_monitor(200, TASK_FOREVER, &ch_monitoring);
//Task t_led(200, TASK_FOREVER, &led_show, &scheduler, true);
//Task t_game(200, TASK_FOREVER, game.state_routine, &scheduler, true);
//TODO generate a dynamic third Task for the game main loop 

void led_show(){
	FastLED.show();
}

//chMonitoring should only return the chessboard bitmap
//additional operation could differ between games
void ch_monitoring(){
	//TODO ensure that chMonitoring is scheduled only after game setup
	digitalWrite(SHIFT_MODE, LOW);
    delay(5);																	//saving chessboard status
    digitalWrite(SHIFT_MODE, HIGH);												//
    for(int row = 0; row < 8; row++){
		game.actual[row] = chRead();
    Serial.print(row);
    if(game.actual[row])
      Serial.println(1);
    else
      Serial.println(0);
    }
}
//called
byte chRead(){
	byte b = 0x00;
	for(int i = 0; i <= 7; i++){
      if(digitalRead(SHIFT_DATA) == HIGH)
        b = b | 0x01<<(7-i);
      digitalWrite(SHIFT_CLK, HIGH);
	  delay(5);
      digitalWrite(SHIFT_CLK, LOW);
    }
    return b;
}



void button_top(){
	game.top_button();
}
void arrow_up(){
	selected = (selected+1)%n_games;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("select game:");
  lcd.setCursor(0,1);
  lcd.print(game_text[selected]);
}

void button_bottom(){
	game.bottom_button();
}
void arrow_down(){
	selected = (selected-1)%n_games;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("select game:");
  lcd.setCursor(0,1);
  lcd.print(game_text[selected]);
}

//check if a piece is placed on the board, if found set up the game
void main_loop(){
	for(int i = 0; i < 8; i++)
		if(game.actual[i])
			setup_game[selected]();
}

void setup(){
	pinMode(SHIFT_CLK, OUTPUT);
  pinMode(SHIFT_MODE, OUTPUT);
  pinMode(SHIFT_DATA, INPUT);
	attachInterrupt(digitalPinToInterrupt(TOP), button_top, RISING);
	attachInterrupt(digitalPinToInterrupt(BOTTOM), button_bottom, RISING);
	FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
	Serial.begin(9600);
  lcd.begin(16, 2);
	lcd.print("hi");
	game.top_button = *arrow_up; 
	game.bottom_button = *arrow_down;
	game.state_routine = *main_loop;
	delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("select game:");
  lcd.setCursor(0,1);
  lcd.print(game_text[selected]);
  
	//scheduler.startNow();
}
void loop(){
  ch_monitoring();
  game.state_routine();
  FastLED.show();
  delay(200);
	//scheduler.execute();
}
