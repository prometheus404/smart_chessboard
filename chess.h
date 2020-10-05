#define PRESENT 0xff
#define WHITE 0x01
#define BLACK 0x02
#define PAWN 0x04
#define ROOK 0x08
#define KNIGHT 0x10
#define BISHOP 0x20
#define QUEEN 0x40
#define KING 0x80
#define PIECE_IS(type, row, col) (ch_ext[row][col]&type)                        //equal zero if type doesn't matches, greater than zero otherwise
#define PIECE_IS_ALL(type, row, col) ((ch_ext[row][col]&type) == type)
#define OPPONENT(player) player^0x03
#define MOVE(old_row, old_col, new_row, new_col) (ch_ext[new_row][new_col] = ch_ext[old_row][old_col]; ch_ext[old_row][old_col] = 0x00)
#define CAN_CASTLE(pl) (castling[pl-1])

byte ch_bitmap[] = {0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff};
byte ch_ext[][8] = {
    {ROOK|BLACK, KNIGHT|BLACK, BISHOP|BLACK, QUEEN|BLACK, KING|BLACK, BISHOP|BLACK, KNIGHT|BLACK, ROOK|BLACK},
    {PAWN|BLACK, PAWN|BLACK, PAWN|BLACK, PAWN|BLACK, PAWN|BLACK, PAWN|BLACK, PAWN|BLACK, PAWN|BLACK},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {PAWN|WHITE, PAWN|WHITE, PAWN|WHITE, PAWN|WHITE, PAWN|WHITE, PAWN|WHITE, PAWN|WHITE, PAWN|WHITE},
    {ROOK|WHITE, KNIGHT|WHITE, BISHOP|WHITE, QUEEN|WHITE, KING|WHITE, BISHOP|WHITE, KNIGHT|WHITE, ROOK|WHITE},
};
//player can be 1(white) or 2(black) instead of 0 and 1 for simpler controls
byte player = 0x01, top_player;													//player is the active player, top_player is the player on the top
bool castling[] = {true, true};
int piece_row = -1, piece_col = -1;
//bool found = false, placed = false;


