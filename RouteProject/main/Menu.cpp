#include "Menu.h"
#include <iostream>
Menu::Menu(RenderWindow& win,float width, float height) : window(win), isLoadEnabled(true) { 
    menuShape.setSize(sf::Vector2f(200, height));
    menuShape.setFillColor(sf::Color(100, 100, 100));
    menuShape.setPosition(width - 200, 0);

    // Cargar la fuente
    if (!font.loadFromFile("C:\\Users\\morit\\OneDrive\\Escritorio\\Progra 1.0\\Proyecto_Rutas\\RouteProject\\Fotos\\OldLondon.ttf")) {
        cout << "Error al cargar la fuente\n";
    }

    // Inicializa el texto que se mostrará para la entrada de la ruta
    inputText.setFont(font);
    inputText.setCharacterSize(20);
    inputText.setFillColor(sf::Color::White);

    userInputText.setFont(font);
    userInputText.setCharacterSize(20);
    userInputText.setFillColor(sf::Color::White);

    createColorPalette();
    createButtons();

    selectedColor = sf::Color::White;
    colorsVisible = false;
}

void Menu::createColorPalette() {
    // Colores predefinidos
    Color colors[6] = { Color::Red, Color::Green, Color::Blue, Color::Yellow, Color::Cyan, Color::Magenta };

    float colorBoxSize = 40;
    float padding = 30;
    float startX = menuShape.getPosition().x + padding;
    float startY = 310;

    for (int i = 0; i < 6; ++i) {
        int row = i / 2;
        int col = i % 2;

        colorPalette[i].setSize(Vector2f(colorBoxSize, colorBoxSize));
        colorPalette[i].setFillColor(colors[i]);
        colorPalette[i].setPosition(startX + col * (colorBoxSize + padding), startY + row * (colorBoxSize + padding));
    }
}

void Menu::createButtons() {
    float buttonHeight = 30;
    float buttonWidth = 150;
    float padding = 20;
    float buttonXPosition = menuShape.getPosition().x + 25;

    std::string labels[7] = { "Colores", "Eliminar Ruta", "Eliminar Punto", "Guardar", "Cargar", "Salir" };

    for (int i = 0; i < 6; ++i) {
        buttons[i].setSize(Vector2f(buttonWidth, buttonHeight));
        buttons[i].setFillColor(Color::Blue);
        buttons[i].setOutlineThickness(2);
        buttons[i].setOutlineColor(Color::Black);
        buttons[i].setPosition(buttonXPosition, padding + i * (buttonHeight + padding));

        buttonLabels[i].setFont(font);
        buttonLabels[i].setString(labels[i]);
        buttonLabels[i].setCharacterSize(16);
        buttonLabels[i].setFillColor(Color::White);
        buttonLabels[i].setPosition(buttonXPosition + 10, padding + i * (buttonHeight + padding) + 5);
    }
}

void Menu::draw(RenderWindow& window) {
    window.draw(menuShape);

    if (colorsVisible) {
        for (int i = 0; i < 6; ++i) {
            window.draw(colorPalette[i]);
        }
    }

    for (int i = 0; i < 7; ++i) {
        // Solo dibujamos los botones de eliminar cuando isLoadEnabled es verdadero
        if (isLoadEnabled || i < 3 && i>0 || i == 4) { // 1,2,4 son los botones de eliminar ruta, punto y cargar
            window.draw(buttons[i]);
            window.draw(buttonLabels[i]);
        }
    }
}

Color Menu::getSelectedColor() const {
    return selectedColor;
}

void Menu::handleMouseClick(Vector2i mousePos, RouteManager& routeManager) {
    if (colorsVisible) {
        for (int i = 0; i < 6; ++i) {
            if (colorPalette[i].getGlobalBounds().contains(Vector2f(mousePos))) {
                selectedColor = colorPalette[i].getFillColor();
                break;
            }
        }
    }

    if (deletePointSelected) {
        routeManager.handleClickDelete(mousePos, false, true);
    }

    handleButtonClick(mousePos, routeManager);
}

