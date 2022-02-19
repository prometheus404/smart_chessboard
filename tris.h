#define SETUP 0x00
#define WAIT 0x01

#define WIN 0x00
#define DRAW 0x01

int board[] = {0,0,0,0,0,0,0,0,0};
CRGB player_color[] = {LED_YELLOW, LED_PURPLE};
int player = 1;

typedef struct __move{
	int found;
	int placed;
	int found_row;
	int found_col;
}Move;

Move * movement(){
	Move * move =  malloc(sizeof(Move));
	move->found = 0; 	//0 if not fund, 1 if found 1, values greater than 1 are errors
	move->placed = 0;
	move->found_row = 0;
	move->found_col = 0;

	for(int i = 0; i < 8; i++){
		byte diff = game.actual[i] ^ game.ch_bitmap[i]; 	// 1 only where difference is found
		if(diff){
			for(int j = 0; j < 8; j++){
				if((0x80>>j)&diff){
					move->found++;
					move->found_col = j;
					move->found_row = i;
					move->placed = game.actual[i] & diff; 	//0 if not placed, != 0 if placed
				}
			}
		}
	}

	return move;
}

int move_to_sq(int row, int col){
	if((col+1) % 3 == 0 || (row+1) % 3 == 0){
		return -1;
	} else {
		return 3*(row / 3) + (col /3);
	}
}

void fill_sq(int square, int player){
	int c = square % 3 * 3;
	int r = square / 3 * 3;
	SET_LED(player_color[player], r, c);
	SET_LED(player_color[player], (r+1), c); //(r+1) not r+1 because of problems with macro
	SET_LED(player_color[player], r, c+1);
	SET_LED(player_color[player], (r+1), c+1);
}

void draw_field(){
	for(int row = 0; row < 8; row++){
		for(int col = 0; col < 8; col++){
			if((col+1) % 3 == 0 || (row+1) % 3 == 0)
				SET_LED(LED_WHITE, row, col);
			else
				SET_LED(LED_BLACK, row, col);
		}
	}
	//draw pieces
	for(int sq = 0; sq < 9; sq++)
		if(board[sq])
			fill_sq(sq, board[sq]-1);
	FastLED.show();
}

void reset_board(){
  Serial.println("reset");
	for(int i = 0; i < 9; i++)
		board[i] = 0;
}

bool check_win(){
	//vertical
	for(int i = 0; i < 3; i++)
		if(board[i] && board[i] == board[i+3] && board[i] == board[i+6])
			return true;
	//horizontal
	for(int i = 0; i <= 6; i += 3)
		if(board[i] && board[i] == board[i+1] && board[i] == board[i+2])
			return true;
	//diagonal
	if(board[0] && board[0] == board[4] && board[0] == board[8])
		return true;
	if(board[2] && board[2] == board[4] && board[2] == board[6])
		return true;
	return false;
}

bool check_draw(){
	for(int i = 0; i < 9; i++)
		if(!board[i])
			return false;
	return true;
}

void end(byte result){
	CRGB color = LED_GREEN;
	if(result == WIN)
		color = player_color[player-1];
	for(int row = 0; row < 8; row++)
		for(int col = 0; col < 8; col++)
			SET_LED(color, row, col);
	FastLED.show();
	delay(2000);
	reset_board();
	game.state = SETUP;
}

void tris_routine(){
	switch(game.state){
		case SETUP:
			//reset_board();
			game.state = WAIT;
			break; //not breaking saves some time
		case WAIT:
			Move * move = movement();
			if(!move->found) return;
			int square = move_to_sq(move->found_row, move->found_col);
			if(square == -1) {Serial.println("invalid move"); break;}
			//Serial.println(square);
			Serial.print(player);
			Serial.print(" has ");
			if(move->placed)
				Serial.print("placed ");
			else
				Serial.print("removed ");
			Serial.print(move->found);
			Serial.print(" pieces from square");
			Serial.println(square);
			if(board[square])
				Serial.println("square is already full");
			if(move->found > 1 || !move->placed || board[square]){
				//TODO error
			} else {
				board[square] = player;
				if(check_win())
					end(WIN);
				if(check_draw())
					end(DRAW);
				player = player ^ 0x03;
			}
			byte * bitmap = game.ch_bitmap;
			game.ch_bitmap = game.actual;
			game.actual = bitmap;
			free(move);
			break;
	}
	draw_field();
}

void restart(){
	game.state = 0x00;
	//reset_board();
}

void tris_setup(){
	game.state = 0x00;
	game.state_routine = tris_routine;
	game.top_button = restart;
	game.bottom_button = restart;
}
