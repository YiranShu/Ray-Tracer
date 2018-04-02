#ifndef _CHESS_BOARD_H_
#define _CHESS_BOARD_H_
#include "square.h"

class ChessBoard {
public:
	Square lightSquare[32];
	Square darkSquare[32];
	Vector normal;

	ChessBoard(Square lightSquare[], Square darkSquare[]) {
		for(int i = 0; i < 32; i++) {
			this->lightSquare[i] = lightSquare[i];
			this->darkSquare[i] = darkSquare[i];
		}
	}

	ChessBoard() {}

	float intersect(Point origin, Vector direction, Point *hit);
	bool blocked(Point origin, Vector direction);
};

#endif
