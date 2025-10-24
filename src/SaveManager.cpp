#include "SaveManager.hpp"
#include <filesystem>
#include <iostream>

void checkArray(json &obj, const std::string &key, size_t size,
                const std::vector<double> &defaultValue) {
  if (!obj.contains(key) || !obj[key].is_array() || obj[key].size() != size) {
    obj[key] = defaultValue;
  }
}

void printJson(const json &j) { std::cout << j.dump(4) << std::endl; }

json serializeObject(const Object &obj) {
  json jfile;
  json j = jfile["objects"];
  j["id"] = obj.getId();
  j["label"] = obj.getLabel();
  j["color"] = {obj.getColor().r, obj.getColor().g, obj.getColor().b};
  j["mass"] = obj.getMass();
  j["radius"] = obj.getRadius();
  j["position"] = {obj.getInitialPosition().x, obj.getInitialPosition().y};
  j["velocity"] = {obj.getInitialVelocity().x, obj.getInitialVelocity().y};
  j["Children"] = json::array();

  for (const auto &child : obj.getChildren()) {
    j["Children"].push_back(child->getId());
  }
  return j;
}

/*json deserializeObjects(const std::vector<Object &> &objects)
{
    for (size_t i = 0; i < objects.size(); i++)
    {
        Object &obj = objects[i];
        json s_obj = serializeObject(obj);
    }

    return {};
}*/

std::vector<Object> deserializeObjects(const json &j) {
  std::vector<Object> list_obj;

  //=====Construction des objets=====
  if (!j.contains("objects") || !j["objects"].is_array()) {
    throw std::runtime_error(
        "La clé 'objects' est manquante ou n'est pas un tableau JSON");
  }

  const auto &objects = j["objects"];

  for (size_t i = 0; i < objects.size(); ++i) {
    const auto &obj = objects[i];

    std::string label = obj.value("label", "Object");
    float mass = obj.value("mass", 1500.f);
    float radius = obj.value("radius", 20.f);

    sf::Color color(255, 255, 255); // Valeur par défaut
    sf::Vector2f position(0.f, 0.f);
    sf::Vector2f velocity(0.f, 0.f);

    if (obj.contains("position") && obj["position"].is_array() &&
        obj["position"].size() == 2) {
      position.x = obj["position"][0];
      position.y = obj["position"][1];
    }

    if (obj.contains("velocity") && obj["velocity"].is_array() &&
        obj["velocity"].size() == 2) {
      velocity.x = obj["velocity"][0];
      velocity.y = obj["velocity"][1];
    }

    if (obj.contains("color") && obj["color"].is_array() &&
        obj["color"].size() == 3) {
      color.r = obj["color"][0];
      color.g = obj["color"][1];
      color.b = obj["color"][2];
    }

    Object astre(i + 1, position, velocity, mass, color, label, radius);
    list_obj.push_back(astre);
  }

  //=====Assignation des relations=====
  for (size_t i = 0; i < objects.size(); ++i) {
    const auto &j_obj = objects[i];
    Object &obj = list_obj[i];

    if (j_obj.contains("parent")) {
      int parentIndex = j_obj["parent"];

      // Vérifier que l'index est valide
      if (parentIndex >= 0 &&
          static_cast<size_t>(parentIndex) < list_obj.size()) {
        obj.setParent(&list_obj[parentIndex]);
        list_obj[parentIndex].addChildren(&obj);
      }
    }
  }

  return list_obj;
}

json mergeJson(std::vector<json> &jsons) {
  json mergedJson;

  for (json &j : jsons) {
    mergedJson.push_back(j);
  }

  return mergedJson;
}

json loadJson(const std::string &PATH) {
  std::ifstream i(PATH);
  if (!i.is_open()) {
    throw std::runtime_error("Impossible d'ouvrir le fichier : " + PATH);
  }

  try {
    return json::parse(i);
  } catch (const json::parse_error &e) {
    throw std::runtime_error("Erreur de parsing JSON : " +
                             std::string(e.what()));
  }
}

std::vector<FilePreview> getSaves(const std::string &directoryPath) {
  std::vector<FilePreview> filesData;

  try {
    for (const auto &entry :
         std::filesystem::directory_iterator(directoryPath)) {

      FilePreview preview;

      if (entry.is_regular_file() && entry.path().extension() == ".json") {
        std::ifstream file(entry.path());
        json j = json::parse(file);

        if (j.contains("name")) {
          preview.name = j["name"];
        } else if (j.contains("description")) {
          preview.description = j["description"];
        }

        else {
          preview.name = "Unnamed - " + entry.path().filename().string();
          preview.description = "No description available.";
        }
        preview.path = entry.path().string();
      }
      filesData.push_back(preview);
    }
  } catch (const std::filesystem::filesystem_error &e) {
    throw std::runtime_error("Erreur lors de la lecture du répertoire : " +
                             std::string(e.what()));
  }
  return filesData;
}

void saveJson(const json &j, const std::string &name,
              const std::string &description) {
  json jfile;
  jfile["name"] = name;
  jfile["description"] = description;
  jfile["objects"] = j;

  std::ofstream o(name);
  o << std::setw(4) << jfile << std::endl;
}