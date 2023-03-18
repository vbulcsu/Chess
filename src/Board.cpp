#pragma clang diagnostic push
#pragma clang diagnostic pop
#pragma ide diagnostic ignored "hicpp-multiway-paths-covered"
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"
#pragma ide diagnostic ignored "bugprone-integer-division"

//
// Created by Bulcsú on 12/19/2020.
//

#include "Board.h"
#include <iostream>

extern sf::RenderWindow window;
extern std::string message;

Board::Board(bool playingAs, bool mode) {

    connectToEngine("../stockfish.exe");

    turn = flipped = castled = enPass = change =  false;

    playerTurn = playingAs;
    this->mode = mode;

    promotionIndex = -1;

    clock.restart();

    /// PAWNS
    for(int i = 0; i < 8; ++i) {

        pieces[i] = new Pawn(sf::Vector2f(i,6), WHITE);
        pieces[16+i] = new Pawn(sf::Vector2f(i, 1), BLACK);

        for(int j = 0; j < 8; ++j) { /// SETTING THE BOARD DATA

            if(i < 2) spaces[i][j] = 2;            /// BLACK
            else if(i > 5) spaces[i][j] = 1;       /// WHITE
            else spaces[i][j] = 0;                 /// BLANK
        }
    }

    /// ROOKS
    pieces[8] = new Rook(sf::Vector2f(0,7), WHITE);
    pieces[15] = new Rook(sf::Vector2f(7,7), WHITE);
    pieces[24] = new Rook(sf::Vector2f(0,0), BLACK);
    pieces[31] = new Rook(sf::Vector2f(7,0), BLACK);

    /// KNIGHTS
    pieces[9] = new Knight(sf::Vector2f(1,7), WHITE);
    pieces[14] = new Knight(sf::Vector2f(6,7), WHITE);
    pieces[25] = new Knight(sf::Vector2f(1,0), BLACK);
    pieces[30] = new Knight(sf::Vector2f(6,0), BLACK);

    /// BISHOPS
    pieces[10] = new Bishop(sf::Vector2f(2,7), WHITE);
    pieces[13] = new Bishop(sf::Vector2f(5,7), WHITE);
    pieces[26] = new Bishop(sf::Vector2f(2,0), BLACK);
    pieces[29] = new Bishop(sf::Vector2f(5,0), BLACK);

    /// QUEENS
    pieces[11] = new Queen(sf::Vector2f(3,7), WHITE);
    pieces[27] = new Queen(sf::Vector2f(3,0), BLACK);

    /// KINGS
    pieces[12] = new King(sf::Vector2f(4,7), WHITE);
    pieces[28] = new King(sf::Vector2f(4,0), BLACK);
}

Board::~Board() {

    closeConnection();

    for(auto & piece : pieces) {

        delete piece;
    }
}

void Board::handleEvent(sf::Event &ev) {

    switch (ev.type) {
        case sf::Event::MouseButtonPressed: {

            sf::Vector2i mouse = sf::Mouse::getPosition(window);
            mouse.x /= 100;
            mouse.y /= 100;

            if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

                if(!circles.empty()) {

                    circles.clear();
                }

                if(promotionIndex >= 0) break;
                if(!mode && turn == !playerTurn) break;

                for(auto & piece : pieces) { /// activates the piece clicked

                    if(piece->isTaken) continue;
                    if(mouse.x == piece->pos.x && mouse.y == piece->pos.y && turn == piece->color) {
                        piece->isActive = true;
                        break;
                    }
                }
            }
            if(sf::Mouse::isButtonPressed(sf::Mouse::Right)) {

                circles.push_back(mouse);
            }

            break;
        }
        case sf::Event::MouseButtonReleased: {

            if(ev.key.code != sf::Mouse::Button::Left) break;

            sf::Vector2i mouse = sf::Mouse::getPosition(window);
            mouse.x /= 100;
            mouse.y /= 100;

            if(promotionIndex < 0) {
                for (int i = 0; i < 32; ++i) {

                    if (pieces[i]->isTaken) continue;
                    if (pieces[i]->isActive && pieces[i]->canMove(mouse)) {

                        /// moving the piece with index i to dest
                        move(i, (sf::Vector2f)mouse);

                        /// deactivating the piece
                        pieces[i]->isActive = false;

                        if(check()) { /// if the player is in check after his move
                            undo();
                            return;
                        }
                        if (pieces[i]->type == PAWN && (pieces[i]->pos.y == 0 || pieces[i]->pos.y == 7)) { /// piece with index i needs to be promoted
                            promotionIndex = i;
                        }
                        if (promotionIndex < 0) { /// if nothing needs to be promoted
                            turn = !turn;
                        }
                        if(mode && promotionIndex < 0) {
                            clock.restart();
                            change = true;
                        }

                        break;
                    } else if (pieces[i]->isActive) { /// if the move is nowhere near legal
                        pieces[i]->isActive = false;
                    }
                }
            }
            else if(mouse.x > 1 && mouse.x < 6 && mouse.y > 2 && mouse.y < 5) { /// handling the promotion menu

                switch (mouse.x) { /// promoting to certain type
                    case 2: {
                        promote(promotionIndex, QUEEN);
                        break;
                    }
                    case 3: {
                        promote(promotionIndex, KNIGHT);
                        break;
                    }
                    case 4: {
                        promote(promotionIndex, ROOK);
                        break;
                    }
                    case 5: {
                        promote(promotionIndex, BISHOP);
                        break;
                    }
                }
                turn = !turn;
            }

            break;
        }
        case sf::Event::KeyReleased: {

            if(ev.key.code == sf::Keyboard::Key::Space) { /// if space is pressed flipping the board

                flipBoard();
            }
            if(ev.key.code == sf::Keyboard::Key::Left) { /// if left arrow is pressed reversing a move

                undo();
                if(!mode) undo();
            }

            break;
        }
        default:
            break;
    }
}

