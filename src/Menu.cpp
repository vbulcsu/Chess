//
// Created by Bulcsú on 1/16/2021.
//

#include "Menu.h"
#include "windows.h"

extern Board *board;

mainMenu::mainMenu() {

    state = SHOWMENU;

    buttons[0].setButton(sf::Vector2f(window.getSize().x / 2 - 100, window.getSize().y / 3 - 35), 200, 70, "Singleplayer", "../resources/CalibriRegular.ttf", "../resources/Button");
    buttons[1].setButton(sf::Vector2f(window.getSize().x / 2 - 100, window.getSize().y / 3 - 35 + 100), 200, 70, "Multiplayer", "../resources/CalibriRegular.ttf", "../resources/Button");
    buttons[2].setButton(sf::Vector2f(window.getSize().x / 2 - 100, window.getSize().y / 3 - 35 + 200), 200, 70, "Exit", "../resources/CalibriRegular.ttf", "../resources/Button");

    if(!title.loadFromFile("../resources/title.png") || !background.loadFromFile("../resources/background.png")) {

        printf("Textures failed loading!\n");
        exit(1);
    }
    title.setSmooth(true);
    background.setSmooth(true);
}

mainMenu::~mainMenu() {
    delete board;
}

void mainMenu::handleEvent(sf::Event &ev) {

    switch (ev.type) {
        case sf::Event::MouseButtonPressed:
        case sf::Event::MouseButtonReleased: {

            switch (state) {
                case SHOWMENU: {
                    for(auto & button : buttons) {

                        button.handleEvent(ev);
                    }
                    break;
                }
                case SINGLEPLAYER:
                case MULTIPLAYER: {
                    board->handleEvent(ev);
                    break;
                }
            }

            break;
        }
        case sf::Event::KeyReleased: {

            if(ev.key.code == sf::Keyboard::Escape) {
                state = SHOWMENU;
            }
            else if(ev.key.code == sf::Keyboard::T && sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt)) {

                if(IsWindowVisible(GetConsoleWindow())) {
                    ShowWindow(GetConsoleWindow(), SW_HIDE);
                } else {
                    ShowWindow(GetConsoleWindow(), SW_SHOW);
                }
            }
            else {
                board->handleEvent(ev);
            }

            break;
        }
        default: break;
    }
}

void mainMenu::updateMenu() {

    switch (state) {
        case SHOWMENU: {
            for(int i = 0; i < 3; ++i) {

                buttons[i].updateButton();
                if(buttons[i].wasPressed) {

                    switch (i) {
                        case 0: {
                            state = SINGLEPLAYER;

                            delete board;
                            board = new Board(false, false);

                            break;
                        }
                        case 1: {
                            state = MULTIPLAYER;

                            delete board;
                            board = new Board(false, true);

                            break;
                        }
                        case 2: {
                            state = EXIT;
                            break;
                        }
                    }

                    buttons[i].wasPressed = false;
                }
            }
            break;
        }
        case SINGLEPLAYER:
        case MULTIPLAYER: {
            board->updateBoard();
            break;
        }
        case EXIT: {
            window.close();
            break;
        }
        default: break;
    }
}

void mainMenu::drawMenu() {

    switch (state) {
        case SHOWMENU: {
            drawBackground();
            drawTitle();

            for(auto & button : buttons) {

                button.drawButton();
            }
            break;
        }
        case SINGLEPLAYER:
        case MULTIPLAYER: {
            board->drawBoard();
            break;
        }
        default: break;
    }
}

void mainMenu::drawTitle() const {

    sf::Sprite tmp;
    tmp.setTexture(title);
    tmp.setPosition(window.getSize().x/2 - title.getSize().x/2, 70);

    window.draw(tmp);
}

void mainMenu::drawBackground() const {

    sf::Sprite tmp;
    tmp.setTexture(background);
    tmp.setPosition(0,0);

    window.draw(tmp);
}