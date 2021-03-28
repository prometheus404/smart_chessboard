#include<FastLED.h>
#include<TaskScheduler.h>
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

#define LED_GREEN 	CRGB::Green
#define LED_RED 	CRGB::Red
#define LED_YELLOW 	CRGB::Yellow
#define LED_PURPLE 	CRGB::Purple
#define LED_BLUE 	CRGB::Blue
#define LED_WHITE 	CRGB::White
#define LED_BLACK 	CRGB::Black
#include "struct.h"
#define SET_LED(color, row, col) 	(leds[(row*8) + col] = color)
#define LED_IS(color, row, col) 	(leds[(row*8) + col] == color)
CRGB leds[NUM_LEDS];
Game game;
//#include"common.h"
//#include"struct.h"
//including rules
#include"test.h"
//#include"chess.h"







volatile int selected = 0;
int n_games = 1;
typedef void (*setup_func)(Game game);
setup_func setup_game[] = {test_setup};
String game_text[] = {"test"};
//moved in struct
//int found_row = -1, found_col = -1, n_char = 0;
//bool found = false, placed = false;

//Scheduler
void ch_monitoring();
void led_show();
Scheduler scheduler;
Task t_monitor(200, TASK_FOREVER, &ch_monitoring);
Task t_led(200, TASK_FOREVER, &led_show, &scheduler, true);
Task t_game(200, TASK_FOREVER, game.state_routine, &scheduler, true);
//TODO generate a dynamic third Task for the game main loop 

void led_show(){
/*
	if(!game)
		showText(game_text[selected], n_char);
*/
	FastLED.show();
}

//chMonitoring should only return the chessboard bitmap
//additional operation could differ between games
void ch_monitoring(){
	//TODO ensure that chMonitoring is scheduled only after game setup
	//if(found) return;															//should monitor only when loop isn't working on the same variables
    //int row = 0, col, diff = 0;a
	digitalWrite(SHIFT_MODE, LOW);												//
    delay(5);																	//saving chessboard status
    digitalWrite(SHIFT_MODE, HIGH);												//
    for(int row = 0; row < 8; row++){
		game.actual[row] = chRead();
	/*
        diff = chRead()^(game.ch_bitmap[row]);									//diff is the difference between the saved bitmap and the actual chessboard
        if(diff){
            if(game.found){
                errorHandler();													//if more than one piece is moved -> error
                return;
            }
            for(col = 1; col <= 8; col++){										//finding the diferent value position
                if(!(diff >> col)){
                    if(game.ch_bitmap[row]^(1<<col))							//if the old value was 1-> piece removed
						game.placed = false;
					else														//if the old value was 0-> piece placed
						game.placed = true;

					game.found_row = row;											//saving piece position
					game.found_col = col;
                }
            }
            game.found = true;
        }*/
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

//this function could be useless if a segment display is present
//but even if useful should be included as a lib
/*
void showText(String game_text,int n_char){	
switch(game_text[n_char]){
		case 'c':
			for(int row = 0; row < 8; row++)
				for(int col = 0; col < 8; col++)
					if(row == 0 || row == 8 || col == 0)
						SET_LED(LED_WHITE, row, col);
					else
						SET_LED(LED_BLACK, row, col);
			break;
		case 'h':
			for(int row = 0; row < 8; row++)
				for(int col = 0; col < 8; col++)
					if(col == 0 || col == 8 || row == 3)
						SET_LED(LED_WHITE, row, col);
					else
						SET_LED(LED_BLACK, row, col);
			break;
		case 'e':
			for(int row = 0; row < 8; row++)
				for(int col = 0; col < 8; col++)
					if(row == 0 || row == 8 || row == 3 || col == 0)
						SET_LED(LED_WHITE, row, col);
					else
						SET_LED(LED_BLACK, row, col);
			break;
		case 's':
			for(int row = 0; row < 8; row++)
				for(int col = 0; col < 8; col++)
					if(row == 0 || row == 4 || row == 7 ||(col == 0 && row < 4)||(col == 7 && row > 4))
						SET_LED(LED_WHITE, row, col);
					else
						SET_LED(LED_BLACK, row, col);
	}
	n_char++;
}
*/

//interrupt				TODO: attach interrupt
void button_top(){
	game.top_button(game);
}
void arrow_up(){
	selected = (selected+1)%n_games;
	Serial.println(game_text[selected]);
}

//on UNO board there are only 2 interrupt pin
/*
void buttonMiddle(){
	if(!game)
		setup_game[selected](&game);
	else
		game.middle_button_interrupt(&game);
}
*/
void button_bottom(){
	game.bottom_button(game);
}
void arrow_down(){
	selected = (selected-1)%n_games;
	Serial.println(game_text[selected]);
}

//check if a piece is placed on the board, if found set up the game
void main_loop(){
	Serial.print("main_loop: ");
	Serial.println(game_text[selected]);
	for(int i = 0; i < 8; i++)
		if(game.actual[i])
			setup_game[selected](game);
}

void setup(){
	pinMode(SHIFT_CLK, OUTPUT);
    pinMode(SHIFT_MODE, OUTPUT);
    pinMode(SHIFT_DATA, INPUT);
	attachInterrupt(digitalPinToInterrupt(TOP), button_top, RISING);
	attachInterrupt(digitalPinToInterrupt(BOTTOM), button_bottom, RISING);
	FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
	Serial.begin(9600);
	Serial.write("game initialization");
	game.top_button = arrow_up; 
	game.bottom_button = arrow_down;
	game.state_routine = main_loop;
	Serial.write("starting now");
	scheduler.startNow();
}
void loop(){
	Serial.println("loop");
	scheduler.execute();
}
