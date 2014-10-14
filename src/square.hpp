#ifndef SQUARE_HPP_
#define SQUARE_HPP_

#include <cassert>

class Square
{
public:
    Square() : square(NoSquare) {};
    Square(int newSquare) : square(newSquare) {};

    enum {
        A1, B1, C1, D1, E1, F1, G1, H1,
        A2, B2, C2, D2, E2, F2, G2, H2,
        A3, B3, C3, D3, E3, F3, G3, H3,
        A4, B4, C4, D4, E4, F4, G4, H4,
        A5, B5, C5, D5, E5, F5, G5, H5,
        A6, B6, C6, D6, E6, F6, G6, H6,
        A7, B7, C7, D7, E7, F7, G7, H7,
        A8, B8, C8, D8, E8, F8, G8, H8,
        NoSquare
    };

    operator int() const { return square; }
    operator int&() { return square; }
private:
    int square;
};

// Checks if the square is okay, i.e. >= A1 and <= H8. 
inline bool isSquareOk(Square sq)
{
    return ((sq >= Square::A1) && (sq <= Square::H8));
}

// Returns the number of the file the square is on.
inline int file(Square sq)
{
    assert(isSquareOk(sq));
    return (sq % 8);
}

// Returns the number of the rank the square is on.
inline int rank(Square sq)
{
    assert(isSquareOk(sq));
    return (sq / 8);
}

#endif