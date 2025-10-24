#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <algorithm>

class Object
{
private:
    int id;

    sf::CircleShape s;

    sf::Vector2f pos;
    sf::Vector2f vel;
    sf::Vector2f acc;

    sf::Vector2f init_pos;
    sf::Vector2f init_vel;

    sf::Color color;
    float mass;
    float radius;
    std::string label;

    std::vector<sf::Vector2f> trajectory;
    std::vector<sf::Vector2f> trajectoryRelativeToParent;

    Object *parent = nullptr;
    std::vector<Object *> children;

public:
    bool showTrajectory = false;
    bool showRelativeTrajectory = parent ? true : false;

    // Constructeur
    Object(int id, sf::Vector2f init_pos, sf::Vector2f init_vel, float mass,
           sf::Color color = sf::Color::White, std::string label = "Object", float radius = 20.f);

    // Setters / Getters
    int getId() const;

    void setColor(sf::Color color);
    sf::Color getColor() const;
    sf::Vector2f getPosition() const;
    void setPosition(sf::Vector2f pos);

    sf::Vector2f getInitialPosition() const;
    void setInitialPosition(sf::Vector2f init_pos);

    sf::Vector2f getInitialVelocity() const;
    void setInitialVelocity(sf::Vector2f init_vel);

    float getMass() const;
    void setMass(float mass);
    std::string getLabel() const;
    void setLabel(std::string label);
    void setAcceleration(sf::Vector2f accel);

    float getRadius() const;
    void setRadius(float radius);

    std::vector<sf::Vector2f> getTrajectory() const;

    sf::Vector2f getVelocity() const;

    //====== Parent management=======
    Object *getParent() const;
    void setParent(Object *newParent);

    std::vector<Object *> getChildren() const;
    void addChildren(Object *newChild);

    // Physique & affichage
    void update_physics(float dt);
    void draw(sf::RenderWindow &window);
    void drawRelativeTrajectory(sf::RenderWindow &window);
    sf::FloatRect getGlobalBounds() const;
    void reset();
};

// Fonction externe : calcule la gravité entre tous les objets
void applyGravityForce(std::vector<Object> &objects);