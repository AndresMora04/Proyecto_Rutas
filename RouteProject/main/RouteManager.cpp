#include "RouteManager.h"
#include <fstream>
#include <iostream>
void RouteManager::addPoint(float x, float y, Color color, const string& name) {
    if (routes.empty()) {
        routes.emplace_back();  // Crear una nueva ruta si no existen rutas
    }
    routes.back().emplace_back(x, y, color, name);  // Agregar el punto a la última ruta
}

void RouteManager::draw(RenderWindow& window) {
    // Dibuja las rutas
    for (auto& route : routes) {
        // Dibujar puntos de cada ruta
        for (auto& point : route) {
            window.draw(point.shape);
        }

        // Dibujar curvas suaves entre puntos de una misma ruta
        auto it = route.begin();
        for (auto nextIt = ++route.begin(); nextIt != route.end(); ++it, ++nextIt) {
            // Aquí usamos las posiciones de los puntos para dibujar la curva
            VertexArray curve(sf::LinesStrip);

            // Definimos los puntos de control para la curva cúbica
            Vector2f P0 = it->shape.getPosition();  // Punto anterior
            Vector2f P1 = it->shape.getPosition() + Vector2f(50.0f, 50.0f);  // Primer punto de control
            Vector2f P2 = nextIt->shape.getPosition() + Vector2f(-50.0f, -50.0f);  // Segundo punto de control
            Vector2f P3 = nextIt->shape.getPosition();  // Punto siguiente

            // Usamos t para interpolar entre los puntos (desde t = 0 hasta t = 1)
            for (float t = 0; t <= 1; t += 0.02f) {  // Reducimos el paso para más puntos intermedios y suavizar la curva
                float x = (1 - t) * (1 - t) * (1 - t) * P0.x
                    + 3 * (1 - t) * (1 - t) * t * P1.x
                    + 3 * (1 - t) * t * t * P2.x
                    + t * t * t * P3.x;
                float y = (1 - t) * (1 - t) * (1 - t) * P0.y
                    + 3 * (1 - t) * (1 - t) * t * P1.y
                    + 3 * (1 - t) * t * t * P2.y
                    + t * t * t * P3.y;
                curve.append(sf::Vertex(Vector2f(x, y), Color::White));
            }

            // Dibuja la curva en la ventana
            window.draw(curve);
        }
    }

    // Dibujar los nombres de las rutas solo si se han cargado
    if (!routeNames.empty()) {
        Font font;
        if (!font.loadFromFile("C:\\Users\\morit\\OneDrive\\Escritorio\\Progra 1.0\\Proyecto_Rutas\\RouteProject\\Fotos\\OldLondon.ttf")) {
            cout << "No se pudo cargar la fuente.\n";
            return;
        }

        float yPosition = 10.0f;  // Posición vertical inicial para mostrar los nombres

        for (const auto& name : routeNames) {
            Text routeNameText;
            routeNameText.setFont(font);
            routeNameText.setString(name);
            routeNameText.setCharacterSize(20);
            routeNameText.setFillColor(sf::Color::White);
            routeNameText.setPosition(10.0f, yPosition);  // Dibujar en el margen izquierdo

            window.draw(routeNameText);

            yPosition += 25.0f;  // Espacio entre cada nombre de ruta
        }
    }
}

void RouteManager::checkMouseHover(Vector2i mousePos, RenderWindow& window) {
    for (const auto& route : routes) {
        for (const auto& point : route) {
            if (point.shape.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                Text pointNameText;
                Font font;
                if (font.loadFromFile("C:\\Users\\morit\\OneDrive\\Escritorio\\Progra 1.0\\Proyecto_Rutas\\RouteProject\\Fotos\\OldLondon.ttf")) {
                    pointNameText.setFont(font);
                    pointNameText.setString(point.name);
                    pointNameText.setCharacterSize(20);
                    pointNameText.setFillColor(Color::White);
                    pointNameText.setPosition(point.shape.getPosition().x + 10, point.shape.getPosition().y - 20);
                    window.draw(pointNameText);
                }
            }
        }
    }
}

void RouteManager::saveRoutesToFile(const std::string& filename, const std::string& routeName) {
    addRouteName(routeName);  // Almacena el nombre de la ruta

    std::ofstream file(filename, ios::app);
    if (!file.is_open()) {
        cout << "Error al abrir el archivo para guardar las rutas.\n";
        return;
    }

    file << "Route " << routeName << "\n";
    for (const auto& point : routes.back()) {
        file << point.name << " "
            << point.shape.getPosition().x << " "
            << point.shape.getPosition().y << " "
            << static_cast<int>(point.shape.getFillColor().r) << " "
            << static_cast<int>(point.shape.getFillColor().g) << " "
            << static_cast<int>(point.shape.getFillColor().b) << "\n";
    }
    file.close();

    // Borra los nombres de las rutas para que no aparezcan después de guardar
    routeNames.clear();
}

