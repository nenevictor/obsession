#include "OAppLanguage.hpp"

void OAppLanguage__initEnglish(OAppLanguage *_language)
{
    if (!_language)
    {
        return;
    };

    _language->title = "English";
    _language->name = "english";
    _language->codepoints = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

    _language->dictionary["title"] = "Obsession.";

    _language->dictionary["intro_blinking_text"] = "Press SPACE to begin!";
    _language->dictionary["title_settings"] = "Settings";

    _language->dictionary["button_begin"] = "BEGIN";
    _language->dictionary["button_settings"] = "SETTINGS";
    _language->dictionary["button_exit"] = "EXIT";

    _language->dictionary["slider_cursor_smoothing"] = "Cursor smoothing";
    _language->dictionary["slider_cursor_speed"] = "Cursor speed";
    _language->dictionary["slider_circles_count"] = "Circles count";
    _language->dictionary["slider_circles_narrowing_speed"] = "Circles narrowing speed";

    _language->dictionary["button_fullscreen"] = "Fullscreen";
    _language->dictionary["button_hard_mode"] = "Hard mode";
    _language->dictionary["button_use"] = "USE";
    _language->dictionary["button_close"] = "CLOSE";
    _language->dictionary["title_hard_mode"] = "Hard mode";

    _language->dictionary["title_game_press_space"] = "Press SPACE to begin!";
    _language->dictionary["text_game_press_esc"] = "Or press ESC to lose...";
    _language->dictionary["text_game_score"] = "Score";
    _language->dictionary["title_game_lose"] = "Kill yourself!";
    _language->dictionary["title_game_win"] = "You win!";
    _language->dictionary["text_game_result_helper"] = "ESC - go to main menu    R - restart";
};

void OAppLanguage__loadFromFile(OAppLanguage *_language, std::string _path, std::string _name)
{
    if (!_language)
    {
        return;
    };
    OAppLanguage__initEnglish(_language);

    if (_name == "english")
    {
        return;
    };

    std::ifstream f(_path);

    if (!f.is_open())
    {
        return;
    };

    nlohmann::json data = nlohmann::json::parse(f);

    if (!data.count(_name) ||
        !data[_name].count("codepoints") ||
        !data[_name].count("dictionary"))
    {
        return;
    };

    if (!data[_name].count("title"))
    {
        _language->title = _name;
    }
    else
    {
        _language->title = data[_name]["title"];
    }

    _language->name = _name;
    _language->codepoints = data[_name]["codepoints"];

    for (auto &object : data[_name]["dictionary"].get<nlohmann::json::object_t>())
    {
        if (!object.second.is_string())
        {
            continue;
        };

        _language->dictionary[object.first] = object.second;
    };

    f.close();
    data.clear();
};

void OAppLanguage__loadListFromFile(std::vector<std::string> *_languages_list, std::string _path)
{
    if (!_languages_list)
    {
        return;
    };

    std::ifstream f(_path);

    if (!f.is_open())
    {
        return;
    };

    nlohmann::json data = nlohmann::json::parse(f);
    f.close();

    _languages_list->clear();
    _languages_list->push_back("english");

    for (auto &object : data.get<nlohmann::json::object_t>())
    {
        _languages_list->push_back(object.first);
    };

    data.clear();
};

const char *OAL__get(OAppLanguage *_language, std::string _name)
{
    if (!_language)
    {
        return "untranslated";
    };

    if (_language->dictionary.count(_name))
    {
        return _language->dictionary.at(_name).c_str();
    }
    else
    {
        return "untranslated";
    };
};