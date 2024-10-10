#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;
using namespace std;
class Menu {
public:
    Menu(float width, float height);
    void draw(RenderWindow& window);

private:
    RectangleShape menuShape;
};