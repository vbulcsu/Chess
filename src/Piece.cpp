#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"
#pragma ide diagnostic ignored "bugprone-branch-clone"
//
// Created by Bulcsú on 12/19/2020.
//

#include "Piece.h"
#include <cstdio>

extern sf::RenderWindow window;

Piece::Piece() {

    pos = sf::Vector2f (0,0);
    color = WHITE;
    type = PAWN;
    texture.setSmooth(true);

    isTaken = isActive = isMoved = false;
}

Piece::~Piece() {


}

bool Piece::checkDiagonals(sf::Vector2i dest) const {

    if(pos.x - dest.x > 0 && pos.y - dest.y > 0) {

        int j = pos.x-1;
        int i = pos.y-1;

        while(i != dest.y) {

            if(board->spaces[i][j] != 0) return false;

            i--;
            j--;
        }
    }
    else if(pos.x - dest.x < 0 && pos.y - dest.y < 0) {

        int j = pos.x+1;
        int i = pos.y+1;

        while(i != dest.y) {

            if(board->spaces[i][j] != 0) return false;

            i++;
            j++;
        }
    }
    else if(pos.x - dest.x > 0 && pos.y - dest.y < 0) {

        int j = pos.x-1;
        int i = pos.y+1;

        while(i != dest.y) {

            if(board->spaces[i][j] != 0) return false;

            i++;
            j--;
        }
    }
    else if(pos.x - dest.x < 0 && pos.y - dest.y > 0) {

        int j = pos.x+1;
        int i = pos.y-1;

        while(i != dest.y) {

            if(board->spaces[i][j] != 0) return false;

            i--;
            j++;
        }
    }

    return true;
}

bool Piece::checkLines(sf::Vector2i dest) const {

    if(pos.x == dest.x) {

        if(pos.y - dest.y > 0) {

            for(int i = pos.y-1; i != dest.y; i--) {

                if(board->spaces[i][dest.x] != 0) return false;
            }
        }
        else {

            for(int i = pos.y+1; i != dest.y; i++) {

                if(board->spaces[i][dest.x] != 0) return false;
            }
        }
    }
    else if(pos.y == dest.y) {

        if(pos.x - dest.x > 0) {

            for(int i = pos.x-1; i != dest.x; i--) {

                if(board->spaces[dest.y][i] != 0) return false;
            }
        }
        else {

            for(int i = pos.x+1; i != dest.x; i++) {

                if(board->spaces[dest.y][i] != 0) return false;
            }
        }
    }

    return true;
}

void Piece::drawPiece() const {

    sf::Sprite tmp;

    tmp.setTexture(texture);

    if(color == WHITE) {
        tmp.setTextureRect(sf::IntRect(0, 0, texture.getSize().x,texture.getSize().y/2));
    } else {
        tmp.setTextureRect(sf::IntRect(0, texture.getSize().y/2, texture.getSize().x,texture.getSize().y/2));
    }

    tmp.scale(0.5,0.46838);
    tmp.setPosition(pos.x*100, pos.y*100);

    if(isActive) {
        sf::Color col = tmp.getColor();
        col.a = 80;
        tmp.setColor(col);
        window.draw(tmp);

        col.a = 255;
        tmp.setColor(col);
        tmp.setPosition(sf::Mouse::getPosition(window).x - 50, sf::Mouse::getPosition(window).y - 50);
    }

    window.draw(tmp);
}

King::King(sf::Vector2f pos, eColor col) {
    this->pos = pos;
    this->color = col;
    this->type = KING;

    if(!texture.loadFromFile("../resources/king.png")) {

        printf("Sprite didn't load!");
        exit(1);
    }
}

Queen::Queen(sf::Vector2f pos, eColor col) {
    this->pos = pos;
    this->color = col;
    this->type = QUEEN;

    if(!texture.loadFromFile("../resources/queen.png")) {

        printf("Sprite didn't load!");
        exit(1);
    }
}

Bishop::Bishop(sf::Vector2f pos, eColor col) {
    this->pos = pos;
    this->color = col;
    this->type = BISHOP;

    if(!texture.loadFromFile("../resources/bishop.png")) {

        printf("Sprite didn't load!");
        exit(1);
    }
}

Rook::Rook(sf::Vector2f pos, eColor col) {
    this->pos = pos;
    this->color = col;
    this->type = ROOK;

    if(!texture.loadFromFile("../resources/rook.png")) {

        printf("Sprite didn't load!");
        exit(1);
    }
}

Knight::Knight(sf::Vector2f pos, eColor col) {
    this->pos = pos;
    this->color = col;
    this->type = KNIGHT;

    if(!texture.loadFromFile("../resources/knight.png")) {

        printf("Sprite didn't load!");
        exit(1);
    }
}

