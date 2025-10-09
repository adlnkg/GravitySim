#include "Object.hpp"
#include <cmath>
#include <algorithm>

Object::Object(sf::Vector2f init_pos, sf::Vector2f init_vel, float mass,
               sf::Color color, std::string label, float radius)
{
    this->init_pos = init_pos;
    this->init_vel = init_vel;
    this->mass = mass;
    this->color = color;
    this->radius = radius;
    this->label = label;

    pos = init_pos;
    vel = init_vel;

    /*s.setFillColor(color);
    s.setRadius(radius);
    s.setOrigin(sf::Vector2f(radius, radius));
    s.setPosition(pos);*/
}

void Object::setColor(sf::Color color)
{
    this->color = color;
}
sf::Color Object::getColor() const
{
    return color;
}

sf::Vector2f Object::getPosition() const
{
    return pos;
}
void Object::setPosition(sf::Vector2f pos)
{
    this->pos = pos;
}

sf::Vector2f Object::getInitialPosition() const
{
    return init_pos;
}
void Object::setInitialPosition(sf::Vector2f init_pos)
{
    this->init_pos = init_pos;
}

float Object::getMass() const
{
    return mass;
}
void Object::setMass(float mass)
{
    this->mass = mass;
}

std::string Object::getLabel() const
{
    return label;
}
void Object::setLabel(std::string label)
{
    this->label = label;
}

float Object::getRadius() const
{
    return radius;
}
void Object::setRadius(float radius)
{
    this->radius = radius;
}

sf::Vector2f Object::getVelocity() const
{
    return vel;
}
sf::Vector2f Object::getInitialVelocity() const
{
    return init_vel;
}
void Object::setInitialVelocity(sf::Vector2f init_vel)
{
    this->init_vel = init_vel;
}

void Object::setAcceleration(sf::Vector2f accel)
{
    acc = accel;
}

void Object::update_physics(float dt)
{
    vel += acc * dt;
    pos += vel * dt;
    trajectory.push_back(pos);
}

void Object::draw(sf::RenderWindow &window)
{
    s.setPosition(pos);
    s.setFillColor(color);
    s.setRadius(radius);
    s.setOrigin(sf::Vector2f(radius, radius));
    s.setOutlineColor(sf::Color::Red);
    s.setOutlineThickness(1.f);

    if (showTrajectory)
    {
        sf::VertexArray line(sf::PrimitiveType::LineStrip, trajectory.size());
        for (size_t i = 0; i < trajectory.size(); ++i)
        {
            line[i].position = trajectory[i];
            line[i].color = color;
        }
        window.draw(line);

        if (trajectory.size() > 20000) // Limite la taille de la trajectoire pour éviter l'engorgement mémoire
            trajectory.erase(trajectory.begin());
    }
    window.draw(s);
}

void Object::reset()
{
    pos = init_pos;
    vel = init_vel;
    acc = sf::Vector2f(0.f, 0.f);
    trajectory.clear();
}

sf::FloatRect Object::getGlobalBounds() const
{
    return s.getGlobalBounds();
}

void applyGravityForce(std::vector<Object> &objects)
{
    const float G = 100.f;

    for (size_t i = 0; i < objects.size(); i++)
    {
        sf::Vector2f acceleration(0.f, 0.f);

        for (size_t j = 0; j < objects.size(); j++)
        {
            sf::Vector2f r_ij = objects[j].getPosition() - objects[i].getPosition();
            float dist = std::sqrt(r_ij.x * r_ij.x + r_ij.y * r_ij.y);

            if (i != j)
            {
                // Cas ou les astres fusionnent
                if (dist < objects[i].getRadius() + objects[j].getRadius())
                {
                    float m1 = objects[i].getMass();
                    float m2 = objects[j].getMass();
                    float new_mass = m1 + m2;

                    sf::Vector2f new_pos = (m1 * objects[i].getPosition() + m2 * objects[j].getPosition()) / new_mass;
                    sf::Vector2f new_vel = (m1 * objects[i].getVelocity() + m2 * objects[j].getVelocity()) / new_mass;

                    float new_radius = std::cbrt(std::pow(objects[i].getRadius(), 3) + std::pow(objects[j].getRadius(), 3));

                    Object mergedObj(new_pos, new_vel, new_mass,
                                     sf::Color((objects[i].getColor().r + objects[j].getColor().r) / 2,
                                               (objects[i].getColor().g + objects[j].getColor().g) / 2,
                                               (objects[i].getColor().b + objects[j].getColor().b) / 2),
                                     objects[i].getLabel() + "+" + objects[j].getLabel(), new_radius);
                    objects.erase(objects.begin() + i);
                    objects.erase(objects.begin() + j);
                    objects.push_back(mergedObj);

                    return; // Sortir de la fonction pour éviter des comportements indéfinis
                }
                else
                {
                    sf::Vector2f norm_r_ij = r_ij / dist;
                    float F = G * objects[i].getMass() * objects[j].getMass() / (dist * dist);

                    acceleration += (F / objects[i].getMass()) * norm_r_ij;
                }
            }
        }
        objects[i].setAcceleration(acceleration);
    }
}