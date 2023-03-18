//
// Created by Bulcsú on 12/25/2020.
//

#ifndef CHESS_MOVE_H
#define CHESS_MOVE_H

#include "SFML/Graphics.hpp"
#include "string"

struct Move {
    int pieceIndex;
    int pieceType;
    sf::Vector2f oldPos;
    sf::Vector2f newPos;
    int takenPieceIndex;
    bool boardTurn, moved, special;
    short boardData[8][8]{};

    Move();
    Move& operator = (Move);
};

class Stack {
    Move array[1000];
    int index;
    std::string position;

    bool isFlipped;
public:
    Stack();
    std::string getPosition();
    Move getOnTop();

    void push(Move&);
    bool pop(Move&);

    bool isEmpty() const;
    bool isFull() const;

    void flipData();
    void updatePos(const char*);
};

#endif //CHESS_MOVE_H
