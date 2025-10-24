#pragma once
#include "Object.hpp"
#include <fstream>
#include <nlohmann/json.hpp>
#include <ostream>
#include <vector>

using json = nlohmann::json;

void checkArray(json &obj, const std::string &key, size_t size,
                const std::vector<double> &defaultValue);
void printJson(const json &j);
json serializeObject(const Object &obj);
std::vector<Object> deserializeObjects(const json &j);
json mergeJson(const std::vector<json> &jsons);
json loadJson(const std::string &PATH);

struct FilePreview {
  std::string name;
  std::string description;
  std::string path;
};

std::vector<FilePreview> getSaves(const std::string &directoryPath);
void saveJson(const json &j, const std::string &name, const std::string &PATH,
              const std::string &description = "");