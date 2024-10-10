#include "Menu.h"
#include <iostream>
using namespace sf;
using namespace std;
Menu::Menu(float width, float height) {
    menuShape.setSize(Vector2f(200, height));  // Ancho del men�
    menuShape.setFillColor(Color(100, 100, 100));  // Color gris
    menuShape.setPosition(width - 200, 0);  // Posici�n del men� (derecha)
}

void Menu::draw(RenderWindow& window) {
    window.draw(menuShape);
}