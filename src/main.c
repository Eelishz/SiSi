#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define WHITE true;
#define BLACK false;

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
} Board ;

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
        .turn = true,
        .castles = castles,
        .ep_squares = 0x0,
        .halfmoves = 0,
        .fullmoves = 1,
    };

    return p;
}

void display_board(Position *position) {
    
}

int main() {
    printf("hello world!!!\n");
}
