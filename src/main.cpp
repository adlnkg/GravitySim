#include "CameraView.hpp"
#include "Object.hpp"
#include "SaveManager.hpp"
#include "imgui_notifications.hpp"
#include <imgui-SFML.h>
#include <imgui.h>
#include <iostream>

using namespace std;

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900

void setCustomImGuiTheme() {
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

int main() {
  srand((unsigned int)time(0));

  sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}),
                          "GravitySim", sf::Style::Default, sf::State::Windowed,
                          sf::ContextSettings{.antiAliasingLevel = 8});

  int FPS_LIMIT = 0; // Limite FPS (0 = illimité)
  bool isVSyncEnabled = false;
  FPS_LIMIT > 0 ? window.setFramerateLimit(FPS_LIMIT)
                : window.setFramerateLimit(0);

  //=======IMGUI CONFIG========
  if (!ImGui::SFML::Init(window))
    return -1;

  ImGuiIO &io = ImGui::GetIO();
  ImFont *fontRoboto =
      io.Fonts->AddFontFromFileTTF("assets/Roboto-Regular.ttf", 18.0f);
  if (!ImGui::SFML::UpdateFontTexture())
    return -1;

  io.FontDefault = fontRoboto;
  setCustomImGuiTheme();
  //===========================
  sf::Clock clock;
  float timeScale = 1.f;
  float accumulator = 0.0f;
  const float FIXED_DT = 0.01f;

  bool isSimRunning = false;
  bool showFPS = true;
  bool showMainPanel = true;
  bool showSettings = false;
  bool showLoadMenu = false;

  // View
  CameraView view({WINDOW_WIDTH, WINDOW_HEIGHT}, {0, 0});
  bool toggleFocus = false;
  int followObjectIndex = -1;
  float followSpeed = 0.5f;

  // Propriétés des objets
  int selectedObjectIndex = -1;
  vector<Object> objects;
  int objNum = 0;

  // Boucle principale
  while (window.isOpen()) {
    float delta_time = clock.restart().asSeconds();
    isSimRunning ? accumulator += delta_time * timeScale
                 : 0; // dt en s depuis la derniere frame

    // Gestion des événements
    while (const std::optional event = window.pollEvent()) {
      ImGui::SFML::ProcessEvent(window, *event);

      // Close window: exit
      if (event->is<sf::Event::Closed>())
        window.close();
      else if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
      } else if (const auto *scroll =
                     event->getIf<sf::Event::MouseWheelScrolled>()) {
        float factor = (scroll->delta > 0) ? 0.9f : 1.1f;
        view.zoomAtPointer(window, factor);
      }
    }

    /*if (toggleFocus)
    {
        sf::Vector2f target = objects[0].getPosition();
        sf::Vector2f current = view.getCenter();
        view.focusOn(current + (target - current) * followSpeed);
    }
    else
        selectedObjectIndex = -1;*/

    ImGui::SFML::Update(window, clock.restart());

    //=======TASKBAR========
    ImGui::BeginMainMenuBar();
    ImGui::Button("Load/Save") ? showLoadMenu = !showLoadMenu : 0;
    if (showLoadMenu) {
      ImGui::Begin("Load/Save");
      std::vector<FilePreview> filesData = getSaves("saves/");
      if (!filesData.empty()) {
        if (ImGui::BeginListBox("##saves")) {
          for (const auto &fileData : filesData) {
            if (ImGui::Selectable(fileData.name.c_str(), false)) {
              try {
                json data = loadJson(fileData.path);
                objects.clear();
                objects = deserializeObjects(data);
                ImGui::InsertNotification("Chargement réussie ✅",
                                          ImGuiNotificationType_Success);
              } catch (const std::runtime_error &e) {
                ImGui::InsertNotification("Erreur lors du chargement ❌: " +
                                              std::string(e.what()),
                                          ImGuiNotificationType_Error);
              }
            }
          }
          ImGui::EndListBox();
        }

      } else {
        ImGui::Text("No save files found in the 'saves' directory.");
      }
      ImGui::Spacing();
      ImGui::Separator();
      ImGui::Spacing();

      static char save_name[64] = "";
      ImGui::InputText("Name", save_name, sizeof(save_name));

      if (ImGui::Button("Save")) {
        vector<json> jsonVec;
        for (auto &obj : objects) {
          jsonVec.push_back(serializeObject(obj));
        }
        try {
          saveJson(jsonVec, save_name,
                   "saves/" + std::string(save_name) + ".json",
                   "Saved on " + std::to_string(time(0)));
          ImGui::InsertNotification("Sauvegarde réussie ✅",
                                    ImGuiNotificationType_Success);
        } catch (const std::runtime_error &e) {
          ImGui::InsertNotification("Erreur lors de la sauvegarde ❌: " +
                                        std::string(e.what()),
                                    ImGuiNotificationType_Error);
        }
      }
      ImGui::End();
    }

    ImGui::Text("File loaded: ");
    ImGui::Button(showMainPanel ? "Close Menu" : "Open Menu")
        ? showMainPanel = !showMainPanel
        : 0;
    ImGui::Button(showSettings ? "Close Settings" : "Settings")
        ? showSettings = !showSettings
        : 0;
    if (ImGui::Button("Reset View")) {
      view = CameraView({WINDOW_WIDTH, WINDOW_HEIGHT},
                        {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2});
      toggleFocus = false;
      followObjectIndex = -1;
    }

    ImGui::Text("Pointer: (%.1f, %.1f)", view.getMouseWorldPos(window).x,
                view.getMouseWorldPos(window).y);
    ImGui::SameLine(ImGui::GetWindowWidth() -
                    100); // ou -80 selon la longueur du texte

    // Récupérer les FPS :
    if (showFPS)
      ImGui::Text("FPS: %.1f", 1.f / delta_time);
    ImGui::EndMainMenuBar();

    if (showMainPanel) {
      ImGui::Begin("GravitySim - Simulation Controls");
      //=======GENERAL UI========
      ImGui::BeginGroup();
      ImGui::Button(isSimRunning ? "Pause" : "Play")
          ? isSimRunning = !isSimRunning
          : 0;

      ImGui::SameLine(0, 20);
      if (ImGui::Button("Reset Simulation")) {
        for (auto &obj : objects)
          obj.reset();
        selectedObjectIndex = -1;
        followObjectIndex = -1;
        toggleFocus = false;
      }
      ImGui::SameLine(0, 20);

      ImGui::SetNextItemWidth(150.0f);
      ImGui::SliderFloat("Time Scale", &timeScale, .1f, 20.0f, "%.1f");
      ImGui::EndGroup();
      ImGui::Spacing();
      ImGui::Separator();
      ImGui::Spacing();

      //=======Objects UI========
      ImGui::BeginGroup();
      ImGui::BeginGroup();
      if (ImGui::BeginListBox("##Objects", ImVec2(150, 250))) {
        for (int i = 0; i < objects.size(); ++i) {
          if (ImGui::Selectable(objects[i].getLabel().c_str(), false)) {
            selectedObjectIndex = i;
            view.focusOn(objects[i].getPosition());
          }
        }
        ImGui::EndListBox();
      }
      if (ImGui::Button("Add Object")) {
        objNum++;
        objects.push_back(Object(
            objects.size() + 1,
            {float(rand() % WINDOW_WIDTH), float(rand() % WINDOW_HEIGHT)},
            {float((rand() % 200) - 100) / 10.f,
             float((rand() % 200) - 100) / 10.f},
            float((rand() % 900) + 1000),
            sf::Color(rand() % 256, rand() % 256, rand() % 256, 255),
            "Astre " + to_string(objNum)));
      }
      ImGui::EndGroup();
      ImGui::SameLine(0, 20);
      //=======Object Properties UI========
      ImGui::BeginChild("Object Properties", ImVec2(300, 0), true);
      if (selectedObjectIndex != -1 && selectedObjectIndex < objects.size()) {
        Object &selectedObject = objects[selectedObjectIndex];

        ImGui::Text("Properties of %s", selectedObject.getLabel().c_str());

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Text("ID: %d", selectedObject.getId());

        if (selectedObject.getParent())
          ImGui::Text("Parent Object: %s",
                      selectedObject.getParent()->getLabel().c_str());

        if (selectedObject.getChildren().size() > 0) {
          ImGui::Text("Satellite(s): ");
          ImGui::SameLine();
          for (auto child : selectedObject.getChildren()) {
            ImGui::SameLine();
            ImGui::Text("%s", child->getLabel().c_str());
          }
        }

        ImGui::Spacing();

        string label = selectedObject.getLabel();
        char buf[64];
        strncpy(buf, label.c_str(), sizeof(buf));
        if (ImGui::InputText("Label", buf, sizeof(buf))) {
          selectedObject.setLabel(string(buf));
        }

        float radius = selectedObject.getRadius();
        if (ImGui::InputFloat("Radius", &radius, 1.f, 100.f)) {
          radius > 1.f ? selectedObject.setRadius(radius)
                       : selectedObject.setRadius(1.f);
        }

        sf::Color color = selectedObject.getColor();
        float colorArr[3] = {color.r / 255.f, color.g / 255.f, color.b / 255.f};
        if (ImGui::ColorEdit3("Color", colorArr)) {
          selectedObject.setColor(sf::Color(
              colorArr[0] * 255, colorArr[1] * 255, colorArr[2] * 255));
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        sf::Vector2f position = selectedObject.getPosition();
        if (ImGui::InputFloat("Position X", &position.x)) {
          selectedObject.setPosition(position);
        }
        if (ImGui::InputFloat("Position Y", &position.y)) {
          selectedObject.setPosition(position);
        }

        ImGui::Spacing();

        float mass = selectedObject.getMass();
        if (ImGui::InputFloat("Mass", &mass, 1.f, 1000000.f)) {
          mass > 0.f ? selectedObject.setMass(mass)
                     : selectedObject.setMass(1.f);
        }

        ImGui::Spacing();

        ImGui::Checkbox("Show Trajectory", &selectedObject.showTrajectory);

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Text("Initials parameters :");
        ImGui::Spacing();

        sf::Vector2f initPos = selectedObject.getInitialPosition();
        if (ImGui::InputFloat("Position X", &initPos.x)) {
          selectedObject.setInitialPosition(initPos);
        }
        if (ImGui::InputFloat("Position Y", &initPos.y)) {
          selectedObject.setInitialPosition(initPos);
        }
        ImGui::Spacing();
        sf::Vector2f initVel = selectedObject.getInitialVelocity();
        if (ImGui::InputFloat("Velocity X", &initVel.x)) {
          selectedObject.setInitialVelocity(initVel);
        } else if (ImGui::InputFloat("Velocity Y", &initVel.y)) {
          selectedObject.setInitialVelocity(initVel);
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        if (ImGui::Button("Reset Object")) {
          selectedObject.reset();
        }
        ImGui::SameLine();
        if (ImGui::Button("Delete Object")) {
          objects.erase(objects.begin() + selectedObjectIndex);
          selectedObjectIndex = -1;
        }
        if (ImGui::Button("Serialize Object")) {
          printJson(serializeObject(selectedObject));
        }

        bool isObjectFollowed = true;
        if (ImGui::Checkbox("Follow Object", &isObjectFollowed)) {
          view.focusOn(selectedObject.getPosition());
        }
      } else
        ImGui::Text("Select or add an object.");
      ImGui::EndChild();
      ImGui::EndGroup();

      ImGui::End();
    }

    if (showSettings) {
      ImGui::Begin("GravitySim - Settings");
      ImGui::Checkbox("Show FPS", &showFPS);
      if (ImGui::SliderInt("Limite FPS", &FPS_LIMIT, 1, 240,
                           FPS_LIMIT == 0   ? "Illimité"
                           : isVSyncEnabled ? "V-Sync enabled"
                                            : "%d"))
        window.setFramerateLimit(FPS_LIMIT);
      if (ImGui::Checkbox("V-Sync", &isVSyncEnabled))
        window.setVerticalSyncEnabled(isVSyncEnabled);
      ImGui::End();
    }

    // ==========================
    // MISE À JOUR DE LA PHYSIQUE
    // ==========================

    if (isSimRunning) {
      while (accumulator >= FIXED_DT) {
        applyGravityForce(objects);

        for (auto &astre : objects) {
          astre.update_physics(FIXED_DT);
        }

        accumulator -= FIXED_DT;
      }
    }

    ImGui::RenderNotifications();
    window.clear();

    view.applyTo(window);

    for (auto &obj : objects) {
      obj.draw(window);
      obj.drawRelativeTrajectory(window);
    }

    window.setView(window.getDefaultView());
    // Elements qui ne sont pas affecter par la vue

    ImGui::SFML::Render(window);
    window.display();
  }

  ImGui::SFML::Shutdown();
}