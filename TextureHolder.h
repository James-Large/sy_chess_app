#ifndef TEXTURES_H_INCLUDED
#define TEXTURES_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <utility>
#include "PieceEnum.h"


class TextureHolder {
public:
    typedef std::map<Enums::PieceID, sf::Texture*> textureMap;

public: //CHANGE BACK TO PRIVATE ONCE TESTING FOR SIZE() IS COMPELTE
    textureMap textures;

public:
    void load(Enums::PieceID, const std::string); //loads a texture from a file and adds it to the texture map
    sf::Texture& get(Enums::PieceID); //retrieves a texture with a given key
    const sf::Texture& get(Enums::PieceID) const; //const version of above

    ~TextureHolder() { //must delete the dynamically allocated textures upon destruction
        for (textureMap::iterator iter = textures.begin(); iter != textures.end(); ++iter) {
            delete iter->second;
        }
    }

};

#endif // TEXTURES_H_INCLUDED
