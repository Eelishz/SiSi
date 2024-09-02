#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define WHITE true
#define BLACK false

#define EMPTY 0
#define PAWN 1
#define KNIGHT 2
#define BISHOP 3
#define ROOK 4
#define QUEEN 5
#define KING 6

#define PIECES ".pkbrqkPKBRQK"

static uint64_t ranks[8] = { 0x101010101010101,
                             0x202020202020202,
                             0x404040404040404,
                             0x808080808080808,
                             0x1010101010101010,
                             0x2020202020202020,
                             0x4040404040404040,
                             0x8080808080808080 };

static uint64_t files[8] = { 0xff,
                             0xff00,
                             0xff0000,
                             0xff000000,
                             0xff00000000,
                             0xff0000000000,
                             0xff000000000000,
                             0xff00000000000000 };

typedef bool Color;

typedef struct Board {
    uint64_t pawns;
    uint64_t knights;
    uint64_t bishops;
    uint64_t rooks;
    uint64_t queens;
    uint64_t kings;

    uint64_t w_pieces;
    uint64_t b_pieces;

    uint64_t occupied;
} Board;

typedef struct Castles {
    bool w_kingside;
    bool w_queenside;
    bool b_kingside;
    bool b_queenside;
} Castles;

typedef struct Position {
    Board board;
    Color turn;
    Castles castles;
    uint64_t ep_squares;
    uint32_t halfmoves;
    uint32_t fullmoves;
} Position;

typedef struct Move {
    uint8_t from;
    uint8_t to;
} Move;

Board default_board() {
    Board b = {
        .pawns = 0x00ff00000000ff00,
        .knights = 0x4200000000000042,
        .bishops = 0x2400000000000024,
        .rooks = 0x8100000000000081,
        .queens = 0x0800000000000008,
        .kings = 0x1000000000000010,

        .w_pieces = 0xffff000000000000,
        .b_pieces = 0xffff,

        .occupied = 0xffff00000000ffff
    };

    return b;
}

Position default_position() {
    Castles castles = {
        .w_kingside = true,
        .w_queenside = true,
        .b_kingside = true,
        .b_queenside = true,
    };

    Position p = {
        .board = default_board(),
        .turn = WHITE,
        .castles = castles,
        .ep_squares = 0x0,
        .halfmoves = 0,
        .fullmoves = 1,
    };

    return p;
}

uint8_t get_piece_color(Board *b, uint8_t rank, uint8_t file) {
    uint64_t mask = ranks[rank] & files[file];

    if (b->w_pieces & mask) {
        return WHITE;
    }
    return BLACK;
}

uint8_t get_piece_type(Board *b, uint8_t rank, uint8_t file) {
    uint64_t mask = ranks[rank] & files[file];

    if (b->pawns & mask) {
        return PAWN;
    }
    else if (b->knights & mask) {
        return KNIGHT;
    }
    else if (b->rooks & mask) {
        return ROOK;
    }
    else if (b->bishops & mask) {
        return BISHOP;
    }
    else if (b->queens & mask) {
        return QUEEN;
    }
    else if (b->kings & mask) {
        return KING;
    }
    return EMPTY;
}

void display_board(Position *p) {
    char board_str[] =
        "X X X X X X X X\n"
        "X X X X X X X X\n"
        "X X X X X X X X\n"
        "X X X X X X X X\n"
        "X X X X X X X X\n"
        "X X X X X X X X\n"
        "X X X X X X X X\n"
        "X X X X X X X X\n";

    for (size_t file = 0; file < 8; file++) {
        for (size_t rank = 0; rank < 8; rank++) {
            uint8_t piece = get_piece_type(&p->board, rank, file);
            Color color = get_piece_color(&p->board, rank, file);
            char glyph = PIECES[piece + color * 6];
            board_str[rank * 2 + file * 16] = glyph;
        }
    }

    printf("%s", board_str);
}

int main() {
    Position p = default_position();
    display_board(&p);
    printf("%lu\n", sizeof(Position));
}
