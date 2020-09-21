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
            if(PIECE_IS(player^0b00000011, row+(1*direction), col+1)) SET_LED(RED, row+(1*direction), col+1);
            if(PIECE_IS(player^0b00000011, row+(1*direction), col-1)) SET_LED(RED, row+(1*direction), col-1);
            break;
        case ROOK|WHITE:
        case ROOK|BLACK:
            for(int i = row; i < 8; i++){
                if(!PIECE_IS(PRESENT, i, col)) SET_LED(GREEN, i, col);
                if(PIECE_IS(player^0b00000011, i, col)){
                    SET_LED(RED, i, col);
                    break;
                }
            }
            for(int i = row; i >= 0; i--){
                if(!PIECE_IS(PRESENT, i, col)) SET_LED(GREEN, i, col);
                if(PIECE_IS(player^0b00000011, i, col)){
                    SET_LED(RED, i, col);
                    break;
                }
            }
            for(int i = col; i < 8; i++){
                if(!PIECE_IS(PRESENT, row, i)) SET_LED(GREEN, row, i);
                if(PIECE_IS(player^0b00000011, row, i)){
                    SET_LED(RED, row, i);
                    break;
                }
            }
            for(int i = col; i >= 0; i--){
                if(!PIECE_IS(PRESENT, row, i)) SET_LED(GREEN, row, i);
                if(PIECE_IS(player^0b00000011, row, i)){
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

            if(PIECE_IS(player^0b00000011, row + 1, col + 2)) SET_LED(RED, row + 1, col + 2);
            if(PIECE_IS(player^0b00000011, row + 1, col - 2)) SET_LED(RED, row + 1, col - 2);
            if(PIECE_IS(player^0b00000011, row - 1, col + 2)) SET_LED(RED, row - 1, col + 2);
            if(PIECE_IS(player^0b00000011, row - 1, col - 2)) SET_LED(RED, row - 1, col - 2);

            if(PIECE_IS(player^0b00000011, row + 2, col + 1)) SET_LED(RED, row + 2, col + 1);
            if(PIECE_IS(player^0b00000011, row + 2, col - 1)) SET_LED(RED, row + 2, col - 1);
            if(PIECE_IS(player^0b00000011, row - 2, col + 1)) SET_LED(RED, row - 2, col + 1);
            if(PIECE_IS(player^0b00000011, row - 2, col - 1)) SET_LED(RED, row - 2, col - 1);
            break;
        case KING|BLACK:
        case KING|WHITE:
            break;
        case QUEEN|BLACK:
        case QUEEN|WHITE:
            for(int i = row; i < 8; i++){
                if(!PIECE_IS(PRESENT, i, col)) SET_LED(GREEN, i, col);
                if(PIECE_IS(player^0b00000011, i, col)){
                    SET_LED(RED, i, col);
                    break;
                }
            }
            for(int i = row; i >= 0; i--){
                if(!PIECE_IS(PRESENT, i, col)) SET_LED(GREEN, i, col);
                if(PIECE_IS(player^0b00000011, i, col)){
                    SET_LED(RED, i, col);
                    break;
                }
            }
            for(int i = col; i < 8; i++){
                if(!PIECE_IS(PRESENT, row, i)) SET_LED(GREEN, row, i);
                if(PIECE_IS(player^0b00000011, row, i)){
                    SET_LED(RED, row, i);
                    break;
                }
            }
            for(int i = col; i >= 0; i--){
                if(!PIECE_IS(PRESENT, row, i)) SET_LED(GREEN, row, i);
                if(PIECE_IS(player^0b00000011, row, i)){
                    SET_LED(RED, row, i);
                    break;
                }
            }
        case BISHOP|BLACK:
        case BISHOP|WHITE:
            for(int i = 0; row + i < 8 && col + i < 8; i++){
                if(!PIECE_IS(PRESENT, row + i, col + i)) SET_LED(GREEN, row + i, col + i);
                if(PIECE_IS(player^0b00000011, row + i, col + i)) SET_LED(RED, row + i, col + i);
            }
            for(int i = 0; row + i < 8 && col - i > 0; i++){
                if(!PIECE_IS(PRESENT, row + i, col - i)) SET_LED(GREEN, row + i, col - i);
                if(PIECE_IS(player^0b00000011, row + i, col - i)) SET_LED(RED, row + i, col - i);
            }
            for(int i = 0; row - i > 0 && col + i < 8; i++){
                if(!PIECE_IS(PRESENT, row - i, col + i)) SET_LED(GREEN, row - i, col + i);
                if(PIECE_IS(player^0b00000011, row - i, col + i)) SET_LED(RED, row - i, col + i);
            }
            for(int i = 0; row - i > 0 && col - i > 0; i++){
                if(!PIECE_IS(PRESENT, row - i, col - i)) SET_LED(GREEN, row - i, col - i);
                if(PIECE_IS(player^0b00000011, row - i, col - i)) SET_LED(RED, row - i, col - i);
            }
            break;
    }
}
