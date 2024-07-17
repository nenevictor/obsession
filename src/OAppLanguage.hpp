#pragma once

#include <string>
#include <unordered_map>
#include <fstream>

#include "nlohmann/json.hpp"

typedef struct
{
    std::string title;
    std::string name;
    std::string codepoints;
    std::unordered_map<std::string, std::string> dictionary;
} OAppLanguage;

void OAppLanguage__initEnglish(OAppLanguage *_language);

void OAppLanguage__loadFromFile(OAppLanguage *_language, std::string _path, std::string _name);

void OAppLanguage__loadListFromFile(std::vector<std::string> *_languages_list, std::string _path);

const char *OAL__get(OAppLanguage *_language, std::string _name);