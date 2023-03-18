//
// Created by Bulcsú on 1/16/2021.
//

#ifndef CHESS_MENU_H
#define CHESS_MENU_H

#include "SFML/Graphics.hpp"
#include "Button.h"
#include "Board.h"

extern sf::RenderWindow window;

enum mainMenuState {SHOWMENU = 0, SINGLEPLAYER, MULTIPLAYER, EXIT};

class Menu {
    virtual void handleEvent(sf::Event&) {}
    virtual void updateMenu() {}
    virtual void drawMenu() {}
};

class mainMenu : public Menu {
public:
    mainMenuState state;
    Button buttons[3];

    sf::Texture title;
    sf::Texture background;

    mainMenu();
    ~mainMenu();

    void handleEvent(sf::Event&);
    void updateMenu();
    void drawMenu();

private:
    void drawTitle() const;
    void drawBackground() const;
};

#endif //CHESS_MENU_H
