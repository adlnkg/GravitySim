#include <iostream>
#include <imgui-SFML.h>
#include <imgui.h>
#include "CameraView.hpp"
#include "Object.hpp"

int main()
{
    using namespace std;

    const unsigned int WINDOW_WIDTH = 1600;
    const unsigned int WINDOW_HEIGHT = 900;

    sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "GravitySim");
    window.setFramerateLimit(165);

    if (!ImGui::SFML::Init(window))
        return -1;

    sf::Clock clock;
    sf::Font font("assets/arial.ttf");

    int timeScale = 1;
    sf::Text timeScaleText(font);

    // View
    CameraView view({WINDOW_WIDTH, WINDOW_HEIGHT}, {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2});
    bool toggleFocus = false;
    float followSpeed = 0.5f;

    // Ajout objets
    vector<Object> objects;
    int objNum = 3;
    objects.push_back(Object({100, 100}, {10, 10}, 1000, sf::Color(189, 166, 212, 255)));
    objects.push_back(Object({1500, 800}, {-10, -5}, 1000, sf::Color(0, 166, 212, 255)));

    // Boucle
    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds() * timeScale; // dt en s depuis la derniere frame

        while (const std::optional event = window.pollEvent())
        {
            ImGui::SFML::ProcessEvent(window, *event);

            // Close window: exit
            if (event->is<sf::Event::Closed>())
                window.close();
            else if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>())
            {

                // Gestion Time Scale
                if (keyPressed->scancode == sf::Keyboard::Scancode::Right)
                    timeScale++;
                if (keyPressed->scancode == sf::Keyboard::Scancode::Left)
                    timeScale--;
                if (keyPressed->scancode == sf::Keyboard::Scancode::Left)
                    timeScale--;

                if (keyPressed->scancode == sf::Keyboard::Scancode::F)
                {
                    toggleFocus = !toggleFocus;
                    cout << toggleFocus << endl;
                }

                if (keyPressed->scancode == sf::Keyboard::Scancode::R)
                    window.setView(window.getDefaultView());
            }
            else if (const auto *scroll = event->getIf<sf::Event::MouseWheelScrolled>())
            {
                float factor = (scroll->delta > 0) ? 0.9f : 1.1f;
                view.zoomAtPointer(window, factor);
            }

            if (toggleFocus)
            {
                sf::Vector2f target = objects[0].getPosition();
                sf::Vector2f current = view.getCenter();
                view.focusOn(current + (target - current) * followSpeed);
            }
        }
        ImGui::SFML::Update(window, clock.restart());

        ImGui::Begin("Hello, world!");
        ImGui::Button("Look at this pretty button");
        ImGui::End();

        gravityForce(objects);
        for (auto &astre : objects)
        {
            astre.update_physics(dt);
        }

        timeScaleText.setString(sf::String(L"Échelle: x") + to_string(timeScale));
        window.clear();

        view.applyTo(window);
        view.applyTo(window);

        for (auto &obj : objects)
        {
            obj.draw(window);
        }

        window.setView(window.getDefaultView());
        // Elements qui ne sont pas affecter par la vue

        window.draw(timeScaleText);

        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
}