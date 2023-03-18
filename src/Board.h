//
// Created by Bulcsú on 12/19/2020.
//

#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include "Piece.h"
#include "Move.h"
#include "Connector.h"
#include <cstdlib>

class Piece;

class Board {
public:
    short spaces[8][8]{};
    bool flipped;

    Stack moves;

    Board(bool = false, bool = false);
    ~Board();
    void drawBoard();
    void handleEvent(sf::Event&);
    bool castle(bool, sf::Vector2i&);
    bool enPassant(const Piece&, sf::Vector2i&);
    void updateBoard();
private:
    Piece *pieces[32]{};

    bool turn, playerTurn, mode;
    short promotionIndex;

    std::vector<sf::Vector2i> circles;

    bool castled;
    bool enPass;

    sf::Clock clock;
    bool change;

    void flipBoard();
    void move(int, sf::Vector2f);
    void toMove(std::string str, sf::Vector2f &pos, sf::Vector2f &dest) const;
    void undo();
    int check();
    void promote(int, int);
    static void showPromotionMenu(bool);
    static void drawCircle(sf::Vector2i);
};


#endif //CHESS_BOARD_H