void Board::updateBoard() {

    if(mode && change) {

        sf::Time time = clock.getElapsedTime();
        if((int)time.asMilliseconds() > 500) {

            flipBoard();
            change = false;
        }
    }

    if(!mode && turn != playerTurn) {

        std::string str = getNextMove(moves.getPosition());

        if(str == "error") {
            message += "Engine error!\n";
            return;
        }

        sf::Vector2f pos;
        sf::Vector2f dest;
        toMove(str, pos, dest);

        for(int i = 0; i < 32; ++i) {

            if(pieces[i]->isTaken) continue;
            if(pieces[i]->pos == pos && pieces[i]->color == !playerTurn && pieces[i]->canMove((sf::Vector2i)dest)) {

                move(i, dest);

                if(str.at(4) != ' ') {

                    switch (str.at(4)) {
                        case 'q':
                            promote(i, QUEEN);
                            break;
                        case 'r':
                            promote(i, ROOK);
                            break;
                        case 'k':
                            promote(i, KNIGHT);
                            break;
                        case 'b':
                            promote(i, BISHOP);
                            break;
                    }
                }

                turn = !turn;

                break;
            }
        }
    }
}

void Board::drawBoard() {

    /// CLEARING THE SCREEN
    window.clear(sf::Color(240,217,181));

    /// BACKGROUND
    for (int i = 0; i < 8; ++i) {

        for(int j = 0; j < 8; j++) {

            if((i+j)%2 == 1) { /// draw a black tile

                sf::RectangleShape rect;
                rect.setSize(sf::Vector2f(window.getSize().x/8, window.getSize().y/8));
                rect.setFillColor(sf::Color(181,136,99));
                rect.setOutlineThickness(0);
                rect.setPosition(i*window.getSize().x/8, j*window.getSize().y/8);

                window.draw(rect);
            }
        }
    }

    /// PIECES
    short isActiveIndex = -1;
    for(int i = 0; i < 32; ++i) { /// draw the pieces still on board
        if(pieces[i]->isActive) {
            isActiveIndex = i;
            continue;
        }
        if(!pieces[i]->isTaken) pieces[i]->drawPiece();
    }
    if(isActiveIndex >= 0) pieces[isActiveIndex]->drawPiece(); /// drawing the active piece on top

    /// PROMOTION MENU
    if(promotionIndex >= 0) showPromotionMenu(turn); /// if promotion needs to be done drawing the promotion menu

    /// CIRCLES
    if(!circles.empty()) { /// drawing circles
        for(auto & pos : circles) {
            drawCircle(pos);
        }
    }
}

void Board::drawCircle(sf::Vector2i pos) {

    sf::CircleShape circle;
    circle.setRadius(46);
    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineColor(sf::Color(0,150,0));
    circle.setOutlineThickness(4);
    circle.setPosition(pos.x*100+4, pos.y*100+4);
    circle.setPointCount(300);

    window.draw(circle);
}

