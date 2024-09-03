#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <x86intrin.h>

#define WHITE true
#define BLACK false

#define PIECES ".pkbrqkPKBRQK"

typedef enum Piece {
    EMPTY = 0,
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING,
} Piece;

static const uint64_t ranks[8] = {
    0xff,
    0xff00,
    0xff0000,
    0xff000000,
    0xff00000000,
    0xff0000000000,
    0xff000000000000,
    0xff00000000000000
};

static const uint64_t files[8] = {
    0x101010101010101,
    0x202020202020202,
    0x404040404040404,
    0x808080808080808,
    0x1010101010101010,
    0x2020202020202020,
    0x4040404040404040,
    0x8080808080808080
};

static const char* const square_names[] = {
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
};

typedef bool Color;
typedef uint8_t Square;

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
    uint8_t halfmoves;
    uint32_t fullmoves;
} Position;

typedef struct Move {
    Square from;
    Square to;
    Piece piece;
    Piece capture;
    Piece promotion;
    Castles prev_castles;
    Square ep_square;
} Move;

typedef struct MoveList {
    uint8_t count;
    uint8_t capacity;
    Move *items;
} MoveList;

Board default_board() {
    Board b = {
        .pawns = 0x00ff00000000ff00,
        .knights = 0x4200000000000042,
        .bishops = 0x2400000000000024,
        .rooks = 0x8100000000000081,
        .queens = 0x0800000000000008,
        .kings = 0x1000000000000010,

        .w_pieces = 0xffff,
        .b_pieces = 0xffff000000000000,

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

Square coords_to_square(uint8_t x, uint8_t y) {
    // Same as `rank_and_file_to_square` but zero based
    return x + y * 8;
}

Square rank_and_file_to_square(uint8_t rank, uint8_t file) {
    return (file - 1) + (rank - 1) * 8;
}

bool make_move(Position *p, Move m) {
    // Returns true if move is legal
    assert(false); // Unimplemented
}

void make_move_unchecked(Position *p, Move m) {
    // Unset to src bit and set the dst bit

    uint64_t from_bit = (1 << m.from);
    uint64_t to_bit = (1 << m.to);

    switch (m.piece) {
        case PAWN:
            p->board.pawns |= from_bit;
            break;
        case KNIGHT:
            break;
        case BISHOP:
            break;
        case ROOK:
            break;
        case QUEEN:
            break;
        case KING:
            break;
        case EMPTY:
            assert(false);
            break;
    }

    p->turn = !p->turn;
}

void unmake_move(Position *p, Move m) {
    
}

void display_bitboard(uint64_t bitboard) {
    char board_str[] =
        "X X X X X X X X\n"
        "X X X X X X X X\n"
        "X X X X X X X X\n"
        "X X X X X X X X\n"
        "X X X X X X X X\n"
        "X X X X X X X X\n"
        "X X X X X X X X\n"
        "X X X X X X X X\n";

    for (size_t rank = 0; rank < 8; ++rank) {
        for (size_t file = 0; file < 8; ++file) {
            char glyph = '.';
            uint64_t mask = ranks[rank] & files[file];
            if (mask & bitboard) {
                glyph = 'O';
            }
            board_str[file * 2 + (7 - rank) * 16] = glyph;
        }
    }

    printf("%s", board_str);
}

int popcount(uint64_t a) {
    return _popcnt64(a);
    // TODO: fallback
}

int pop_lsb(uint64_t *bb) {
    //uint64_t lsb = *bb & -*bb; // Isolate LSB
    int index = __builtin_ctzll(*bb); // Find index of LSB
    // TODO: fallback
    *bb &= *bb - 1; // Clear LSB
    return index;
}

void pawn_moves(Position *p, MoveList *move_list) {
    uint64_t empty_squares = ~p->board.occupied;

    uint64_t single_pushes = 0;
    uint64_t double_pushes = 0;
    uint64_t captures = 0;
    uint64_t en_passant = 0;

    // p->turn = BLACK;

    if (p->turn == WHITE) {
        uint64_t pawn_mask = p->board.w_pieces & p->board.pawns;
        single_pushes = (pawn_mask << 8) & empty_squares;
        double_pushes = ((single_pushes & ranks[2]) << 8) & empty_squares;
    } else {
        uint64_t pawn_mask = p->board.b_pieces & p->board.pawns;
        single_pushes = (pawn_mask >> 8) & empty_squares;
        double_pushes = ((single_pushes & ranks[5]) >> 8) & empty_squares;
    }

    while (single_pushes) {
        Square to = pop_lsb(&single_pushes);  // Get destination square
        Square from = to + p->turn * 8;
        Move move = {
            .from = from,
            .to = to,
            .piece = PAWN,
            .capture = EMPTY,
            .promotion = EMPTY,
            .prev_castles = p->castles,
            .ep_square = 0
        };
        move_list->items[move_list->count++] = move;  // Add move to the list
    }

    while (double_pushes) {
        Square to = pop_lsb(&double_pushes);  // Get destination square
        Square from = to + p->turn * 16;
        Move move = {
            .from = from,
            .to = to,
            .piece = PAWN,
            .capture = EMPTY,
            .promotion = EMPTY,
            .prev_castles = p->castles,
            .ep_square = 0
        };
        move_list->items[move_list->count++] = move;  // Add move to the list
    }
}

uint8_t generate_legal_moves(Position *p, MoveList *move_list) {
    // Returns number of legal moves
    move_list->count = 0;

    pawn_moves(p, move_list);

    return move_list->count;
}

void print_move_list(MoveList *move_list) {
    for (size_t i = 0; i < move_list->count; ++i) {
        Move move = move_list->items[i];
        printf("%s%s\n", square_names[move.from], square_names[move.to]);
    }
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

    for (size_t rank = 0; rank < 8; ++rank) {
        for (size_t file = 0; file < 8; ++file) {
            uint8_t piece = get_piece_type(&p->board, rank, file);
            Color color = get_piece_color(&p->board, rank, file);
            char glyph = PIECES[piece + color * 6];
            board_str[file * 2 + (7 - rank) * 16] = glyph;
        }
    }

    printf("%s", board_str);
}

uint64_t perft(Position *p, size_t depth) {
    assert(depth >= 1);

    MoveList move_list = {
        .count = 0,
        .capacity = 255,
        .items = (Move[255]){0}
    };
    uint64_t nodes = 0;

    uint8_t n_moves = generate_legal_moves(p, &move_list);

    if (depth == 1) {
        return n_moves;
    }

    for (size_t i = 0; i < n_moves; ++i) {
        make_move_unchecked(p, move_list.items[i]);
        nodes += perft(p, depth - 1);
        unmake_move(p, move_list.items[i]);
    }

    return nodes;
}

int main() {
    for (size_t i = 1; i < 6; ++i) {
        Position p = default_position();
        uint64_t nodes = perft(&p, i);
        printf("depth: %lu, nodes: %lu\n", i, nodes);
    }
}
