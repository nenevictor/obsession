#include <string>
#include <vector>
#include <math.h>

#include <raylib.h>

#include "ne_defines.h"
#include "OAppUI.hpp"
#include "OAppSettings.hpp"
#include "OAppLanguage.hpp"

#define VERSION_MAJOR 1
#define VERSION_MINOR 0
#define VERSION_PATCH 1

#define OAPP_SETTINGS_FILE_PATH "resources/settings.json"
#define OAPP_LANGUAGES_FILE_PATH "resources/languages.json"
#define OAPP_LOGO_FILE_PATH "resources/logo.png"
#define OAPP_ICON_FILE_PATH "resources/icon.png"
#define OAPP_FONT_DEFAULT_PATH "resources/Tinos-Bold.ttf"

#define OAPP_WIN_SCORE 1000
#define OAPP_WINDOW_DEFAULT_FPS 60
#define OAPP_GAME_GRID_SIZE 16.0f
#define OAPP_GAME_CIRCLE_SIZE 1.0f

#define COMPARE_COLOR(_c1, _c2) (_c1.r == _c2.r && _c1.g == _c2.g && _c1.b == _c2.b && _c1.a == _c2.a)

typedef enum
{
    gamestatus_begin = 0,
    gamestatus_play,
    gamestatus_lose,
    gamestatus_win,
} OAppGameStatus;

typedef enum
{
    gamemode_infinite = 0,
    gamemode_score,
} OAppGameMode;

typedef enum
{
    layer_intro = 0,
    layer_main_menu,
    layer_settings,
    layer_game,
} OAppLayer;

typedef struct
{
    i32_t score;
    OAppGameStatus status;
} OAppGameStats;

class OApp
{
public:
    OApp();
    ~OApp(){};

    i32_t init();
    i32_t run();
    void quit();

private:
    v2u32 window_size, window_size_restored;
    u32_t window_current_fps;
    u32_t window_flags;
    bool window_quit;

    f64_t frame_delta;
    Vector2 cursor_position;
    OAppCursorState cursor_state;

    Font main_font;

    Texture2D logo_texture;
    OAppElemStyle style;
    OAppGameStats game_stats;
    OAppSettings settings, temp_settings;
    OAppLanguage language;
    std::vector<std::string> languages_list;

    std::vector<v3f32> circles;

    OAppLayer previous_layer;
    OAppLayer current_layer;
    OAppLayer new_layer;

    void toggleFullscreen();

    void reloadFont();

    void prepareCircles();
    void setNewLayer(OAppLayer _new_layer);
    void resetGame();

    void layerIntro();
    void layerMainMenu();
    void layerSettings();
    void layerGame();
};