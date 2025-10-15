#pragma once
#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>
#include "Object.hpp"
using json = nlohmann::json;

void printJson(const json &j);
json serializeObject(const Object &obj);
Object deserializeObject(const json &j);