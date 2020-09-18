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
#define PIECE_IS(type, row, col) (ch_ext[row][col]&type)                        //equal zero if type matches
