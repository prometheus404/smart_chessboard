#define PRESENT 0xff
#define WHITE 0x01
#define BLACK 0x02
#define PAWN 0x04
#define ROOK 0x08
#define KNIGHT 0x10
#define BISHOP 0x20
#define QUEEN 0x40
#define KING 0x80
byte[] ch_bitmap = {0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff};
byte[][] ch_ext = {
    {ROOK|BLACK, KNIGHT|BLACK, BISHOP|BLACK, QUEEN|BLACK, KING|BLACK, BISHOP|BLACK, KNIGHT|BLACK, ROOK|BLACK},
    {PAWN|BLACK, PAWN|BLACK, PAWN|BLACK, PAWN|BLACK, PAWN|BLACK, PAWN|BLACK, PAWN|BLACK, PAWN|BLACK},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {PAWN|WHITE, PAWN|WHITE, PAWN|WHITE, PAWN|WHITE, PAWN|WHITE, PAWN|WHITE, PAWN|WHITE, PAWN|WHITE},
    {ROOK|WHITE, KNIGHT|WHITE, BISHOP|WHITE, QUEEN|WHITE, KING|WHITE, BISHOP|WHITE, KNIGHT|WHITE, ROOK|WHITE},
};
#define PIECE_IS(type, row, col) (ch_ext[row][col]&type)                        //equal zero if type doesn't matches, greater than zero otherwise
#define PIECE_IS_ALL(type, row, col) ((ch_ext[row][col]&type) == type)
#define OPPONENT(player) player^0x03

void ch_monitoring(){
    bool found = false;
    int row = 0, col, diff = 0;
    for(; row < 8; row++){
        diff = ch_read()^ch_bitmap[row];
        if(diff){
            if(found){
                error_handler();
                return;
            }
            for(col = 1; col <= 8; col++){
                if(!(diff >> col)){
                    find_moves(row, col-1);
                    show_moves();
                    }
            }
            found = true;
        }
    }
}

