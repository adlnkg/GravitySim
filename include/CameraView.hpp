#pragma once
#include <SFML/Graphics.hpp>

class CameraView
{
private:
    float zoomFactor = 1.0f;

    bool toggleFocus = false;

    sf::Vector2f size;
    sf::Vector2f centerPos;

    sf::View v;

public:
    CameraView(sf::Vector2f size, sf::Vector2f centerPos = {0, 0});

    void zoom(float factor);

    void zoomIn();  // Zoom avant
    void zoomOut(); // Zoom arrière

    void zoomAtPointer(sf::RenderWindow &window, float factor);

    void focusOn(const sf::Vector2f &center);

    sf::Vector2f getCenter() const;

    void applyTo(sf::RenderWindow &window);

    sf::Vector2f getMouseWorldPos(const sf::RenderWindow &window);
};