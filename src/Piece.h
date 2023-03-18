//
// Created by Bulcsú on 12/19/2020.
//

#ifndef CHESS_PIECE_H
#define CHESS_PIECE_H

#include "SFML/Graphics.hpp"
#include "Board.h"

class Board;
extern Board *board;

enum eColor {WHITE = 0, BLACK};
enum eType {KING, QUEEN, BISHOP, ROOK, KNIGHT, PAWN};

class Piece {
public:
    sf::Vector2f pos;
    eColor color;
    eType type;
    sf::Texture texture;

    bool isTaken, isActive, isMoved;

    Piece();
    ~Piece();
    void drawPiece() const;
    virtual bool canMove(sf::Vector2i) { return true; }

protected:
    bool checkDiagonals(sf::Vector2i) const;
    bool checkLines(sf::Vector2i) const;
};

class King : public Piece {
public:
    King(sf::Vector2f, eColor);
    bool canMove(sf::Vector2i);
};

class Queen : public Piece {
public:
    Queen(sf::Vector2f, eColor);
    bool canMove(sf::Vector2i);
};

class Bishop : public Piece {
public:
    Bishop(sf::Vector2f, eColor);
    bool canMove(sf::Vector2i);
};

class Rook : public Piece {
public:
    Rook(sf::Vector2f, eColor);
    bool canMove(sf::Vector2i);
};

class Knight : public Piece {
public:
    Knight(sf::Vector2f, eColor);
    bool canMove(sf::Vector2i);
};

class Pawn : public Piece {
public:
    Pawn(sf::Vector2f, eColor);
    bool canMove(sf::Vector2i);
};

#endif //CHESS_PIECE_H
