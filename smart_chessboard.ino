#include"common.h"
//including rules
#include"tris.h"
#include"test.h"
#include"test_green.h"
//#include"chess.h"

//debouncing
unsigned long debouncing = 400; //in milliseconds 
volatile unsigned long last_interrupt;
volatile int selected = 2;
int n_games = 3;
typedef void (*setup_func)();
setup_func setup_game[] = {tris_setup, test_setup, test_green_setup};
String game_text[] = {"tris", "test", "test green"};


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
    delay(10);																	//saving chessboard status
    digitalWrite(SHIFT_MODE, HIGH);												//
    for(int row = 0; row < 8; row++)
		  game.actual[row] = chRead();
}
//called
byte chRead(){
	byte b = 0x00;
	for(int i = 0; i <= 7; i++){
      if(digitalRead(SHIFT_DATA) == HIGH)
        b = b | 0x01<<(7-i);
      digitalWrite(SHIFT_CLK, HIGH);
	  delay(10);
      digitalWrite(SHIFT_CLK, LOW);
    }
    return b;
}



void button_top(){
	if((long)(millis() - last_interrupt) >= debouncing){
		noInterrupts(); //used to avoid concurrency problems with interrupts (if last_interrupt is not updated two interrupts can call the function)
		game.top_button();
		last_interrupt = millis();
		interrupts();
	}
}

void arrow_up(){
	selected = (selected+1)%n_games;
	update_lcd();
}

void button_bottom(){
	if((long)(millis() - last_interrupt) >= debouncing){
		noInterrupts();
		game.bottom_button();
		last_interrupt = millis();
		interrupts();
	}
}
void arrow_down(){
	selected = (selected-1+n_games)%n_games;
	update_lcd();
}

void update_lcd(){
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
	update_lcd();
	//scheduler.startNow();
}

void loop(){
	ch_monitoring();
	game.state_routine();
	FastLED.show();
  FastLED.show();
	delay(200);
	//scheduler.execute();
}
