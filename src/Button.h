//
// Created by Bulcsú on 1/16/2021.
//

#ifndef CHESS_BUTTON_H
#define CHESS_BUTTON_H

#include "SFML/Graphics.hpp"
#include <cstring>

enum buttonState {NEUTRAL = 0, HOVER, CLICKED};

class Button {
public:
    buttonState state;
    sf::Vector2f pos;
    sf::Texture texture[3];
    sf::Text text;
    sf::Font font;

    float width, height;
    bool wasPressed;

    Button();
    void setButton(sf::Vector2f pos, float width, float height, std::string text, std::string font, std::string textureSource);
    ~Button();

    bool isMouseOnButton();
    void drawButton();
    void updateButton();
    void handleEvent(sf::Event&);
};

#endif //CHESS_BUTTON_H
