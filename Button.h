#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <string>

class Button {
private:
    bool active;
    bool wasClicked;
    sf::Text buttonText;
    sf::Sprite buttonSprite;

public:
    Button(): active(false), wasClicked(false), buttonText(sf::Text()), buttonSprite(sf::Sprite()) { }

    void build(const sf::Font &font, const sf::Texture &texture, const sf::Vector2f &position, const std::string &text, const unsigned &fontSize) {
        active = false;
        wasClicked = false;

        buttonText.setFont(font);
        buttonText.setString(text);
        buttonText.setCharacterSize(fontSize);
        buttonText.setColor(sf::Color::Black);
        buttonText.setStyle(sf::Text::Bold);

        buttonSprite.setTexture(texture);
        buttonSprite.setPosition(position);

        centreText();
    }

    bool clicked(const sf::Vector2f &pos) {
        if (buttonSprite.getGlobalBounds().contains(pos)) {
            wasClicked = true;
            return true;
        }
        return false;
    }
    bool clicked(const sf::Vector2i &posi) {
        sf::Vector2f posf = sf::Vector2f(posi.x, posi.y);
        return clicked(posf);
    }
    bool hasBeenClicked() {
        return wasClicked;
    }
    void reset() {
        wasClicked = false;
    }
//    bool isActive() {
//        return active;
//    }
//    void activate() {
//        active = true;
//    }
//    void deactivate() {
//        active = false;
//    }
    void setPosition(sf::Vector2f &newPos) {
        buttonSprite.setPosition(newPos);
        centreText();
    }

    void render(sf::RenderWindow &window) {
        window.draw(buttonSprite);
        window.draw(buttonText);
    }

private:
    void centreText() {
        float xOffset = (buttonSprite.getGlobalBounds().width - buttonText.getGlobalBounds().width) / 2;
        float yOffset = (buttonSprite.getGlobalBounds().height - buttonText.getGlobalBounds().height) / 2;

        buttonText.setPosition(buttonSprite.getPosition().x + xOffset, buttonSprite.getPosition().y + yOffset);
    }

};

#endif // BUTTON_H_INCLUDED
