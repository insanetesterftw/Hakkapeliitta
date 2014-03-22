#ifndef POSITION_H
#define POSITION_H

#include "defs.h"
#include "bitboard.h"

class Position
{
	public:
		void initializeBoardFromFEN(string FEN);

		int getPiece(int sq);

		inline uint64_t getWhitePieces() { return bitboards[12]; }
		inline uint64_t getBlackPieces() { return bitboards[13]; }
		inline uint64_t getPieces(bool colour) { return bitboards[12 + colour]; }
		inline uint64_t getOccupiedSquares() { return bitboards[14]; }
		inline uint64_t getFreeSquares() { return bitboards[15]; }
		inline uint64_t getBitboard(bool colour, int piece) { return bitboards[piece + colour * 6]; }

		inline bool getSideToMove() { return sideToMove; }
		inline int getEnPassantSquare() { return enPassantSquare; }
		inline int getCastlingRights() { return castlingRights; }

		bool makeMove(int move);
		void unmakeMove();

	private:
		// All bitboards needed to represent the position.
		// 6 bitboards for different white pieces + 1 for all white pieces.
		// 6 bitboards for different black pieces + 1 for all black pieces.
		// 1 for all occupied squares.
		// 1 for all not-occupied squares.
		array<uint64_t, 16> bitboards;

		// Miscellaneous, everything is pretty self explanatory.
		bool sideToMove;
		int castlingRights;
		int enPassantSquare;
		int fiftyMoveDistance;
		int ply, hply;
		uint64_t hash, pawnHash;

		int initBoardFromFEN(string FEN);
};

#endif