void find_moves(row, col){
    int direction = 1;
    if(!PIECE_IS(player, row, col)){
        error_handler();
        return;
    }
    switch (ch_ext[row][col]) {
        case
        case PAWN|player2:                                                      //player2 is the player on top (color doesn't matter)
            direction = -1;
        case PAWN|player1:
            if(!PIECE_IS(PRESENT, row+(1*direction), col)) SET_LED(GREEN, row+(1*direction), col);
            if(!PIECE_IS(PRESENT, row+(2*direction), col) && FIRST_MOVE(row, col)) SET_LED(GREEN, row+(1*direction), col);
            if(PIECE_IS(OPPONENT(player), row+(1*direction), col+1)) SET_LED(RED, row+(1*direction), col+1);
            if(PIECE_IS(OPPONENT(player), row+(1*direction), col-1)) SET_LED(RED, row+(1*direction), col-1);
            break;
        case ROOK|WHITE:
        case ROOK|BLACK:
            for(int i = row; i < 8; i++){
                if(!PIECE_IS(PRESENT, i, col)) SET_LED(GREEN, i, col);
                if(PIECE_IS(OPPONENT(player), i, col)){
                    SET_LED(RED, i, col);
                    break;
                }
            }
            for(int i = row; i >= 0; i--){
                if(!PIECE_IS(PRESENT, i, col)) SET_LED(GREEN, i, col);
                if(PIECE_IS(OPPONENT(player), i, col)){
                    SET_LED(RED, i, col);
                    break;
                }
            }
            for(int i = col; i < 8; i++){
                if(!PIECE_IS(PRESENT, row, i)) SET_LED(GREEN, row, i);
                if(PIECE_IS(OPPONENT(player), row, i)){
                    SET_LED(RED, row, i);
                    break;
                }
            }
            for(int i = col; i >= 0; i--){
                if(!PIECE_IS(PRESENT, row, i)) SET_LED(GREEN, row, i);
                if(PIECE_IS(OPPONENT(player), row, i)){
                    SET_LED(RED, row, i);
                    break;
                }
            }
            break;
        case KNIGHT|BLACK:
        case KNIGHT|WHITE:
            if(!PIECE_IS(PRESENT, row + 1, col + 2)) SET_LED(GREEN, row + 1, col + 2);
            if(!PIECE_IS(PRESENT, row + 1, col - 2)) SET_LED(GREEN, row + 1, col - 2);
            if(!PIECE_IS(PRESENT, row - 1, col + 2)) SET_LED(GREEN, row - 1, col + 2);
            if(!PIECE_IS(PRESENT, row - 1, col - 2)) SET_LED(GREEN, row - 1, col - 2);

            if(!PIECE_IS(PRESENT, row + 2, col + 1)) SET_LED(GREEN, row + 2, col + 1);
            if(!PIECE_IS(PRESENT, row + 2, col - 1)) SET_LED(GREEN, row + 2, col - 1);
            if(!PIECE_IS(PRESENT, row - 2, col + 1)) SET_LED(GREEN, row - 2, col + 1);
            if(!PIECE_IS(PRESENT, row - 2, col - 1)) SET_LED(GREEN, row - 2, col - 1);

            if(PIECE_IS(OPPONENT(player), row + 1, col + 2)) SET_LED(RED, row + 1, col + 2);
            if(PIECE_IS(OPPONENT(player), row + 1, col - 2)) SET_LED(RED, row + 1, col - 2);
            if(PIECE_IS(OPPONENT(player), row - 1, col + 2)) SET_LED(RED, row - 1, col + 2);
            if(PIECE_IS(OPPONENT(player), row - 1, col - 2)) SET_LED(RED, row - 1, col - 2);

            if(PIECE_IS(OPPONENT(player), row + 2, col + 1)) SET_LED(RED, row + 2, col + 1);
            if(PIECE_IS(OPPONENT(player), row + 2, col - 1)) SET_LED(RED, row + 2, col - 1);
            if(PIECE_IS(OPPONENT(player), row - 2, col + 1)) SET_LED(RED, row - 2, col + 1);
            if(PIECE_IS(OPPONENT(player), row - 2, col - 1)) SET_LED(RED, row - 2, col - 1);
            break;
        case KING|BLACK:
        case KING|WHITE:
            break;
        case QUEEN|BLACK:
        case QUEEN|WHITE:
            for(int i = row; i < 8; i++){
                if(!PIECE_IS(PRESENT, i, col)) SET_LED(GREEN, i, col);
                if(PIECE_IS(OPPONENT(player), i, col)){
                    SET_LED(RED, i, col);
                    break;
                }
            }
            for(int i = row; i >= 0; i--){
                if(!PIECE_IS(PRESENT, i, col)) SET_LED(GREEN, i, col);
                if(PIECE_IS(OPPONENT(player), i, col)){
                    SET_LED(RED, i, col);
                    break;
                }
            }
            for(int i = col; i < 8; i++){
                if(!PIECE_IS(PRESENT, row, i)) SET_LED(GREEN, row, i);
                if(PIECE_IS(OPPONENT(player), row, i)){
                    SET_LED(RED, row, i);
                    break;
                }
            }
            for(int i = col; i >= 0; i--){
                if(!PIECE_IS(PRESENT, row, i)) SET_LED(GREEN, row, i);
                if(PIECE_IS(OPPONENT(player), row, i)){
                    SET_LED(RED, row, i);
                    break;
                }
            }
        case BISHOP|BLACK:
        case BISHOP|WHITE:
            for(int i = 0; row + i < 8 && col + i < 8; i++){
                if(!PIECE_IS(PRESENT, row + i, col + i)) SET_LED(GREEN, row + i, col + i);
                if(PIECE_IS(OPPONENT(player), row + i, col + i)) SET_LED(RED, row + i, col + i);
            }
            for(int i = 0; row + i < 8 && col - i > 0; i++){
                if(!PIECE_IS(PRESENT, row + i, col - i)) SET_LED(GREEN, row + i, col - i);
                if(PIECE_IS(OPPONENT(player), row + i, col - i)) SET_LED(RED, row + i, col - i);
            }
            for(int i = 0; row - i > 0 && col + i < 8; i++){
                if(!PIECE_IS(PRESENT, row - i, col + i)) SET_LED(GREEN, row - i, col + i);
                if(PIECE_IS(OPPONENT(player), row - i, col + i)) SET_LED(RED, row - i, col + i);
            }
            for(int i = 0; row - i > 0 && col - i > 0; i++){
                if(!PIECE_IS(PRESENT, row - i, col - i)) SET_LED(GREEN, row - i, col - i);
                if(PIECE_IS(OPPONENT(player), row - i, col - i)) SET_LED(RED, row - i, col - i);
            }
            break;
    }
}