void find_moves(int row, int col){
    int direction = -1;
    if(!PIECE_IS(player, row, col)){											//a player can only move his pieces
        error_handler();
        return;
    }
    switch (ch_ext[row][col]) {
        case PAWN|BLACK:
        case PAWN|WHITE:
			//top_player is the player on top (color doesn't matter)
			if(PIECE_IS(top_player, row, col)) direction = 1;						//top_player's pawns can only eat and move downward, bottom player's pawns can only eat and move upward
			//(row == direction || row == 7+direction) -> true only if the pawn is in the original position
			//if pawn belongs to the top player its original position is row 1 and direction is 1-> row == direction (a pawn of the bottom player can never be in row -1)
			//if pawn belongs to the bottom player its original position is  row 6 and direction is -1 -> row == 7+direction (a pawn of the top player can never be in row 8)
            if(row+(2*direction) < 8 && row+(2*direction) >=0 && !PIECE_IS(PRESENT, row+(2*direction), col) && (row == direction || row == 7 + direction))
				SET_LED(LED_GREEN, row+(2*direction), col);
			if(row + direction < 8 && row + direction >= 0){
				if(!PIECE_IS(PRESENT, row + direction, col))
					SET_LED(LED_GREEN, row + direction, col);
	            if(PIECE_IS(OPPONENT(player), row + direction, col+1))
					SET_LED(LED_RED, row + direction, col+1);
	            if(PIECE_IS(OPPONENT(player), row + direction, col-1))
					SET_LED(LED_RED, row + direction, col-1);
			}
            break;
        case ROOK|WHITE:
        case ROOK|BLACK:
            for(int i = row; i < 8; i++){
                if(!PIECE_IS(PRESENT, i, col)) SET_LED(LED_GREEN, i, col);
                if(PIECE_IS(OPPONENT(player), i, col)){
                    SET_LED(LED_RED, i, col);
                    break;
                }
            }
            for(int i = row; i >= 0; i--){
                if(!PIECE_IS(PRESENT, i, col)) SET_LED(LED_GREEN, i, col);
                if(PIECE_IS(OPPONENT(player), i, col)){
                    SET_LED(LED_RED, i, col);
                    break;
                }
            }
            for(int i = col; i < 8; i++){
                if(!PIECE_IS(PRESENT, row, i)) SET_LED(LED_GREEN, row, i);
                if(PIECE_IS(OPPONENT(player), row, i)){
                    SET_LED(LED_RED, row, i);
                    break;
                }
            }
            for(int i = col; i >= 0; i--){
                if(!PIECE_IS(PRESENT, row, i)) SET_LED(LED_GREEN, row, i);
                if(PIECE_IS(OPPONENT(player), row, i)){
                    SET_LED(LED_RED, row, i);
                    break;
                }
            }
            break;
        case KNIGHT|BLACK:
        case KNIGHT|WHITE:
            if(row < 7 && col < 6 && !PIECE_IS(PRESENT, row + 1, col + 2)) SET_LED(LED_GREEN, row + 1, col + 2);
            if(row < 7 && col > 1 && !PIECE_IS(PRESENT, row + 1, col - 2)) SET_LED(LED_GREEN, row + 1, col - 2);
            if(row > 0 && col < 6 && !PIECE_IS(PRESENT, row - 1, col + 2)) SET_LED(LED_GREEN, row - 1, col + 2);
            if(row > 0 && col > 1 && !PIECE_IS(PRESENT, row - 1, col - 2)) SET_LED(LED_GREEN, row - 1, col - 2);

            if(row < 6 && col < 7 && !PIECE_IS(PRESENT, row + 2, col + 1)) SET_LED(LED_GREEN, row + 2, col + 1);
            if(row < 6 && col > 0 && !PIECE_IS(PRESENT, row + 2, col - 1)) SET_LED(LED_GREEN, row + 2, col - 1);
            if(row > 1 && col < 7 && !PIECE_IS(PRESENT, row - 2, col + 1)) SET_LED(LED_GREEN, row - 2, col + 1);
            if(row > 1 && col > 0 && !PIECE_IS(PRESENT, row - 2, col - 1)) SET_LED(LED_GREEN, row - 2, col - 1);

            if(row < 7 && col < 6 && PIECE_IS(OPPONENT(player), row + 1, col + 2)) SET_LED(LED_RED, row + 1, col + 2);
            if(row < 7 && col > 1 && PIECE_IS(OPPONENT(player), row + 1, col - 2)) SET_LED(LED_RED, row + 1, col - 2);
            if(row > 0 && col < 6 && PIECE_IS(OPPONENT(player), row - 1, col + 2)) SET_LED(LED_RED, row - 1, col + 2);
            if(row > 0 && col > 1 && PIECE_IS(OPPONENT(player), row - 1, col - 2)) SET_LED(LED_RED, row - 1, col - 2);

            if(row < 6 && col < 7 && PIECE_IS(OPPONENT(player), row + 2, col + 1)) SET_LED(LED_RED, row + 2, col + 1);
            if(row < 6 && col > 0 && PIECE_IS(OPPONENT(player), row + 2, col - 1)) SET_LED(LED_RED, row + 2, col - 1);
            if(row > 1 && col < 7 && PIECE_IS(OPPONENT(player), row - 2, col + 1)) SET_LED(LED_RED, row - 2, col + 1);
            if(row > 1 && col > 0 && PIECE_IS(OPPONENT(player), row - 2, col - 1)) SET_LED(LED_RED, row - 2, col - 1);
            break;
        case KING|BLACK:
        case KING|WHITE:
			//normal movement
			for(int i = -1; i < 2; i++){
				if(row + i > 7 || row + i < 0)
					continue;
				for(int j = -1; j < 2; j++){
					if(col + j > 7 || col + j < 0)
						continue;
					if(!PIECE_IS(PRESENT, row + i, col + j)) SET_LED(LED_GREEN, row+i, col+j);
					if(PIECE_IS(OPPONENT(player), row + i, col + j)) SET_LED(LED_RED, row+i, col+j);
				}
			}
			//short castling
			if(CAN_CASTLE(player) && !PIECE_IS(PRESENT, row, col + 1) && !PIECE_IS(PRESENT, row, col + 2))
				SET_LED(LED_BLUE, row, col + 2);
			//long castling
			if(CAN_CASTLE(player) && !PIECE_IS(PRESENT, row, col - 1) && !PIECE_IS(PRESENT, row, col - 2))
				SET_LED(LED_BLUE, row, col - 2);
            break;
        case QUEEN|BLACK:
        case QUEEN|WHITE:
            for(int i = row; i < 8; i++){
                if(!PIECE_IS(PRESENT, i, col)) SET_LED(LED_GREEN, i, col);
                if(PIECE_IS(OPPONENT(player), i, col)){
                    SET_LED(LED_RED, i, col);
                    break;
                }
            }
            for(int i = row; i >= 0; i--){
                if(!PIECE_IS(PRESENT, i, col)) SET_LED(LED_GREEN, i, col);
                if(PIECE_IS(OPPONENT(player), i, col)){
                    SET_LED(LED_RED, i, col);
                    break;
                }
            }
            for(int i = col; i < 8; i++){
                if(!PIECE_IS(PRESENT, row, i)) SET_LED(LED_GREEN, row, i);
                if(PIECE_IS(OPPONENT(player), row, i)){
                    SET_LED(LED_RED, row, i);
                    break;
                }
            }
            for(int i = col; i >= 0; i--){
                if(!PIECE_IS(PRESENT, row, i)) SET_LED(LED_GREEN, row, i);
                if(PIECE_IS(OPPONENT(player), row, i)){
                    SET_LED(LED_RED, row, i);
                    break;
                }
            }
        case BISHOP|BLACK:
        case BISHOP|WHITE:
            for(int i = 0; row + i < 8 && col + i < 8; i++){
                if(!PIECE_IS(PRESENT, row + i, col + i)) SET_LED(LED_GREEN, row + i, col + i);
                if(PIECE_IS(OPPONENT(player), row + i, col + i)) SET_LED(LED_RED, row + i, col + i);
            }
            for(int i = 0; row + i < 8 && col - i > 0; i++){
                if(!PIECE_IS(PRESENT, row + i, col - i)) SET_LED(LED_GREEN, row + i, col - i);
                if(PIECE_IS(OPPONENT(player), row + i, col - i)) SET_LED(LED_RED, row + i, col - i);
            }
            for(int i = 0; row - i > 0 && col + i < 8; i++){
                if(!PIECE_IS(PRESENT, row - i, col + i)) SET_LED(LED_GREEN, row - i, col + i);
                if(PIECE_IS(OPPONENT(player), row - i, col + i)) SET_LED(LED_RED, row - i, col + i);
            }
            for(int i = 0; row - i > 0 && col - i > 0; i++){
                if(!PIECE_IS(PRESENT, row - i, col - i)) SET_LED(LED_GREEN, row - i, col - i);
                if(PIECE_IS(OPPONENT(player), row - i, col - i)) SET_LED(LED_RED, row - i, col - i);
            }
            break;
    }
}//TODO add suicidal move control

