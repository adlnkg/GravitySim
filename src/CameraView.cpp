#include <SFML/Graphics.hpp>

class CameraView
{

    float zoomFactor;

    bool toggleFocus = false;

    sf::Vector2f size;
    sf::Vector2f centerPos;

    sf::View v;

public:
    CameraView(sf::Vector2f size, sf::Vector2f centerPos = {0, 0}) : v(centerPos, size) {}

    void zoom(float factor)
    {
        zoomFactor *= factor;
        v.zoom(factor);
    }

    void zoomIn() { zoom(0.9f); }  // Zoom avant
    void zoomOut() { zoom(1.1f); } // Zoom arrière

    void zoomAtPointer(sf::RenderWindow &window, float factor)
    {
        sf::Vector2i mousePixel = sf::Mouse::getPosition(window);

        sf::Vector2f beforeZoom = window.mapPixelToCoords(mousePixel, v);

        zoomFactor *= factor;
        v.zoom(factor);

        sf::Vector2f afterZoom = window.mapPixelToCoords(mousePixel, v);

        v.move(beforeZoom - afterZoom);
    }

    void focusOn(const sf::Vector2f &center)
    {
        v.setCenter(center);
    }

    sf::Vector2f getCenter()
    {
        return v.getCenter();
    }

    void applyTo(sf::RenderWindow &window)
    {
        window.setView(v);
    }
};