void Board::move(int pieceIndex, sf::Vector2f dest) { /// moves the piece to the destination

    Move tmpMove;
    tmpMove.pieceIndex = pieceIndex;
    tmpMove.oldPos = pieces[pieceIndex]->pos;
    tmpMove.moved = pieces[pieceIndex]->isMoved;
    tmpMove.pieceType = pieces[pieceIndex]->type;

    for(int y = 0; y < 8; ++y) {

        for(int x = 0; x < 8; ++x) {

            tmpMove.boardData[x][y] = spaces[x][y];
        }
    }

    spaces[(int)pieces[pieceIndex]->pos.y][(int)pieces[pieceIndex]->pos.x] = 0;

    if(spaces[(int)dest.y][(int)dest.x] != 0) { /// taking the piece

        for(int i = 0; i < 32; ++i) {

            if(!pieces[i]->isTaken && pieces[i]->pos.x == dest.x && pieces[i]->pos.y == dest.y) {

                pieces[i]->isTaken = true;
                tmpMove.takenPieceIndex = i;
                break;
            }
        }
    }

    pieces[pieceIndex]->pos = dest;

    tmpMove.newPos = pieces[pieceIndex]->pos;
    tmpMove.boardTurn = turn;

    pieces[pieceIndex]->isMoved = true;
    spaces[(int)pieces[pieceIndex]->pos.y][(int)pieces[pieceIndex]->pos.x] = turn+1;

    if(castled) {
        tmpMove.special = true;
        castled = false;
    }
    if(enPass) {
        tmpMove.special = true;
        enPass = false;
    }

    moves.push(tmpMove);
}

void Board::toMove(std::string str, sf::Vector2f &pos, sf::Vector2f &dest) const {

    pos.x = str.at(0) - 'a';
    pos.y = '8' - str.at(1);

    dest.x = str.at(2) - 'a';
    dest.y = '8' - str.at(3);

    if(flipped) {
        pos.x = 7 - pos.x;
        pos.y = 7 - pos.y;
        dest.x = 7 - dest.x;
        dest.y = 7 - dest.y;
    }
}

void Board::flipBoard() { /// flips the board

    flipped  = !flipped;
    playerTurn = flipped;

    /// ERASE BOARD DATA
    for(auto & space : spaces) {

        for(short & j : space) {

            j = 0;
        }
    }

    /// INVERT POSITIONS / SET NEW DATA
    for(auto & piece : pieces) {

        if(piece->isTaken) continue;

        piece->pos.y = 7 - piece->pos.y;
        piece->pos.x = 7 - piece->pos.x;

        if(piece->color == WHITE) spaces[(int)piece->pos.y][(int)piece->pos.x] = 1;
        else if(piece->color == BLACK) spaces[(int)piece->pos.y][(int)piece->pos.x] = 2;
    }

    /// FLIPPING PREVIOUS POSITIONS
    moves.flipData();
}

int Board::check() { /// checks for active checks

    sf::Vector2i whiteKingPos;
    whiteKingPos.x = pieces[12]->pos.x;
    whiteKingPos.y = pieces[12]->pos.y;

    sf::Vector2i blackKingPos;
    blackKingPos.x = pieces[28]->pos.x;
    blackKingPos.y = pieces[28]->pos.y;

    for(auto & piece : pieces) {

        if(piece->isTaken) continue;
        if(piece->canMove(whiteKingPos) && !turn) {

            message += "White is in check!\n";
            return 1;
        }
        if(piece->canMove(blackKingPos) && turn) {

            message += "Black is in check!\n";
            return 2;
        }
    }

    return 0;
}

void Board::undo() { /// reverses a move

    Move move;
    if(!moves.pop(move)) {

        message += "Undo failed!\n";
        return;
    }

    if(move.pieceIndex > -1) {

        pieces[move.pieceIndex]->pos = move.oldPos;
        pieces[move.pieceIndex]->isMoved = move.moved;
    }
    if(move.takenPieceIndex > -1) {

        pieces[move.takenPieceIndex]->pos = move.newPos;
        pieces[move.takenPieceIndex]->isTaken = false;
    }
    if(pieces[move.pieceIndex]->type != move.pieceType) {

        sf::Vector2f pos = pieces[move.pieceIndex]->pos;
        auto color = pieces[move.pieceIndex]->color;

        delete pieces[move.pieceIndex];
        pieces[move.pieceIndex] = new Pawn(pos, color);
    }

    turn = move.boardTurn;

    for(int i = 0; i < 8; ++i) {

        for(int j = 0; j < 8; ++j) {

            spaces[i][j] = move.boardData[i][j];
        }
    }

    if(moves.getOnTop().special) undo();
}