bool suicidal_move(int old_row, int old_col, int new_row, int new_col){
	//TODO disable interrupt
	byte backup = ch_ext[new_row][new_col];
	MOVE(old_row, old_col, new_row, new_col);
	//ch_ext[new_row][new_col] = ch_ext[old_row][old_col];						//moving the piece into his new position
	//ch_ext[old_row][old_col] = 0x00;											//clearing his old position

	bool result = check(player);

	MOVE(new_row, new_col, old_row, old_col);
	//ch_ext[old_row][old_col] = ch_ext[new_row][new_col];						//restoring old positions
	//ch_ext[new_row][new_col] = 	backup;
	//TODO enable interrupt

	return result;
}

bool suicidal_castling(int k_row, int old_k_col, int new_k_col){
	//TODO disable interrupt
	//no need to backup because new positions are free
	MOVE(k_row, old_k_col, k_row, new_k_col);
	//ch_ext[k_row][new_k_col] = ch_ext[k_row][old_k_col];						//moving the king into his new position
	//ch_ext[k_row][old_k_col] = 0x00;
	if(old_k_col < new_k_col){													//short castling -> rook is on the left
		MOVE(k_row, 7, k_row, new_k_col-1 );									//this macro must always be in brackets
		//ch_ext[k_row][new_k_col - 1] = ch_ext[k_row][7];
		//ch_ext[k_row][7] = 0x00;
	}
	else{
		MOVE(k_row, 0, k_row, new_k_col+1);
		//ch_ext[k_row][new_k_col + 1] = ch_ext[k_row][0];						//long castling -> rook is on the right
		//ch_ext[k_row][0] = 0x00;
	}

	bool result = check(player);

	MOVE(k_row, new_k_col, k_row, old_k_col);
	//ch_ext[k_row][old_k_col] = ch_ext[k_row][new_k_col];						//restoring old positions
	//ch_ext[k_row][new_k_col] = 0x00;
	if(old_k_col < new_k_col){
		MOVE(k_row, new_k_col-1, k_row, 7);
		//ch_ext[k_row][7] = ch_ext[k_row][new_k_col - 1];
		//ch_ext[k_row][new_k_col - 1] = 0x00;
	}
	else{
		MOVE(k_row, new_k_col+1, k_row, 0);
		//ch_ext[k_row][0] = ch_ext[k_row][new_k_col + 1];
		//ch_ext[k_row][new_k_col + 1] = 0x00;
	}
	//TODO enable interrupt
}

