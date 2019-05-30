#include "TextureHolder.h"
#include <map>
#include <utility>
#include <string>
#include <stdexcept>

void TextureHolder::load(Enums::PieceID id, const std::string fileName) {
    sf::Texture* texture = new sf::Texture(); //dynamically allocated so that object persists beyond this scope
    if (!texture->loadFromFile(fileName)) { //setting texture
        throw std::runtime_error("error loading texture file");
    }
    textures.insert(std::make_pair(id, texture));
}

sf::Texture& TextureHolder::get(Enums::PieceID id) {
    textureMap::iterator texture = textures.find(id);
    return *(texture->second); //texture points to pair<PieceID, Texture*>, so get the second member and dereference that
                                //pointer to get the actual texture
}

const sf::Texture& TextureHolder::get(Enums::PieceID id) const {
    textureMap::const_iterator texture = textures.find(id);
    return *(texture->second); //texture points to pair<PieceID, Texture*>, so get the second member and dereference that
                                //pointer to get the actual texture
}
