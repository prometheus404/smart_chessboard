typedef struct gameType{
  byte array1[8] = {0,0,0,0,0,0,0,0};
  byte array2[8] = {0,0,0,0,0,0,0,0};
	byte * ch_bitmap = array1;
	byte * actual = array2;
	byte state;
	void (*state_routine)();
	void (*top_button)();
	void (*bottom_button)();
}Game;
