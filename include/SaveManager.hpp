#pragma once
#include <fstream>
#include <ostream>
#include <nlohmann/json.hpp>
#include <iostream>
#include "Object.hpp"
#include <vector>
using json = nlohmann::json;

void checkArray(json &obj, const std::string &key, size_t size, const std::vector<double> &defaultValue);
void printJson(const json &j);
json serializeObject(const Object &obj);
Object deserializeObject(const json &j);
json mergeJson(const std::vector<json> &jsons);
json loadJson(const std::string &PATH);
void saveJson(const json &j, const std::string &name, const std::string &PATH);