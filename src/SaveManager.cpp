#include "SaveManager.hpp"

void checkArray(json &obj, const std::string &key, size_t size, const std::vector<double> &defaultValue)
{
    if (!obj.contains(key) || !obj[key].is_array() || obj[key].size() != size)
    {
        obj[key] = defaultValue;
    }
}

void printJson(const json &j)
{
    std::cout << j.dump(4) << std::endl;
}

json serializeObject(const Object &obj)
{
    json j;

    j["label"] = obj.getLabel();
    j["color"] = {obj.getColor().r, obj.getColor().g, obj.getColor().b};
    j["mass"] = obj.getMass();
    j["radius"] = obj.getRadius();
    j["position"] = {obj.getInitialPosition().x, obj.getInitialPosition().y};
    j["velocity"] = {obj.getInitialVelocity().x, obj.getInitialVelocity().y};

    return j;
}

Object deserializeObject(const json &j)
{

    std::string label = j.value("label", "Object");

    float mass = j.value("mass", 1500);
    float radius = j.value("radius", 20.f);

    sf::Color color;
    sf::Vector2f position;
    sf::Vector2f velocity;

    if (j.contains("position") && j["position"].is_array() && j["position"].size() == 2)
    {
        position.x = j["position"][0];
        position.y = j["position"][1];
    }

    if (j.contains("velocity") && j["velocity"].is_array() && j["velocity"].size() == 2)
    {
        velocity.x = j["velocity"][0];
        velocity.y = j["velocity"][1];
    }

    if (j.contains("color") && j["color"].is_array() && j["color"].size() == 3)
    {
        color.r = j["color"][0];
        color.g = j["color"][1];
        color.b = j["color"][2];
    }

    return Object(position, velocity, mass, color, label, radius);
}

json mergeJson(std::vector<json> &jsons)
{
    json mergedJson = json::array();

    for (json &j : jsons)
    {
        mergedJson.push_back(j);
    }

    return mergedJson;
}

json loadJson(const std::string &PATH)
{
    std::ifstream i(PATH);
    return json::parse(i);
}

void saveJson(const json &j, const std::string &name, const std::string &PATH)
{
    std::ofstream o(name);
    o << std::setw(4) << j << std::endl;
}