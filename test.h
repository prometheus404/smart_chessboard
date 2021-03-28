//#include "struct.h"
//#include "common.h"
void test_routine(Game game){
	Serial.println("test_routine");
	for(int row = 0; row < 8; row++)
		for (int col = 0; col < 8; col++)
			if(game.actual[row]^(0x01<<col))
				SET_LED(LED_RED, row, col);
}
void test_setup(Game game){
	game.state = 0x00;
	game.state_routine = test_routine;
}
