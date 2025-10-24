#pragma once
#include <chrono>
#include <imgui.h>
#include <string>
#include <vector>

enum ImGuiNotificationType {
  ImGuiNotificationType_Info,
  ImGuiNotificationType_Success,
  ImGuiNotificationType_Warning,
  ImGuiNotificationType_Error
};

struct ImGuiNotification {
  std::string message;
  ImVec4 color;
  std::chrono::steady_clock::time_point startTime;
  float duration;
};

namespace ImGui {

inline std::vector<ImGuiNotification> _notifications;

inline void
InsertNotification(const std::string &msg,
                   ImGuiNotificationType type = ImGuiNotificationType_Info,
                   float duration = 3.0f) {
  ImVec4 color;
  switch (type) {
  case ImGuiNotificationType_Success:
    color = ImVec4(0.2f, 0.8f, 0.2f, 1.0f);
    break;
  case ImGuiNotificationType_Warning:
    color = ImVec4(0.9f, 0.7f, 0.2f, 1.0f);
    break;
  case ImGuiNotificationType_Error:
    color = ImVec4(0.9f, 0.2f, 0.2f, 1.0f);
    break;
  default:
    color = ImVec4(0.3f, 0.5f, 1.0f, 1.0f);
    break;
  }

  _notifications.push_back(
      {msg, color, std::chrono::steady_clock::now(), duration});
}

inline void RenderNotifications() {
  auto now = std::chrono::steady_clock::now();
  float y = 20.0f;

  for (size_t i = 0; i < _notifications.size();) {
    float elapsed =
        std::chrono::duration<float>(now - _notifications[i].startTime).count();

    if (elapsed > _notifications[i].duration) {
      _notifications.erase(_notifications.begin() + i);
      continue;
    }

    float alpha = 1.0f - (elapsed / _notifications[i].duration);
    ImVec4 color = _notifications[i].color;
    color.w = alpha;

    ImGui::SetNextWindowBgAlpha(alpha * 0.85f);
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 250, y));

    ImGui::Begin(("##notif" + std::to_string(i)).c_str(), nullptr,
                 ImGuiWindowFlags_NoDecoration |
                     ImGuiWindowFlags_AlwaysAutoResize |
                     ImGuiWindowFlags_NoFocusOnAppearing |
                     ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove);

    ImGui::TextColored(color, "%s", _notifications[i].message.c_str());
    ImGui::End();

    y += 45.0f;
    ++i;
  }
}
} // namespace ImGui