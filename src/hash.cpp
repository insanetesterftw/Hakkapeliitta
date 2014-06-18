#include "hash.hpp"
#include "random.hpp"
#include "position.hpp"

array<uint64_t, Squares> pieceHash[12];
array<uint64_t, 8> materialHash[12];
array<uint64_t, 16> castlingRightsHash;
array<uint64_t, Squares> enPassantHash;
uint64_t turnHash;

void initializeHash()
{
	MT19937 rng(123456789);

	for (int i = WhitePawn; i <= BlackKing; i++)
	{
		for (int j = A1; j <= H8; j++)
		{
			pieceHash[i][j] = rng.rand64();
		}
		for (int j = 0; j < 8; j++)
		{
			materialHash[i][j] = rng.rand64();
		}
	}

	for (int i = A1; i <= H8; i++)
	{
		enPassantHash[i] = rng.rand64();
	}

	turnHash = rng.rand64();

	// Do something about this mess.
	castlingRightsHash[1] = rng.rand64();
	castlingRightsHash[2] = rng.rand64();
	castlingRightsHash[3] = castlingRightsHash[1] ^ castlingRightsHash[2];
	castlingRightsHash[4] = rng.rand64();
	castlingRightsHash[5] = castlingRightsHash[1] ^ castlingRightsHash[4];
	castlingRightsHash[6] = castlingRightsHash[2] ^ castlingRightsHash[4];
	castlingRightsHash[7] = castlingRightsHash[1] ^ castlingRightsHash[2] ^ castlingRightsHash[4];
	castlingRightsHash[8] = rng.rand64();
	castlingRightsHash[9] = castlingRightsHash[1] ^ castlingRightsHash[8];
	castlingRightsHash[10] = castlingRightsHash[2] ^ castlingRightsHash[8];
	castlingRightsHash[11] = castlingRightsHash[1] ^ castlingRightsHash[2] ^ castlingRightsHash[8];
	castlingRightsHash[12] = castlingRightsHash[4] ^ castlingRightsHash[8];
	castlingRightsHash[13] = castlingRightsHash[1] ^ castlingRightsHash[4] ^ castlingRightsHash[8];
	castlingRightsHash[14] = castlingRightsHash[2] ^ castlingRightsHash[4] ^ castlingRightsHash[8];
	castlingRightsHash[15] = castlingRightsHash[1] ^ castlingRightsHash[2] ^ castlingRightsHash[4] ^ castlingRightsHash[8];

}

uint64_t Position::calculateHash()
{
	uint64_t h = 0;
	for (int i = A1; i <= H8; i++)
	{
		if (board[i] != Empty)
		{
			h ^= pieceHash[board[i]][i];
		}
	}
	if (enPassantSquare != NoSquare)
	{
		h ^= enPassantHash[enPassantSquare];
	}
	if (sideToMove)
	{
		h ^= turnHash;
	}
	if (castlingRights)
	{
		h ^= castlingRightsHash[castlingRights];
	}

	return h;
}

uint64_t Position::calculatePawnHash()
{
	uint64_t p = 0;
	for (int i = A1; i <= H8; i++)
	{
		if (board[i] == WhitePawn || board[i] == BlackPawn)
		{
			p ^= pieceHash[board[i]][i];
		}
	}

	return p;
}

uint64_t Position::calculateMaterialHash()
{
	uint64_t material = 0;
	for (int i = WhitePawn; i <= BlackKing; i++)
	{
		for (int j = 0; j < popcnt(bitboards[i]); j++)
		{
			material ^= materialHash[i][j];
		}
	}
	return material;
}