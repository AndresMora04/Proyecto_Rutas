#include "Map.h"
Map::Map(const string& filePath) {
    if (!mapTexture.loadFromFile(filePath)) {
        cout << "Error al cargar la imagen: " << filePath << endl;
    }
    mapSprite.setTexture(mapTexture);
}

void Map::draw(RenderWindow& window) {
    window.draw(mapSprite);
}