//
// Created by Bulcsú on 1/16/2021.
//

#include "Button.h"
#include <iostream>

extern sf::RenderWindow window;

Button::Button() {

    state = NEUTRAL;

    width = 200;
    height = 100;
    pos = sf::Vector2f(0,0);
    wasPressed = false;

    text.setString("");
    text.setPosition(0,0);
}

Button::~Button() {}

void Button::setButton(sf::Vector2f pos, float width, float height, std::string text, std::string font, std::string textureSource) {

    state = NEUTRAL;
    this->pos = pos;
    this->width = width;
    this->height = height;
    this->text.setString(text);

    if(!this->font.loadFromFile(font)) {

        printf("Font loading failed!");
        exit(1);
    }
    if(!this->texture[0].loadFromFile(textureSource + "/neutral.png") ||
       !this->texture[1].loadFromFile(textureSource + "/hover.png")   ||
       !this->texture[2].loadFromFile(textureSource + "/clicked.png")   ) {

        printf("Button loading failed!");
        exit(1);
    }
    this->texture->setSmooth(true);

    this->text.setFont(this->font);
    this->text.setCharacterSize(30);
    this->text.setFillColor(sf::Color::Black);
    this->text.setPosition(pos.x + width/2 - this->text.getLocalBounds().width/2, pos.y + height/2 - this->text.getCharacterSize()/2);
}

void Button::updateButton() {

    if(!isMouseOnButton()) {

        state = NEUTRAL;
    }
    else if(!sf::Mouse::isButtonPressed(sf::Mouse::Left) && state != CLICKED) {

        state = HOVER;
    }
}

void Button::handleEvent(sf::Event &ev) {

    switch (ev.type) {
        case sf::Event::MouseButtonPressed: {

            if(ev.key.code == sf::Mouse::Button::Left && state == HOVER) {

                state = CLICKED;
            }
            break;
        }
        case sf::Event::MouseButtonReleased: {

            if(ev.key.code == sf::Mouse::Button::Left && state == CLICKED) {

                wasPressed = true;
            }
            break;
        }
        default: break;
    }
}

void Button::drawButton() {

    sf::Sprite tmp;
    sf::Vector2f scale;

    switch (state) {
        case NEUTRAL: {
            tmp.setTexture(texture[0]);
            sf::Vector2u size;
            size = texture[0].getSize();
            scale.x = width / (float)size.x;
            scale.y = height / (float)size.y;

            break;
        }
        case HOVER: {
            tmp.setTexture(texture[1]);
            sf::Vector2u size;
            size = texture[1].getSize();
            scale.x = width / (float)size.x;
            scale.y = height / (float)size.y;

            break;
        }
        case CLICKED: {
            tmp.setTexture(texture[2]);
            sf::Vector2u size;
            size = texture[2].getSize();
            scale.x = width / (float)size.x;
            scale.y = height / (float)size.y;

            break;
        }
        default:
            printf("Button drawing error!");
            break;

    }

    tmp.setPosition(pos);
    tmp.setScale(scale);

    window.draw(tmp);
    window.draw(text);
}

bool Button::isMouseOnButton() {

    int mouseX = sf::Mouse::getPosition(window).x;
    int mouseY = sf::Mouse::getPosition(window).y;

    return mouseX >= pos.x && mouseY >= pos.y && mouseX <= pos.x + width && mouseY <= pos.y + height;
}