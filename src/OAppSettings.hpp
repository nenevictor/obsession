#pragma once

#include <string>
#include <fstream>

#include "nlohmann/json.hpp"
#include "ne_defines.h"

#define JSONSafeSet(_value, _json, _key, _value_type) (_value = (_json.count(_key) ? _json[_key].get<_value_type>() : _value))

typedef struct
{
    u32_t window_target_fps;
    bool window_fullscreen;

    f32_t circle_max_radius;
    f32_t circle_narrowing_speed;
    u32_t circle_count;

    f32_t cursor_size;
    f32_t cursor_smoothing;
    f32_t cursor_speed;

    bool hard_mode;

    std::string language;

    f32_t font_size_text;
    f32_t font_size_title;
    std::string font_path;

} OAppSettings;

void OAppSettings__initDefault(OAppSettings *_settings);

void OAppSettings__loadFromFile(OAppSettings *_settings, std::string _path);

void OAppSettings__saveToFile(OAppSettings *_settings, std::string _path);