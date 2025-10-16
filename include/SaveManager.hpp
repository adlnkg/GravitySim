#pragma once
#include <fstream>
#include <ostream>
#include <nlohmann/json.hpp>
#include <iostream>
#include "Object.hpp"
#include <vector>
using json = nlohmann::json;

void printJson(const json &j);
json serializeObject(const Object &obj);
Object deserializeObject(const json &j);
json mergeJson(const std::vector<json> &jsons);
void saveJson(const json &j, std::string name, std::string PATH);