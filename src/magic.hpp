#ifndef MAGIC_H_
#define MAGIC_H_

#include "defs.hpp"

class Magic
{
	public:
		uint64_t *data;
		uint64_t mask;
		uint64_t magic;
};

class MagicInit
{
	public:
		uint64_t magic;
		int index;
};

extern uint64_t lookupTable[97264];
extern Magic bishopMagic[Squares];
extern Magic rookMagic[Squares];

void initializeMagics();

inline uint64_t bishopAttacks(int sq, uint64_t occupied)
{
	Magic * mag = &bishopMagic[sq];
	return mag->data[((occupied & mag->mask) * mag->magic) >> (64 - 9)];
}

inline uint64_t rookAttacks(int sq, uint64_t occupied)
{
	Magic * mag = &rookMagic[sq];
	return mag->data[((occupied & mag->mask) * mag->magic) >> (64 - 12)];
}

inline uint64_t queenAttacks(int sq, uint64_t occupied)
{
	return bishopAttacks(sq, occupied) | rookAttacks(sq, occupied);
}

#endif