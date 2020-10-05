typedef struct gameType{
	byte ch_bitmap[8];
	//byte ch_ext[8][8];		useless
	//CRGB ch_led[8][8];		use SET_LED instead
	byte state;
	void (*stateRoutine)(byte actual_state);
	//void (*ledRoutine)(CRGB** led_matrix);
	void (*top_button_interrupt)();
	void (*middle_button_interrupt)();
	void (*bottom_button_interrupt)();
	
	int found_row = -1, found_col = -1;
	bool found = false, placed = false;
}Game;
