#include <iostream>
#include <imgui-SFML.h>
#include <imgui.h>
#include "CameraView.hpp"
#include "Object.hpp"

void setCustomImGuiTheme()
{
    ImGuiStyle &style = ImGui::GetStyle();

    // Couleurs principales
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.12f, 0.12f, 0.12f, 1.0f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.18f, 0.18f, 0.18f, 1.0f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.26f, 0.26f, 1.0f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.32f, 0.32f, 0.32f, 1.0f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.18f, 0.18f, 0.18f, 1.0f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.28f, 0.28f, 0.28f, 1.0f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.35f, 0.35f, 0.35f, 1.0f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.0f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.0f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.08f, 0.08f, 0.08f, 1.0f);
    style.Colors[ImGuiCol_Tab] = ImVec4(0.18f, 0.18f, 0.18f, 1.0f);
    style.Colors[ImGuiCol_TabHovered] = ImVec4(0.35f, 0.35f, 0.35f, 1.0f);
    style.Colors[ImGuiCol_TabActive] = ImVec4(0.28f, 0.28f, 0.28f, 1.0f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.4f, 0.7f, 0.9f, 1.0f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.4f, 0.7f, 0.9f, 1.0f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.3f, 0.6f, 0.85f, 1.0f);
    style.Colors[ImGuiCol_Separator] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
    style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
    style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.45f, 0.45f, 0.45f, 1.0f);
    style.Colors[ImGuiCol_Text] = ImVec4(0.9f, 0.9f, 0.9f, 1.0f);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.12f, 0.12f, 0.12f, 1.0f);

    // Rayons et tailles
    style.FrameRounding = 4.0f;
    style.GrabRounding = 4.0f;
    style.WindowRounding = 6.0f;
    style.TabRounding = 4.0f;
    style.WindowPadding = ImVec2(10, 10);
    style.FramePadding = ImVec2(6, 4);
    style.ItemSpacing = ImVec2(6, 4);
}

int main()
{
    using namespace std;

    const unsigned int WINDOW_WIDTH = 1600;
    const unsigned int WINDOW_HEIGHT = 900;
    sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "GravitySim", sf::Style::Default, sf::State::Windowed, sf::ContextSettings{.antiAliasingLevel = 8});
    window.setVerticalSyncEnabled(true);

    //=======IMGUI CONFIG========
    if (!ImGui::SFML::Init(window))
        return -1;

    ImGuiIO &io = ImGui::GetIO();
    ImFont *fontRoboto = io.Fonts->AddFontFromFileTTF("assets/Roboto-Regular.ttf", 18.0f);
    if (!ImGui::SFML::UpdateFontTexture())
        return -1;

    io.FontDefault = fontRoboto;
    setCustomImGuiTheme();

    //===========================
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

        ImGui::Begin("Simulateur gravitationnel");
        ImGui::Text("FPS: %1.f", 1 / dt);
        if (ImGui::Button("Look at this pretty button"))
            objects.push_back(Object({500, 500}, {0, 0}, 1000, sf::Color(0, 166, 212, 255)));

        static float f = 0.0f;
        ImGui::SliderFloat("slider", &f, 0, 1);
        if (ImGui::Button("Focus view"))
        {
            sf::Vector2f target = objects[0].getPosition();
            sf::Vector2f current = view.getCenter();
            view.focusOn(current + (target - current) * followSpeed);
            toggleFocus = !toggleFocus;
        }
        ImGui::ProgressBar(f);
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