typedef struct gameType{
	byte ch_bitmap[8];
	byte actual[8];
	byte state;
	void (*state_routine)();
	void (*top_button)();
	void (*bottom_button)();
}Game;
