#ifndef COORDS_H_INCLUDED
#define COORDS_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <iostream>

class Coords {
public:
    int x;
    int y;

    Coords(): x(0), y(0) {} //default initialiser

    Coords(int a, int b): x(a), y(b) { } //constructor from two ints

    Coords(const sf::Vector2i &mousePosition) { //constructor from a mouse position, converts to logical board coords
        x = (mousePosition.x / 50) + 1;
        y = 8 - (mousePosition.y / 50);
    }

    sf::Vector2f findScreenPos() const { //converts the logical coordinates of the abstract board to onscreen pixel coordinates
        sf::Vector2f scrnPos((x-1)*50, 400-(y*50)); //logical board works with origin of bottom left corner, screen of top left corner
        return scrnPos;
    }

    bool empty() { //returns true if this is a value-initialised Coord
        return x == 0 && y == 0;
    }

    bool operator<(const Coords &rhs) const { //implemented only so that I can use coords as the key in the board map
        if (x < rhs.x) {
            return true;
        }
        else if (rhs.x < x) {
            return false;
        }
        else { //x == rhs.x
            if (y < rhs.y) {
                return true;
            }
            else {
                return false;
            }
        }
    }

    bool operator==(const Coords &rhs) const {
        if (x == rhs.x && y == rhs.y) {
            return true;
        }
        else {
            return false;
        }
    }

    bool operator!=(const Coords &rhs) const {
        if (*this == rhs) {
            return false;
        }
        else {
            return true;
        }
    }
};

inline std::ostream& operator<<(std::ostream &stream, const Coords &coords) {
    //eventually do this properly, as well as input operator
    stream << coords.x << "," << coords.y;
    return stream;
}

inline std::istream& operator>>(std::istream &stream, Coords &coords) {
    //do error checking
    if (!stream) {
        return stream;
    }

    std::string input;
    stream >> input;

    coords.x = input[0]-48; //ascii code for 0 = 48
    coords.y = input[2]-48; //crudely converts char to int

    return stream;
}

#endif // COORDS_H_INCLUDED
