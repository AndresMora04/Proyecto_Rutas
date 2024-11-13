#pragma once
#include <SFML/Graphics.hpp>
#include <list>
#include <string>
using namespace sf;
using namespace std;
struct Point {
    CircleShape shape;
    string name;

    Point(float x, float y, Color color, const string& pointName) : name(pointName) {
        shape.setRadius(5.0f);
        shape.setFillColor(color);
        shape.setPosition(x, y);
    }
};

class RouteManager {
public:
    void addPoint(float x, float y, Color color, const string& name);
    void draw(RenderWindow& window);
    void checkMouseHover(Vector2i mousePos, RenderWindow& window);
    void saveRoutesToFile(const string& filename, const std::string& routeName);
    void loadRoutesFromFile(const string& filename);
    void deleteRoute(int index);
    void deletePoint(int routeIndex, const string& pointName);
    void handleClickDelete(Vector2i mousePos, bool deleteRoute, bool deletePoint);
    void updateRoutesInFile(const string& filename);
    void clearRoutes();
    void clearRouteNames();
    void addRouteName(const std::string& routeName) {
        routeNames.push_back(routeName);
    }
private:
    list<list<Point>> routes;  // Lista de rutas, cada una siendo una lista de puntos
    list<string> routeNames; // Lista de nombres de rutas
};