bool suicidal_move(old_row, old_col, new_row, new_col){
	//TODO disable interrupt
	backup = ch_ext[new_row][new_col];
	ch_ext[new_row][new_col] = ch_ext[old_row][old_col];
	ch_ext[old_row][old_col] = 0x00;

	bool result = check(player);

	ch_ext[old_row][old_col] = ch_ext[new_row][new_col];
	ch_ext[new_row][new_col] = 	backup;
	//TODO enable interrupt

	return result;
}

bool check(player){
	//find king
	int k_row, k_col;
	for(int row = 0; col < 8; col++){
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
			if(PIECE_IS(QUEEN|ROOK, k_row + i, k_col) && PIECE_IS(OPPONENT(player))) return true;
			break;
		}
	}
	//down
	for(int i = 0; i < k_row; i++){
		if(PIECE_IS(PRESENT, k_row - i, k_col)){
			if(i == 1 && PIECE_IS_ALL(KING|OPPONENT(player), k_row - i, k_col)) return true;
			if(PIECE_IS(QUEEN|ROOK, k_row - i, k_col) && PIECE_IS(OPPONENT(player))) return true;
			break;
		}
	}
	//left
	for(int i = 0; i < k_col; i++){
		if(PIECE_IS(PRESENT, k_row, k_col - i)){
			if(i == 1 && PIECE_IS_ALL(KING|OPPONENT(player), k_row, k_col - i)) return true;
			if(PIECE_IS(QUEEN|ROOK, k_row, k_col - i) && PIECE_IS(OPPONENT(player))) return true;
			break;
		}
	}
	//right
	for(int i = 0; i < 8 - k_col; i++){
		if(PIECE_IS(PRESENT, k_row, k_col + i)){
			if(i == 1 && PIECE_IS_ALL(KING|OPPONENT(player), k_row, k_col + i)) return true;
			if(PIECE_IS(QUEEN|ROOK, k_row, k_col + i) && PIECE_IS(OPPONENT(player))) return true;
			break;
		}
	}
	//up-left
	for(int i = 0; i < 8 - k_row &&  i < k_col; i++){
		if(PIECE_IS(PRESENT, k_row + i, k_col - i)){
			if(i == 1 && PIECE_IS_ALL(KING|OPPONENT(player), k_row + i, k_col - i)) return true;
			if(PIECE_IS(QUEEN|BISHOP, k_row + i, k_col - i) && PIECE_IS(OPPONENT(player))) return true;
			break;
		}
	}
	//up-right
	for(int i = 0; i < 8 - k_row &&  i < 8 - k_col; i++){
	   if(PIECE_IS(PRESENT, k_row + i, k_col + i)){
		   if(i == 1 && PIECE_IS_ALL(KING|OPPONENT(player), k_row + i, k_col + i)) return true;
		   if(PIECE_IS(QUEEN|BISHOP, k_row + i, k_col + i) && PIECE_IS(OPPONENT(player))) return true;
		   break;
	   }
   }
	//down-left
	for(int i = 0; i < k_row &&  i < k_col; i++){
		if(PIECE_IS(PRESENT, k_row - i, k_col - i)){
			if(i == 1 && PIECE_IS_ALL(KING|OPPONENT(player), k_row - i, k_col - i)) return true;
			if(PIECE_IS(QUEEN|BISHOP, k_row - i, k_col - i) && PIECE_IS(OPPONENT(player))) return true;
			break;
		}
	}
	//down-right
	for(int i = 0; i < k_row &&  i < 8 - k_col; i++){
		if(PIECE_IS(PRESENT, k_row - i, k_col + i)){
			if(i == 1 && PIECE_IS_ALL(KING|OPPONENT(player), k_row - i, k_col + i)) return true;
			if(PIECE_IS(QUEEN|BISHOP, k_row - i, k_col + i) && PIECE_IS(OPPONENT(player))) return true;
			break;
		}
	}
	//pawn
	//TODO pawn check
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
