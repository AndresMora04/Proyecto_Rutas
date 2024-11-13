#include "RouteManager.h"
#include <fstream>
#include <iostream>

void RouteManager::addPoint(float x, float y, Color color, const string& name) {
    if (routes.empty()) {
        routes.emplace_back();  // Crear una nueva ruta si no existen rutas
    }
    routes.back().emplace_back(x, y, color, name);  // Agregar el punto a la última ruta
}

void RouteManager::draw(sf::RenderWindow& window) {
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
            sf::VertexArray curve(sf::LinesStrip);

            // Calculamos una curva Bézier cuadrática entre tres puntos: el punto anterior, el punto actual y el siguiente
            sf::Vector2f P0 = it->shape.getPosition();
            sf::Vector2f P1 = it->shape.getPosition() + sf::Vector2f(50.0f, 50.0f);  // Aumentamos el desplazamiento para hacer la curva más notoria
            sf::Vector2f P2 = nextIt->shape.getPosition();

            // Usamos t para interpolar entre los puntos (desde t = 0 hasta t = 1)
            for (float t = 0; t <= 1; t += 0.02f) {  // Reducimos el paso para más puntos intermedios y suavizar la curva
                float x = (1 - t) * (1 - t) * P0.x + 2 * (1 - t) * t * P1.x + t * t * P2.x;
                float y = (1 - t) * (1 - t) * P0.y + 2 * (1 - t) * t * P1.y + t * t * P2.y;
                curve.append(sf::Vertex(sf::Vector2f(x, y), sf::Color::White));
            }

            // Dibuja la curva en la ventana
            window.draw(curve);
        }
    }

    // Dibujar los nombres de las rutas solo si se han cargado
    if (!routeNames.empty()) {
        sf::Font font;
        if (!font.loadFromFile("C:\\Users\\morit\\OneDrive\\Escritorio\\Progra 1.0\\Proyecto_Rutas\\RouteProject\\Fotos\\OldLondon.ttf")) {
            std::cerr << "No se pudo cargar la fuente.\n";
            return;
        }

        float yPosition = 10.0f;  // Posición vertical inicial para mostrar los nombres

        for (const auto& name : routeNames) {
            sf::Text routeNameText;
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

    std::ofstream file(filename, std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo para guardar las rutas.\n";
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
        cerr << "Error al abrir el archivo para cargar las rutas.\n";
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
                name = line.substr(0, pos);
                line = line.substr(pos);
            }

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

            routes.back().emplace_back(x, y, Color(r, g, b), name);
        }
    }
    file.close();
}

void RouteManager::deleteRoute(int index) {
    if (index >= 0 && index < routes.size()) {
        auto it = routes.begin();
        advance(it, index);
        routes.erase(it);
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
        // Si la opción "Eliminar Ruta" está activada
        for (auto routeIt = routes.begin(); routeIt != routes.end(); ) {
            bool routeDeleted = false;

            // Iterar sobre cada punto de la ruta para ver si se hizo clic en uno
            for (const auto& point : *routeIt) {
                if (point.shape.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                    // Si se ha hecho clic en un punto de la ruta, elimina la ruta completa
                    routeIt = routes.erase(routeIt);  // Elimina la ruta completa
                    routeDeleted = true;
                    break;  // Salir del bucle si se eliminó la ruta
                }
            }

            if (!routeDeleted) {
                ++routeIt;  // Continuar a la siguiente ruta si no se eliminó ninguna
            }
        }

        // Después de eliminar la ruta, guarda las rutas actualizadas en el archivo
        updateRoutesInFile("C:\\Users\\morit\\OneDrive\\Escritorio\\Progra 1.0\\Proyecto_Rutas\\RouteProject\\Fotos\\rutas.txt");
    }
    else if (deletePoint) {
        // Si la opción "Eliminar Punto" está activada
        for (auto routeIt = routes.begin(); routeIt != routes.end(); ++routeIt) {
            for (auto pointIt = routeIt->begin(); pointIt != routeIt->end(); ++pointIt) {
                if (pointIt->shape.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                    // Si se hizo clic en el punto, elimina solo el punto
                    routeIt->erase(pointIt);
                    break;  // Salir del bucle después de eliminar el punto
                }
            }
        }

        // Después de eliminar el punto, guarda las rutas actualizadas en el archivo
        updateRoutesInFile("C:\\Users\\morit\\OneDrive\\Escritorio\\Progra 1.0\\Proyecto_Rutas\\RouteProject\\Fotos\\rutas.txt");
    }
}


void RouteManager::updateRoutesInFile(const string& filename) {
    ofstream file(filename, ios::trunc);  // Abre en modo trunc para sobrescribir el archivo
    if (!file.is_open()) {
        cerr << "Error al abrir el archivo para actualizar las rutas.\n";
        return;
    }

    // Escribe las rutas actuales en el archivo
    for (const auto& route : routes) {
        file << "Route\n";
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