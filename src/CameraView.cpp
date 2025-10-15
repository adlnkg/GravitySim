#include "CameraView.hpp"

CameraView::CameraView(sf::Vector2f size, sf::Vector2f centerPos)
{
    this->size = size;
    this->centerPos = centerPos;

    v.setCenter(centerPos);
    v.setSize(size);
}

void CameraView::zoom(float factor)
{
    zoomFactor *= factor;
    v.zoom(factor);
}

void CameraView::zoomIn() { zoom(0.9f); }  // Zoom avant
void CameraView::zoomOut() { zoom(1.1f); } // Zoom arrière

void CameraView::zoomAtPointer(sf::RenderWindow &window, float factor)
{
    sf::Vector2i mousePixel = sf::Mouse::getPosition(window);

    sf::Vector2f beforeZoom = window.mapPixelToCoords(mousePixel, v);

    zoomFactor *= factor;
    v.zoom(factor);

    sf::Vector2f afterZoom = window.mapPixelToCoords(mousePixel, v);

    v.move(beforeZoom - afterZoom);
}

void CameraView::focusOn(const sf::Vector2f &center)
{
    v.setCenter(center);
}

sf::Vector2f CameraView::getCenter() const
{
    return v.getCenter();
}

void CameraView::applyTo(sf::RenderWindow &window)
{
    window.setView(v);
}

sf::Vector2f CameraView::getMouseWorldPos(const sf::RenderWindow &window)
{
    sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
    return window.mapPixelToCoords(mousePixel, v);
}