#include "Object.hpp"
#include <cmath>
#include <algorithm>

Object::Object(sf::Vector2f init_pos, sf::Vector2f init_vel, float mass,
               sf::Color color, float radius)
{
    this->init_pos = init_pos;
    this->init_vel = init_vel;
    this->mass = mass;
    this->color = color;
    this->radius = radius;

    pos = init_pos;
    vel = init_vel;

    s.setFillColor(color);
    s.setRadius(radius);
    s.setOrigin(sf::Vector2f(radius, radius));
    s.setPosition(pos);
}

void Object::setColor(sf::Color color)
{
    this->color = color;
}

sf::Vector2f Object::getPosition() const
{
    return pos;
}

float Object::getMass() const
{
    return mass;
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

    if (showTrajectory)
    {
        sf::VertexArray line(sf::PrimitiveType::LineStrip, trajectory.size());
        for (size_t i = 0; i < trajectory.size(); ++i)
        {
            line[i].position = trajectory[i];
            line[i].color = color;
        }
        window.draw(line);
    }

    window.draw(s);
}

sf::FloatRect Object::getGlobalBounds() const
{
    return s.getGlobalBounds();
}

void gravityForce(std::vector<Object> &objects)
{
    const float G = 100.f;

    for (size_t i = 0; i < objects.size(); i++)
    {
        sf::Vector2f acceleration(0.f, 0.f);

        for (size_t j = 0; j < objects.size(); j++)
        {
            if (i != j)
            {
                sf::Vector2f r_ij = objects[j].getPosition() - objects[i].getPosition();
                float dist = std::sqrt(r_ij.x * r_ij.x + r_ij.y * r_ij.y);
                dist = std::max(dist, 1.f); // éviter la division par zéro

                sf::Vector2f norm_r_ij = r_ij / dist;
                float F = G * objects[i].getMass() * objects[j].getMass() / (dist * dist);

                acceleration += (F / objects[i].getMass()) * norm_r_ij;
            }
        }
        objects[i].setAcceleration(acceleration);
    }
}