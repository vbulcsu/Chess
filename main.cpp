#pragma clang diagnostic push
#pragma clang diagnostic pop
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <iostream>
#include "SFML/Graphics.hpp"
#include "src/Menu.h"

#define FRAME_RATE 60

sf::RenderWindow window;
Board *board = nullptr;
mainMenu *menu = nullptr;

std::string message;

[[noreturn]] DWORD WINAPI console([[maybe_unused]] LPVOID lpParam ) {

    char command[50];

    while (true) {
        printf(">");
        std::cin.getline(command, 50);

        if(strcmp(command, "data") == 0 && board != nullptr) {

            for(int i = 0; i < 8; ++i) {

                for(int j = 0;  j < 8; ++j) {

                    std::cout << board->spaces[i][j] << " ";
                }
                std::cout << std::endl;
            }
        }
        else if(strcmp(command, "position") == 0 && board != nullptr) {

            std::cout << board->moves.getPosition() << std::endl;
        }
        else if(strcmp(command, "log") == 0) {

            std::cout << message;
        }
        else if(strcmp(command, "clear") == 0) {

            message = "";
            std::cout << "Cleared!\n";
        }
        else if(strcmp(command, "engine") == 0) {

            std::cout << getNextMove(board->moves.getPosition()) << std::endl;
        }
        else {

            std::cout << "Unknown command!" << std::endl;
        }
    }
}

int main() {

    ShowWindow(GetConsoleWindow(), SW_HIDE);

    window.create(sf::VideoMode(800,800),"Chess", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(FRAME_RATE);
    bool pause = false;

    menu = new mainMenu();

    HANDLE hThread = CreateThread(nullptr,0,console,nullptr,0,nullptr);

    while(window.isOpen()) {

        menu->updateMenu();

        sf::Event ev;

        while(window.pollEvent(ev)) {

            switch (ev.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::LostFocus:
                    pause = true;
                    break;
                case sf::Event::GainedFocus:
                    pause = false;
                    break;
                default:
                    if(!pause) menu->handleEvent(ev);
                    break;
            }
        }

        if(!pause) {
            window.clear(sf::Color::White);
            menu->drawMenu();
            window.display();
        }
    }

    delete menu;
    SuspendThread(hThread);

    return 0;
}