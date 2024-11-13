#include "Map.h"
#include "Menu.h"
#include "RouteManager.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

using namespace std;
using namespace sf;

int main() {
    RenderWindow window(VideoMode(700, 500), "Mapa y Menú");

    // Crear el objeto Mapa
    Map mapa("C:\\Users\\morit\\OneDrive\\Escritorio\\Progra 1.0\\Proyecto_Rutas\\RouteProject\\Fotos\\MapaGTA.jpg");

    // Crear el objeto Menu
    Menu menu(window, window.getSize().x, window.getSize().y);

    // Crear el objeto RouteManager para gestionar los puntos y rutas
    RouteManager routeManager;

    // Fuente para el texto
    Font font;
    if (!font.loadFromFile("C:\\Users\\morit\\OneDrive\\Escritorio\\Progra 1.0\\Proyecto_Rutas\\RouteProject\\Fotos\\OldLondon.ttf")) {
        cout << "Error al cargar la fuente\n";
        return -1;
    }

    // Texto para el nombre del punto
    Text inputText;
    inputText.setFont(font);
    inputText.setCharacterSize(30);
    inputText.setFillColor(Color::White);
    inputText.setPosition(10, 10);  // Posición de entrada en pantalla

    string pointName;
    bool isEnteringName = false;  // Variable para verificar si el usuario está ingresando un nombre
    bool isDeletingRoute = false; // Variable para indicar si se está eliminando una ruta
    bool isDeletingPoint = false; // Variable para indicar si se está eliminando un punto

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }

            // Detectar clics del mouse
            if (event.type == Event::MouseButtonPressed) {
                if (event.mouseButton.button == Mouse::Left) {
                    Vector2i mousePos = Mouse::getPosition(window);

                    // Manejar clic en el menú para seleccionar color o activar funciones de botones
                    menu.handleMouseClick(mousePos, routeManager);

                    // Verificar si se activaron las opciones de eliminar ruta o punto
                    if (menu.isDeleteRouteSelected()) {
                        isDeletingRoute = true;
                    }
                    else if (menu.isDeletePointSelected()) {
                        isDeletingPoint = true;
                    }

                    // Si la opción de eliminar ruta o punto está activa, llama a handleClickDelete
                    if (isDeletingRoute || isDeletingPoint) {
                        routeManager.handleClickDelete(mousePos, isDeletingRoute, isDeletingPoint);
                        isDeletingRoute = false;  // Resetear después del clic
                        isDeletingPoint = false;  // Resetear después del clic
                    }

                    // Si está seleccionada la zona de mapa y el color, iniciar entrada de nombre
                    if (mousePos.x < window.getSize().x - 200 && menu.getSelectedColor() != Color::White && menu.areColorsVisible()) {
                        isEnteringName = true;  // Activar entrada de nombre
                        pointName.clear();  // Limpiar nombre anterior
                    }
                }
            }

            // Capturar entrada de teclado para el nombre del punto o eliminar rutas/puntos
            if (event.type == Event::TextEntered) {
                if (isEnteringName) {
                    if (event.text.unicode == '\r') {  // Si presiona Enter
                        Vector2i mousePos = Mouse::getPosition(window);
                        routeManager.addPoint(mousePos.x, mousePos.y, menu.getSelectedColor(), pointName);
                        isEnteringName = false;  // Desactivar entrada de nombre
                    }
                    else if (event.text.unicode == '\b' && !pointName.empty()) {
                        pointName.pop_back();  // Borrar último carácter con Backspace
                    }
                    else if (event.text.unicode < 128) {
                        pointName += static_cast<char>(event.text.unicode);  // Agregar caracteres al nombre
                    }
                }
            }

            // Comprobar si el mouse está sobre un punto para mostrar el nombre
            if (event.type == Event::MouseMoved) {
                routeManager.checkMouseHover(Mouse::getPosition(window), window);
            }
        }

        window.clear();

        // Dibujar los elementos en la ventana
        mapa.draw(window);              // Dibujar el mapa
        routeManager.draw(window);      // Dibujar los puntos
        routeManager.checkMouseHover(Mouse::getPosition(window), window);  // Verificar el hover

        // Dibujar el texto de entrada de nombre
        if (isEnteringName) {
            inputText.setString("Nombre del punto: " + pointName);
            window.draw(inputText);
        }

        menu.draw(window);  // Dibujar el menú
        window.display();
    }

    return 0;
}