bool Board::castle(bool color, sf::Vector2i &dest) {

    if(check()) return false;
    if(spaces[dest.y][dest.x] != 0) return false;

    switch (color) {
        case WHITE: {

            if(!flipped) {

                if(dest.x < pieces[12]->pos.x) {

                    if(pieces[8]->isMoved || pieces[8]->isTaken) return false;
                    sf::Vector2f tmp(dest.x+1, dest.y);
                    castled = true;
                    move(8, tmp);
                } else {

                    if(pieces[15]->isMoved || pieces[15]->isTaken) return false;
                    sf::Vector2f tmp(dest.x-1, dest.y);
                    castled = true;
                    move(15, tmp);
                }
            }
            else {

                if(dest.x > pieces[12]->pos.x) {

                    if(pieces[8]->isMoved || pieces[8]->isTaken) return false;
                    sf::Vector2f tmp(dest.x-1, dest.y);
                    castled = true;
                    move(8, tmp);
                } else {

                    if(pieces[15]->isMoved || pieces[15]->isTaken) return false;
                    sf::Vector2f tmp(dest.x+1, dest.y);
                    castled = true;
                    move(15, tmp);
                }
            }

            break;
        }
        case BLACK: {

            if(!flipped) {

                if(dest.x > pieces[28]->pos.x) {

                    if(pieces[31]->isMoved || pieces[31]->isTaken) return false;
                    sf::Vector2f tmp(dest.x-1, dest.y);
                    castled = true;
                    move(31, tmp);
                } else {

                    if(pieces[24]->isMoved || pieces[24]->isTaken) return false;
                    sf::Vector2f tmp(dest.x+1, dest.y);
                    castled = true;
                    move(24, tmp);
                }
            }
            else {

                if(dest.x < pieces[28]->pos.x) {

                    if(pieces[31]->isMoved || pieces[31]->isTaken) return false;
                    sf::Vector2f tmp(dest.x+1, dest.y);
                    castled = true;
                    move(31, tmp);
                } else {

                    if(pieces[24]->isMoved || pieces[24]->isTaken) return false;
                    sf::Vector2f tmp(dest.x-1, dest.y);
                    castled = true;
                    move(24, tmp);
                }
            }

            break;
        }
    }

    return true;
}

bool Board::enPassant(const Piece& piece, sf::Vector2i& dest) {

    if(moves.getOnTop().pieceType != PAWN) return false;
    if(std::abs(piece.pos.x-moves.getOnTop().newPos.x) != 1 || piece.pos.y != moves.getOnTop().newPos.y) return false;
    if(!flipped && piece.color == WHITE && (moves.getOnTop().newPos.y != 3 || moves.getOnTop().oldPos.y != 1)) return false;
    if(!flipped && piece.color == BLACK && (moves.getOnTop().newPos.y != 4 || moves.getOnTop().oldPos.y != 6)) return false;
    if(flipped && piece.color == WHITE && (moves.getOnTop().newPos.y != 4 || moves.getOnTop().oldPos.y != 6)) return false;
    if(flipped && piece.color == BLACK && (moves.getOnTop().newPos.y != 3 || moves.getOnTop().oldPos.y != 1)) return false;

    for(int i = 0; i < 32; ++i) {

        if(pieces[i]->isTaken) continue;
        if(piece.pos == pieces[i]->pos) {

            enPass = true;
            move(i, moves.getOnTop().newPos);
            break;
        }
    }

    return true;
}

void Board::promote(int pieceIndex, int type) {

    sf::Vector2f piecePos = pieces[pieceIndex]->pos;
    auto color = pieces[pieceIndex]->color;

    delete pieces[pieceIndex];

    switch (type) {
        case QUEEN: {
            pieces[pieceIndex] = new Queen(piecePos, color);
            moves.updatePos("q");
            break;
        }
        case KNIGHT: {
            pieces[pieceIndex] = new Knight(piecePos, color);
            moves.updatePos("k");
            break;
        }
        case ROOK: {
            pieces[pieceIndex] = new Rook(piecePos, color);
            moves.updatePos("r");
            break;
        }
        case BISHOP: {
            pieces[pieceIndex] = new Bishop(piecePos, color);
            moves.updatePos("b");
            break;
        }
    }

    promotionIndex = -1;
    if(mode) flipBoard();
}

void Board::showPromotionMenu(bool col) { /// draws the promotion menu

    eColor color;
    if(!col) color = WHITE;
    else color = BLACK;

    Piece *tmpPieces[4];
    tmpPieces[0] = new Queen(sf::Vector2f(2, 3.5), color);
    tmpPieces[1] = new Knight(sf::Vector2f(3, 3.5), color);
    tmpPieces[2] = new Rook(sf::Vector2f(4, 3.5), color);
    tmpPieces[3] = new Bishop(sf::Vector2f(5, 3.5), color);

    sf::RectangleShape rect;
    rect.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
    rect.setFillColor(sf::Color(0,0,0,90));
    rect.setOutlineThickness(0);
    rect.setPosition(0, 0);

    window.draw(rect);

    rect.setSize(sf::Vector2f(4.4*window.getSize().x/8, window.getSize().y/8));
    rect.setFillColor(sf::Color(210,210,210,160));
    rect.setOutlineThickness(3);
    rect.setOutlineColor(sf::Color(210,210,210,220));
    rect.setPosition(1.8*window.getSize().x/8, 3.5*window.getSize().y/8);

    window.draw(rect);

    for(auto & piece : tmpPieces) {

        piece->drawPiece();
        delete piece;
    }
}