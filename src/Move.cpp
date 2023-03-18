#pragma clang diagnostic pop
#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"

//
// Created by Bulcsú on 12/25/2020.
//

#include "Move.h"
#include <iostream>

Move::Move() {

    pieceIndex = -1;
    pieceType = -1;
    takenPieceIndex = -1;
    newPos = oldPos = sf::Vector2f (-1,-1);
    boardTurn = moved = special = false;

    for(auto & i : this->boardData) {

        for(short & j : i) {

            j = 0;
        }
    }
}

Move& Move::operator = (Move move) {

    this->pieceIndex = move.pieceIndex;
    this->pieceType = move.pieceType;
    this->takenPieceIndex = move.takenPieceIndex;
    this->newPos = move.newPos;
    this->oldPos = move.oldPos;
    this->boardTurn = move.boardTurn;
    this->moved = move.moved;
    this->special = move.special;

    for(int i = 0; i < 8; ++i) {

        for(int j = 0; j < 8; ++j) {

            this->boardData[i][j] = move.boardData[i][j];
        }
    }

    return *this;
}

Stack::Stack() {

    index = 0;
    position = "";
    isFlipped = false;
}

std::string Stack::getPosition() {

    return position;
}

Move Stack::getOnTop() {

    return array[index-1];
}

void Stack::push(Move &move) {

    if(!isFull()) {
        array[index++] = move;

        if(!move.special) {

            if(!position.empty()) position += " ";

            auto oldPosTmp = (sf::Vector2i)move.oldPos;
            auto newPosTmp = (sf::Vector2i)move.newPos;

            if(isFlipped) {

                oldPosTmp.x = 7-oldPosTmp.x;
                oldPosTmp.y = 7-oldPosTmp.y;

                newPosTmp.x = 7-newPosTmp.x;
                newPosTmp.y = 7-newPosTmp.y;
            }

            char oldPs[3];
            char newPs[3];

            oldPs[0] = oldPosTmp.x + 'a';
            oldPs[1] = '8' - oldPosTmp.y;
            oldPs[2] = '\0';

            newPs[0] = newPosTmp.x + 'a';
            newPs[1] = '8' - newPosTmp.y;
            newPs[2] = '\0';

            position += oldPs;
            position += newPs;
        }
    }
}

bool Stack::pop(Move &move) {

    if(!isEmpty()) {
        move = array[--index];
        for(int i = 0; i < 5; ++i) {

            if(!position.empty()) position.pop_back();
        }

        if(!position.empty() && position.at(position.length()-1) == ' ') position.pop_back();

        return true;
    }
    return false;
}

bool Stack::isFull() const {

    return index == 200;
}

bool Stack::isEmpty() const {

    return index == 0;
}

void Stack::flipData() {

    isFlipped = !isFlipped;

    /// ERASE BOARD DATA
    for(auto & piece : array) {

        short tmp[8][8];

        for (int i = 0; i < 8; ++i) {

            for (int j = 0; j < 8; ++j) {

                tmp[i][j] = piece.boardData[i][j];
            }
        }

        for(int i = 0; i < 8; ++i) {

            for(int j = 0; j < 8; ++j) {

                piece.boardData[i][j] = tmp[7-i][7-j];
            }
        }

        piece.oldPos.y = 7 - piece.oldPos.y;
        piece.oldPos.x = 7 - piece.oldPos.x;

        piece.newPos.x = 7 - piece.newPos.x;
        piece.newPos.y = 7 - piece.newPos.y;
    }
}

void Stack::updatePos(const char *str) {

    position += str;
}