#include "OAppSettings.hpp"

void OAppSettings__initDefault(OAppSettings *_settings)
{
    if (!_settings)
    {
        return;
    };

    _settings->window_target_fps = 60;
    _settings->window_fullscreen =  false;
    _settings->circle_max_radius = 30.0f;
    _settings->circle_narrowing_speed = 0.001f;
    _settings->circle_count = 3;
    _settings->cursor_size = 5.0f;
    _settings->cursor_smoothing = 0.5f;
    _settings->cursor_speed = 1.0f;
    _settings->hard_mode = false;
    _settings->language = "english";

    _settings->font_size_text = 0.03f;
    _settings->font_size_title = 0.06f;
    _settings->font_path = "default";
};

void OAppSettings__loadFromFile(OAppSettings *_settings, std::string _path)
{
    OAppSettings__initDefault(_settings);

    std::ifstream f(_path);

    if (!f.is_open())
    {
        return;
    };

    nlohmann::json data = nlohmann::json::parse(f);
    f.close();

    JSONSafeSet(_settings->window_target_fps, data, "window_target_fps", u32_t);
    JSONSafeSet(_settings->window_fullscreen, data, "window_fullscreen", bool);
    JSONSafeSet(_settings->circle_max_radius, data, "circle_max_radius", f32_t);
    JSONSafeSet(_settings->circle_narrowing_speed, data, "circle_narrowing_speed", f32_t);
    JSONSafeSet(_settings->circle_count, data, "circle_count", u32_t);
    JSONSafeSet(_settings->cursor_size, data, "cursor_size", f32_t);
    JSONSafeSet(_settings->cursor_smoothing, data, "cursor_smoothing", f32_t);
    JSONSafeSet(_settings->cursor_speed, data, "cursor_speed", f32_t);
    JSONSafeSet(_settings->hard_mode, data, "hard_mode", bool);
    JSONSafeSet(_settings->language, data, "language", std::string);

    JSONSafeSet(_settings->font_size_text, data, "font_size_text", f32_t);
    JSONSafeSet(_settings->font_size_title, data, "font_size_title", f32_t);
    JSONSafeSet(_settings->font_path, data, "font_path", std::string);

    d_clogf("%s", _settings->font_path.c_str());

    data.clear();
};

void OAppSettings__saveToFile(OAppSettings *_settings, std::string _path)
{
    nlohmann::json data;

    data["window_target_fps"] = _settings->window_target_fps;
    data["window_fullscreen"] = _settings->window_fullscreen;
    data["circle_max_radius"] = _settings->circle_max_radius;
    data["circle_narrowing_speed"] = _settings->circle_narrowing_speed;
    data["circle_count"] = _settings->circle_count;
    data["cursor_size"] = _settings->cursor_size;
    data["cursor_smoothing"] = _settings->cursor_smoothing;
    data["cursor_speed"] = _settings->cursor_speed;
    data["language"] = _settings->language;
    data["font_size_text"] = _settings->font_size_text;
    data["font_size_title"] = _settings->font_size_title;
    data["font_path"] = _settings->font_path;

    std::ofstream f(_path);

    if (!f.is_open())
    {
        return;
    };

    f << data;

    f.flush();
    f.close();

    data.clear();
};