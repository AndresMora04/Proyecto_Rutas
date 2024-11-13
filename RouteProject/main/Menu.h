#pragma once
#include <SFML/Graphics.hpp>
#include "RouteManager.h"
#include <iostream>
using namespace sf;
using namespace std;

class Menu {
public:
    Menu(RenderWindow& win, float width, float height); // Constructor con la ventana
    void draw(RenderWindow& window);
    Color getSelectedColor() const;
    void handleMouseClick(Vector2i mousePos, RouteManager& routeManager);
    void toggleColorPalette();
    bool areColorsVisible() const;
    bool isDeleteRouteSelected() const;
    bool isDeletePointSelected() const;
    bool isLoadEnabled = true;  // Variable para habilitar o deshabilitar el botón "Cargar"
    std::string routeName;
    void toggleLoadFunction(RouteManager& routeManager);
    std::string getRouteNameInput();
private:
    RectangleShape menuShape;
    RectangleShape colorPalette[6];
    RectangleShape buttons[7];  // Cambiado a 7 para los nuevos botones
    Text buttonLabels[7];       // Etiquetas de texto para los botones
    Font font;                  // Fuente para el texto de los botones
    Color selectedColor;
    bool colorsVisible;

    void createColorPalette();
    void createButtons();
    void handleButtonClick(Vector2i mousePos, RouteManager& routeManager);

    void promptDeleteRoute(RouteManager& routeManager);   // Nuevo método para eliminar ruta
    void promptDeletePoint(RouteManager& routeManager);   // Nuevo método para eliminar punto

    bool deleteRouteSelected = false;
    bool deletePointSelected = false;
    sf::Text inputText;
    sf::Text userInputText;

    RenderWindow& window; // Referencia a la ventana
};