bool check(byte player){
	//find king
	int k_row, k_col;
	for(int row = 0; row < 8; row++){
		for(int col = 0; col < 8; col++){
			if(PIECE_IS_ALL(KING|player, row, col)){
				k_row = row;
				k_col = col;
			}
		}
	}
	//up
	for(int i = 0; i < 8 - k_row; i++){
		if(PIECE_IS(PRESENT, k_row + i, k_col)){
			if(i == 1 && PIECE_IS_ALL(KING|OPPONENT(player), k_row + i, k_col)) return true;
			if(PIECE_IS(QUEEN|ROOK, k_row + i, k_col) && PIECE_IS(OPPONENT(player), k_row + i, k_col)) return true;
			break;
		}
	}
	//down
	for(int i = 0; i < k_row; i++){
		if(PIECE_IS(PRESENT, k_row - i, k_col)){
			if(i == 1 && PIECE_IS_ALL(KING|OPPONENT(player), k_row - i, k_col)) return true;
			if(PIECE_IS(QUEEN|ROOK, k_row - i, k_col) && PIECE_IS(OPPONENT(player), k_row - i, k_col)) return true;
			break;
		}
	}
	//left
	for(int i = 0; i < k_col; i++){
		if(PIECE_IS(PRESENT, k_row, k_col - i)){
			if(i == 1 && PIECE_IS_ALL(KING|OPPONENT(player), k_row, k_col - i)) return true;
			if(PIECE_IS(QUEEN|ROOK, k_row, k_col - i) && PIECE_IS(OPPONENT(player), k_row, k_col - i)) return true;
			break;
		}
	}
	//right
	for(int i = 0; i < 8 - k_col; i++){
		if(PIECE_IS(PRESENT, k_row, k_col + i)){
			if(i == 1 && PIECE_IS_ALL(KING|OPPONENT(player), k_row, k_col + i)) return true;
			if(PIECE_IS(QUEEN|ROOK, k_row, k_col + i) && PIECE_IS(OPPONENT(player), k_row, k_col + i)) return true;
			break;
		}
	}
	//up-left
	for(int i = 0; i < 8 - k_row &&  i < k_col; i++){
		if(PIECE_IS(PRESENT, k_row + i, k_col - i)){
			if(i == 1 && PIECE_IS_ALL(KING|OPPONENT(player), k_row + i, k_col - i)) return true;
			if(PIECE_IS(QUEEN|BISHOP, k_row + i, k_col - i) && PIECE_IS(OPPONENT(player), k_row + i, k_col - i)) return true;
			break;
		}
	}
	//up-right
	for(int i = 0; i < 8 - k_row &&  i < 8 - k_col; i++){
	   if(PIECE_IS(PRESENT, k_row + i, k_col + i)){
		   if(i == 1 && PIECE_IS_ALL(KING|OPPONENT(player), k_row + i, k_col + i)) return true;
		   if(PIECE_IS(QUEEN|BISHOP, k_row + i, k_col + i) && PIECE_IS(OPPONENT(player), k_row + i, k_col + i)) return true;
		   break;
	   }
	}
	//down-left
	for(int i = 0; i < k_row &&  i < k_col; i++){
		if(PIECE_IS(PRESENT, k_row - i, k_col - i)){
			if(i == 1 && PIECE_IS_ALL(KING|OPPONENT(player), k_row - i, k_col - i)) return true;
			if(PIECE_IS(QUEEN|BISHOP, k_row - i, k_col - i) && PIECE_IS(OPPONENT(player), k_row - i, k_col - i)) return true;
			break;
		}
	}
	//down-right
	for(int i = 0; i < k_row &&  i < 8 - k_col; i++){
		if(PIECE_IS(PRESENT, k_row - i, k_col + i)){
			if(i == 1 && PIECE_IS_ALL(KING|OPPONENT(player), k_row - i, k_col + i)) return true;
			if(PIECE_IS(QUEEN|BISHOP, k_row - i, k_col + i) && PIECE_IS(OPPONENT(player), k_row - i, k_col + i)) return true;
			break;
		}
	}
	//pawn
	if(player == top_player)														//if player is the player on top the opponent's pawn can eat only if they are below the king
		if(PIECE_IS_ALL(PAWN|OPPONENT(player), k_row - 1, k_col - 1) || PIECE_IS_ALL(PAWN|OPPONENT(player), k_row - 1, k_col + 1)) return true;
	else
		if(PIECE_IS_ALL(PAWN|OPPONENT(player), k_row + 1, k_col - 1) || PIECE_IS_ALL(PAWN|OPPONENT(player), k_row + 1, k_col + 1)) return true;
	//knight
	if(k_row < 7 && k_col < 5 && PIECE_IS_ALL(KNIGHT|OPPONENT(player), k_row + 1, k_col + 2)) return true;
	if(k_row < 7 && k_col > 2 && PIECE_IS_ALL(KNIGHT|OPPONENT(player), k_row + 1, k_col - 2)) return true;
	if(k_row > 1 && k_col < 5 && PIECE_IS_ALL(KNIGHT|OPPONENT(player), k_row - 1, k_col + 2)) return true;
	if(k_row > 1 && k_col > 2 && PIECE_IS_ALL(KNIGHT|OPPONENT(player), k_row - 1, k_col - 2)) return true;

	if(k_row < 5 && k_col < 7 && PIECE_IS_ALL(KNIGHT|OPPONENT(player), k_row + 2, k_col + 1)) return true;
	if(k_row < 5 && k_col > 1 && PIECE_IS_ALL(KNIGHT|OPPONENT(player), k_row + 2, k_col - 1)) return true;
	if(k_row > 2 && k_col < 7 && PIECE_IS_ALL(KNIGHT|OPPONENT(player), k_row - 2, k_col + 1)) return true;
	if(k_row > 2 && k_col > 1 && PIECE_IS_ALL(KNIGHT|OPPONENT(player), k_row - 2, k_col - 1)) return true;
}

