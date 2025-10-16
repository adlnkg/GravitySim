#include "SaveManager.hpp"

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

    /*std::string label = j.value("label", "Object");

    std::string label = j.value("label", "Object");
    std::string label = j.value("label", "Object");
*/
    Object obj({0, 0}, {0, 0}, 1000, sf::Color::Black, "name", 20.f);
    return obj;
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

void saveJson(json j, std::string name, std::string PATH)
{
    std::ofstream o(name);

    o << std::setw(4) << j << std::endl;
}