Pawn::Pawn(sf::Vector2f pos, eColor col) {
    this->pos = pos;
    this->color = col;
    this->type = PAWN;

    if(!texture.loadFromFile("../resources/pawn.png")) {

        printf("Sprite didn't load!");
        exit(1);
    }
}

bool King::canMove(sf::Vector2i dest) {

    if(pos.x == dest.x && pos.y == dest.y) return false;
    if(color == WHITE && board->spaces[dest.y][dest.x] == 1) return false;
    else if(color == BLACK && board->spaces[dest.y][dest.x] == 2) return false;

    if(std::abs(pos.y - dest.y) > 1 || std::abs(pos.x - dest.x) > 2) return false;
    if(isMoved && std::abs(pos.x - dest.x) > 1) return false;
    if(!checkLines(dest)) return false;

    if(std::abs(pos.x-dest.x) > 1 && !board->castle(color, dest)) return false;

    return true;
}

bool Queen::canMove(sf::Vector2i dest) {

    if(pos.x == dest.x && pos.y == dest.y) return false;
    if(color == WHITE && board->spaces[dest.y][dest.x] == 1) return false;
    else if(color == BLACK && board->spaces[dest.y][dest.x] == 2) return false;

    if(pos.x != dest.x && pos.y != dest.y && abs(pos.x-dest.x) != abs(pos.y-dest.y)) return false;

    if(!checkDiagonals(dest)) return false;
    return checkLines(dest);
}

bool Bishop::canMove(sf::Vector2i dest) {

    if(pos.x == dest.x && pos.y == dest.y) return false;
    if(color == WHITE && board->spaces[dest.y][dest.x] == 1) return false;
    else if(color == BLACK && board->spaces[dest.y][dest.x] == 2) return false;

    if(pos.x == dest.x || pos.y == dest.y) return false;
    if(pos.x != dest.x && pos.y != dest.y && abs(pos.x-dest.x) != abs(pos.y-dest.y)) return false;
    return checkDiagonals(dest);
}

bool Rook::canMove(sf::Vector2i dest) {

    if(pos.x == dest.x && pos.y == dest.y) return false;
    if(color == WHITE && board->spaces[dest.y][dest.x] == 1) return false;
    else if(color == BLACK && board->spaces[dest.y][dest.x] == 2) return false;

    if(pos.x != dest.x && pos.y != dest.y) return  false;
    return checkLines(dest);
}

bool Knight::canMove(sf::Vector2i dest) {

    if(pos.x == dest.x && pos.y == dest.y) return false;
    if(color == WHITE && board->spaces[dest.y][dest.x] == 1) return false;
    else if(color == BLACK && board->spaces[dest.y][dest.x] == 2) return false;

    if(pos.x == dest.x || pos.y == dest.y) return false;
    if(std::abs(pos.x-dest.x) == std::abs(pos.y-dest.y)) return false;
    if((pos.x - dest.x) * (pos.x - dest.x) + (pos.y - dest.y) * (pos.y - dest.y) > 5) return false;

    return true;
}

bool Pawn::canMove(sf::Vector2i dest) {

    if(pos.y == dest.y) return false;
    if(color == WHITE && board->spaces[dest.y][dest.x] == 1) return false;
    else if(color == BLACK && board->spaces[dest.y][dest.x] == 2) return false;

    if(std::abs(pos.y-dest.y) > 2 || std::abs(pos.x-dest.x) > 1) return false;
    if(board->spaces[dest.y][dest.x] != 0 && std::abs(pos.y-dest.y) != 1) return false;
    if(board->spaces[dest.y][dest.x] != 0 && pos.x == dest.x) return false;
    if(isMoved && std::abs(pos.y - dest.y) == 2) return false;

    if(board->spaces[dest.y][dest.x] == 0 && pos.x != dest.x) {

        if(!board->enPassant(*this, dest)) return false;
    }

    if(!board->flipped) {

        if(color == WHITE && pos.y-dest.y < 0) return false;
        else if(color == BLACK && pos.y-dest.y > 0) return false;

        if(color == WHITE && pos.y-dest.y == 2 && board->spaces[dest.y+1][dest.x] != 0) return false;
        else if(color == BLACK && pos.y-dest.y == -2 && board->spaces[dest.y-1][dest.x] != 0) return false;
    }
    else {

        if(color == WHITE && pos.y-dest.y > 0) return false;
        else if(color == BLACK && pos.y-dest.y < 0) return false;

        if(color == WHITE && pos.y-dest.y == -2 && board->spaces[dest.y-1][dest.x] != 0) return false;
        else if(color == BLACK && pos.y-dest.y == 2 && board->spaces[dest.y+1][dest.x] != 0) return false;
    }

    return true;
}
#pragma clang diagnostic pop