void errorHandler(){}//TODO chessboard LED_RED; found = false -> enables monitoring

void chess_setup(Game *game_ptr){
	game->ch_bitmap = ch_bitmap;
	game->state = COLOR_CHOICE;
	game->stateRoutine = stateRoutine;
	game->top_button_interrupt = resignTop;
	game->middle_button_interrupt = draw;
	game->bottom_button_interrupt = resignBottom;

}

void stateRoutine(Game game){
	switch(game.state){
		case COLOR_CHOICE: //TODO 1 or 2 players, white on top or bottom
			break;
		case CH_SETUP:
			if(ch_setted())
				game.state = WAITING;
			break;
		case WAITING:
			if(game.found){
				if(game.placed){
					errorHandler();
				}
				else{
					piece_row = game.found_row;
					piece_col = game.found_col;
					find_moves(piece_row, piece_col);
					game.state = MOVE;
				}
			}
			break;
		case MOVE:
			if(game.found){
				if(!game.placed){												//if a piece is removed
					if(LED_IS(LED_RED, game.found_row, game.found_col)){		//if the piece is eaten
						ch_default();											//reset led
						SET_LED(LED_BLUE, game.found_row, game.found_col);		//next move must be from (piece_row, piece_col) to (found_row, found_col)
					}
					else{
						errorHandler();
					}
				}
				else{
					if(LED_IS(LED_GREEN,game.found_row, game.found_col)||LED_IS(LED_BLUE,game.found_row, game.found_col)){
						MOVE(piece_row, piece_col, game.found_row, game.found_col);
						ch_bitmap[game.found_row] = ch_bitmap[game.found_row] | (1<<game.found_col);//set to 1 the new position
						ch_bitmap[piece_row] = ch_bitmap[piece_row] & ~(1<<piece_col);//set to 0 the old position
					}
					else if (!ch_setted()) {									//if the piece is moved in its old position
						errorHandler();
					}
					ch_default();
					player = OPPONENT(player);
					game.state = WAITING;
				}
			}
			break;
	}
}

void draw(Game game){}

void resignTop(Game game){}

void resignBottom(Game game){}
