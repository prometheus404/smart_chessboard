#include<FastLED.h>
#include"struct.h"
//including rules
#include"chess.h"

#define NUM_LEDS 64
#define LED_PIN 5
#define SHIFT_MODE 6
#define SHIFT_DATA 7
#define SHIFT_CLK 8
#define GREEN CRGB::GREEN
#define RED CRGB::RED
#define YELLOW CRGB::YELLOW
#define PURPLE CRGB::PURPLE
#define BLUE CRGB::BLUE
#define WHITE CRGB::WHITE
#define BLACK CRGB::BLACK
#define SET_LED(color, row, col) (game.ch_led[row][col] = color)

CRGB leds[NUM_LEDS];
Game game;
game.ch_led = leds;
int selected = 0, n_games = 1;
void (*setup_game)()[] = {chess_setup};
String game_text[] = {"chess"};
int found_row = -1, found_col = -1;
bool found = false, placed = false;

//scheduled
void ledShow(){
	if(!game)
		showText(game_text[selected]);
	else
		FastLED.show;
}

void chMonitoring(){
	//TODO ensure that chMonitoring is scheduled only after game setup
	if(found) return;															//should monitor only when loop isn't working on the same variables
    int row = 0, col, diff = 0;
	digitalWrite(SHIFT_MODE, LOW);												//
    delay(5);																	//saving chessboard status
    digitalWrite(SHIFT_MODE, HIGH);												//
    for(; row < 8; row++){
        diff = chRead()^(game.ch_bitmap[row]);									//diff is the difference between the saved bitmap and the actual chessboard
        if(diff){
            if(found){
                errorHandler();													//if more than one piece is moved -> error
                return;
            }
            for(col = 1; col <= 8; col++){										//finding the diferent value position
                if(!(diff >> col)){
                    if(game.ch_bitmap[row]&(1<<col))							//if the old value was 1-> piece removed
						placed = false;
					else														//if the old value was 0-> piece placed
						placed = true;

					found_row = row;											//saving piece position
					found_col = col;
                }
            }
            found = true;
        }
    }
}
//called
byte chRead(){
	byte b = 0x00;
	for(int i = 0; i <= 7; i++){
      if(digitalRead(SHIFT_DATA) == HIGH)
        b = b | 0x01<<(7-i);
      digitalWrite(CLK, HIGH);
	  delay(5);
      digitalWrite(CLK, LOW);
    }
    return b;
}
//interrupt				TODO: attach interrupt
void buttonTop(){
	if(!game)
		selected = (selected+1)%n_games;
	else
		game.top_button_interrupt(&game);
}
void buttonMiddle(){
	if(!game)
		setup_game[selected](&game);
	else
		game.middle_button_interrupt(&game);
}
void buttonBottom(){
	if(!game)
		selected = (selected+1)%n_games;
	else
		game.bottom_button_interrupt(&game);
}


void setup(){
	pinMode(SHIFT_CLK, OUTPUT);
    pinMode(SHIFT_MODE, OUTPUT);
    pinMode(SHIFT_DATA, INPUT);
}
void loop(){
	//TODO setup scheduler
}
