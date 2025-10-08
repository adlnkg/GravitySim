#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Object
{
private:
    sf::CircleShape s;

    sf::Vector2f pos;
    sf::Vector2f vel;
    sf::Vector2f acc;

    sf::Vector2f init_pos;
    sf::Vector2f init_vel;

    sf::Color color;
    float mass;
    float radius;

    std::vector<sf::Vector2f> trajectory;

public:
    bool showTrajectory = true;

    // Constructeur
    Object(sf::Vector2f init_pos, sf::Vector2f init_vel, float mass,
           sf::Color color = sf::Color::White, float radius = 20.f);

    // Setters / Getters
    void setColor(sf::Color color);
    sf::Vector2f getPosition() const;
    float getMass() const;
    void setAcceleration(sf::Vector2f accel);

    // Physique & affichage
    void update_physics(float dt);
    void draw(sf::RenderWindow &window);
    sf::FloatRect getGlobalBounds() const;
};

// Fonction externe : calcule la gravité entre tous les objets
void gravityForce(std::vector<Object> &objects);