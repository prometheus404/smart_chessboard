#include<FastLED.h>
#include"struct.h"
//including rules
#include"chess.h"

#define NUM_LEDS 64
#define LED_PIN 5
#define SHIFT_MODE 6
#define SHIFT_DATA 7
#define SHIFT_CLK 8
#define LED_GREEN CRGB::GREEN
#define LED_RED CRGB::RED
#define LED_YELLOW CRGB::YELLOW
#define LED_PURPLE CRGB::PURPLE
#define LED_BLUE CRGB::BLUE
#define LED_WHITE CRGB::WHITE
#define LED_BLACK CRGB::BLACK
#define SET_LED(color, row, col) (game.ch_led[(row*8) + col] = color)
#define LED_IS(color, row, col) (game.ch_led[(row*8) + col] == color)
CRGB leds[NUM_LEDS];
Game game;
game.ch_led = leds;
int selected = 0, n_games = 1;
void (*setup_game)()[] = {chess_setup};
String game_text[] = {"chess"};
int n_char = 0;
int found_row = -1, found_col = -1, n_char = 0;
bool found = false, placed = false;

//scheduled TODO connection
void ledShow(){
	if(!game)
		showText(game_text[selected], n_char);
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

//interrupt				TODO: attach interrupt
void buttonTop(){
	if(!game){
		selected = (selected+1)%n_games;
		n_char = 0;
	}
	else{
		game.top_button_interrupt(&game);
	}
}

void buttonMiddle(){
	if(!game)
		setup_game[selected](&game);
	else
		game.middle_button_interrupt(&game);
}

void buttonBottom(){
	if(!game){
		selected = (selected+1)%n_games;
		n_char = 0;
	}
	else{
		game.bottom_button_interrupt(&game);
	}
}


void setup(){
	pinMode(SHIFT_CLK, OUTPUT);
    pinMode(SHIFT_MODE, OUTPUT);
    pinMode(SHIFT_DATA, INPUT);
	FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
}
void loop(){
	//TODO setup scheduler
}
