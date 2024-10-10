#include "Map.h"
#include "Menu.h"
#include <iostream>
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;
int main() {
    // Crear la ventana
    sf::RenderWindow window(sf::VideoMode(700, 500), "Mapa y Menú");

    // Crear el objeto Mapa
    Map mapa("C:\\Users\\morit\\OneDrive\\Escritorio\\Progra 1.0\\Proyecto_Rutas\\RouteProject\\Fotos\\MapaGTA.jpg");

    // Crear el objeto Menu
    Menu menu(window.getSize().x, window.getSize().y);

    // Bucle principal de la aplicación
    while (window.isOpen()) {
        // Manejo de eventos
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }

        // Limpiar la ventana
        window.clear();

        // Dibujar el mapa
        mapa.draw(window);

        // Dibujar el menú
        menu.draw(window);

        // Mostrar la ventana
        window.display();
    }
    return 0;
}