void Menu::handleButtonClick(Vector2i mousePos, RouteManager& routeManager) {
    for (int i = 0; i < 7; ++i) {
        if (buttons[i].getGlobalBounds().contains(Vector2f(mousePos))) {
            switch (i) {
            case 0: // Colores
                toggleColorPalette();
                break;
            case 1: // Eliminar Ruta
                deleteRouteSelected = !deleteRouteSelected;
                deletePointSelected = false;
                buttons[1].setFillColor(deleteRouteSelected ? Color::Red : Color::White);
                buttons[2].setFillColor(Color::White);
                break;
            case 2: // Eliminar Punto
                deletePointSelected = !deletePointSelected;
                deleteRouteSelected = false;
                buttons[2].setFillColor(deletePointSelected ? Color::Red : Color::White);
                buttons[1].setFillColor(Color::White);
                break;
            case 3: // Guardar
                // Llamar al método getRouteNameInput para obtener el nombre de la ruta
                routeName = getRouteNameInput();

                // Guardar la ruta con el nombre ingresado
                routeManager.saveRoutesToFile("C:\\Users\\morit\\OneDrive\\Escritorio\\Progra 1.0\\Proyecto_Rutas\\RouteProject\\Fotos\\rutas.txt", routeName);
                routeManager.clearRoutes(); // Limpiar las rutas después de guardarlas
                break;
            case 4: // Cargar
                if (isLoadEnabled) {
                    routeManager.loadRoutesFromFile("C:\\Users\\morit\\OneDrive\\Escritorio\\Progra 1.0\\Proyecto_Rutas\\RouteProject\\Fotos\\rutas.txt");
                    isLoadEnabled = false; // Deshabilitar la carga tras un doble clic
                }
                else {
                    toggleLoadFunction(routeManager);
                }
                break;
            case 5: // Salir
                exit(0);
                break;
            default:
                break;
            }
            break;
        }
    }
}

void Menu::toggleColorPalette() {
    colorsVisible = !colorsVisible;
    selectedColor = Color::White;
}

void Menu::toggleLoadFunction(RouteManager& routeManager) {
    // Si isLoadEnabled es falso, entonces borra rutas y nombres
    if (!isLoadEnabled) {
        routeManager.clearRoutes();
        routeManager.clearRouteNames(); // Asegúrate de que también borra los nombres de la visualización
        isLoadEnabled = true; // Vuelve a habilitar la carga tras borrar
    }
}

bool Menu::areColorsVisible() const {
    return colorsVisible;
}

bool Menu::isDeleteRouteSelected() const {
    return deleteRouteSelected;
}

bool Menu::isDeletePointSelected() const {
    return deletePointSelected;
}

string Menu::getRouteNameInput() {
    std::string routeName;
    Text instructionText;
    Font font;

    // Cargar la fuente
    if (!font.loadFromFile("C:\\Users\\morit\\OneDrive\\Escritorio\\Progra 1.0\\Proyecto_Rutas\\RouteProject\\Fotos\\OldLondon.ttf")) {
        cout << "Error al cargar la fuente\n";
        return "";
    }

    instructionText.setFont(font);
    instructionText.setString("Ingrese el nombre de la ruta:");
    instructionText.setCharacterSize(20);
    instructionText.setFillColor(Color::White);
    instructionText.setPosition(50, 50); // Posición de la instrucción en la pantalla

    // Crear un cuadro de texto para mostrar el nombre de la ruta
    Text inputText;
    inputText.setFont(font);
    inputText.setCharacterSize(20);
    inputText.setFillColor(Color::White);
    inputText.setPosition(50, 100); // Posición del cuadro de texto

    // Manejar la entrada de texto mientras el usuario escribe
    bool isTyping = true;
    while (isTyping) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
                return routeName;
            }

            if (event.type == Event::TextEntered) {
                if (event.text.unicode == 8) { // Backspace
                    if (!routeName.empty()) {
                        routeName.pop_back();
                    }
                }
                else if (event.text.unicode == 13) { // Enter
                    isTyping = false; // Finalizar cuando presionen Enter
                }
                else if (event.text.unicode < 128) { // Solo caracteres válidos
                    routeName += static_cast<char>(event.text.unicode);
                }
            }
        }

        // Actualizar el texto mostrado en la pantalla
        inputText.setString(routeName);

        window.clear(); // Limpiar la ventana
        window.draw(instructionText); // Mostrar la instrucción
        window.draw(inputText); // Mostrar el nombre de la ruta ingresado
        window.display(); // Actualizar la pantalla
    }

    return routeName; // Devolver el nombre de la ruta ingresado
}