void RouteManager::loadRoutesFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error al abrir el archivo para cargar las rutas.\n";
        return;
    }

    routes.clear();       // Limpiar las rutas antes de cargar nuevas
    routeNames.clear();    // Limpiar los nombres de ruta
    string line;
    string currentRouteName;

    while (getline(file, line)) {
        if (line.empty() || line[0] == '#') {
            continue;  // Ignorar líneas vacías o comentarios
        }

        // Si la línea no tiene números, es el nombre de la ruta
        if (line.find_first_of("0123456789") == string::npos) {
            currentRouteName = line;  // Nombre de la ruta
            routeNames.push_back(currentRouteName); // Añadir nombre de la ruta
            routes.emplace_back();    // Iniciar una nueva ruta
        }
        else {
            // Procesar el punto
            string name;
            float x, y;
            int r, g, b;

            size_t pos = line.find_first_of("0123456789");
            if (pos != string::npos) {
                name = line.substr(0, pos); // Nombre del punto
                line = line.substr(pos);    // El resto es el valor de las coordenadas y color
            }

            // Extraer coordenadas y colores
            pos = line.find(' ');
            x = stof(line.substr(0, pos));
            line = line.substr(pos + 1);

            pos = line.find(' ');
            y = stof(line.substr(0, pos));
            line = line.substr(pos + 1);

            pos = line.find(' ');
            r = stoi(line.substr(0, pos));
            line = line.substr(pos + 1);

            pos = line.find(' ');
            g = stoi(line.substr(0, pos));
            line = line.substr(pos + 1);

            b = stoi(line);

            // Verificar que las coordenadas estén dentro del rango de la ventana
            if (x >= 0 && x <= 800 && y >= 0 && y <= 600) {  // 800x600 es un ejemplo de resolución
                routes.back().emplace_back(x, y, Color(r, g, b), name);
            }
        }
    }
    file.close();
}

void RouteManager::deleteRoute(int index) {
    if (index >= 0 && index < routes.size()) {
        auto routeIt = routes.begin();
        auto nameIt = routeNames.begin();

        // Avanzamos a la ruta y su nombre correspondiente
        advance(routeIt, index);
        advance(nameIt, index);

        // Elimina la ruta y el nombre correspondiente
        routes.erase(routeIt);
        routeNames.erase(nameIt);
    }
}

void RouteManager::deletePoint(int routeIndex, const string& pointName) {
    if (routeIndex >= 0 && routeIndex < routes.size()) {
        auto routeIt = routes.begin();
        advance(routeIt, routeIndex);
        auto& route = *routeIt;
        for (auto it = route.begin(); it != route.end(); ++it) {
            if (it->name == pointName) {
                route.erase(it);
                break;
            }
        }
    }
}

void RouteManager::handleClickDelete(Vector2i mousePos, bool deleteRoute, bool deletePoint) {
    if (deleteRoute) {
        // Itera sobre las rutas y los nombres de las rutas
        auto routeIt = routes.begin();
        auto nameIt = routeNames.begin();

        while (routeIt != routes.end() && nameIt != routeNames.end()) {
            bool routeDeleted = false;

            // Itera sobre cada punto de la ruta para verificar si se hizo clic en uno de ellos
            for (auto pointIt = routeIt->begin(); pointIt != routeIt->end(); ++pointIt) {
                if (pointIt->shape.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                    // Si se hizo clic en el punto, elimina la ruta y su nombre
                    routeIt = routes.erase(routeIt);  // Elimina la ruta completa
                    nameIt = routeNames.erase(nameIt);  // Elimina el nombre de la ruta
                    routeDeleted = true;
                    break;  // Sale del bucle una vez que la ruta ha sido eliminada
                }
            }

            if (!routeDeleted) {
                ++routeIt;  // Si no se eliminó ninguna ruta, pasa a la siguiente
                ++nameIt;   // Avanza al siguiente nombre
            }
        }

        // Guarda las rutas actualizadas después de eliminar
        updateRoutesInFile("C:\\Users\\morit\\OneDrive\\Escritorio\\Progra 1.0\\Proyecto_Rutas\\RouteProject\\Fotos\\rutas.txt");
    }
    else if (deletePoint) {
        // Eliminar punto: iterar sobre rutas y eliminar punto seleccionado
        for (auto routeIt = routes.begin(); routeIt != routes.end(); ++routeIt) {
            for (auto pointIt = routeIt->begin(); pointIt != routeIt->end(); ++pointIt) {
                if (pointIt->shape.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                    // Si se hace clic en el punto, eliminarlo de la ruta
                    routeIt->erase(pointIt);
                    break;  // Salir del bucle después de eliminar el punto
                }
            }
        }

        // Guarda las rutas actualizadas después de eliminar el punto
        updateRoutesInFile("C:\\Users\\morit\\OneDrive\\Escritorio\\Progra 1.0\\Proyecto_Rutas\\RouteProject\\Fotos\\rutas.txt");
    }
}

void RouteManager::updateRoutesInFile(const string& filename) {
    ofstream file(filename, ios::trunc);  // Abre en modo trunc para sobrescribir el archivo
    if (!file.is_open()) {
        cout << "Error al abrir el archivo para actualizar las rutas.\n";
        return;
    }

    // Escribe las rutas actuales en el archivo
    auto routeNameIt = routeNames.begin();  // Iterador para los nombres de rutas
    for (const auto& route : routes) {
        if (routeNameIt != routeNames.end()) {
            // Escribe el nombre de la ruta antes de los puntos
            file << *routeNameIt << "\n";
            ++routeNameIt;  // Avanza al siguiente nombre de ruta
        }

        // Escribe los puntos de la ruta
        for (const auto& point : route) {
            file << point.name << " "
                << point.shape.getPosition().x << " "
                << point.shape.getPosition().y << " "
                << static_cast<int>(point.shape.getFillColor().r) << " "
                << static_cast<int>(point.shape.getFillColor().g) << " "
                << static_cast<int>(point.shape.getFillColor().b) << "\n";
        }
    }
}

void RouteManager::clearRoutes() {
    routes.clear();
}

void RouteManager::clearRouteNames() {
    routeNames.clear();
}