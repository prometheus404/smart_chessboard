typedef struct gameType{
	byte ch_bitmap[8];
	byte actual[8];
	//byte ch_ext[8][8];		useless
	//CRGB ch_led[8][8];		use SET_LED instead
	byte state;
	void (*state_routine)(gameType game);
	//void (*ledRoutine)(CRGB** led_matrix);
	void (*top_button)(gameType game);
	//void (*middle_button)(gameType game);
	void (*bottom_button)(gameType game);
	
	int found_row = -1, found_col = -1;
	bool found = false, placed = false;
}Game;
