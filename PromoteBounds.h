#ifndef PROMOTEBOUNDS_H_INCLUDED
#define PROMOTEBOUNDS_H_INCLUDED

#include <SFML/Graphics.hpp>


class PromoteBounds {

    //promote screen origin: 100, 100
    //promote screen size: 200x200
    //central point of promote screen (local): 100, 120
    //central point of promote screen (global): 200, 220
    //size of each piece square: 80x65

    public:
        sf::IntRect overallBox;
        sf::IntRect bishopBox;
        sf::IntRect knightBox;
        sf::IntRect rookBox;
        sf::IntRect queenBox;

        PromoteBounds() {
            overallBox = sf::IntRect(120, 155, 160, 130);
            bishopBox = sf::IntRect(120, 155, 80, 65);
            knightBox = sf::IntRect(200, 155, 80, 65);
            rookBox = sf::IntRect(120, 220, 80, 65);
            queenBox = sf::IntRect(200, 220, 80, 65);
        }
};


#endif // PROMOTEBOUNDS_H_INCLUDED
