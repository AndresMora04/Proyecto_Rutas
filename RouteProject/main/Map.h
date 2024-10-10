#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;
using namespace std;
class Map {
public:
    Map(const string& filePath);
    void draw(RenderWindow& window);

private:
    Texture mapTexture;
    Sprite mapSprite;
};