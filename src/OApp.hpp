#include <string>
#include <vector>
#include <math.h>

#include <raylib.h>

#include "../include/ne_defines.hpp"
#include "../src/OAppUI.hpp"
#include "../src/OAppSettings.hpp"
#include "../src/OAppLanguage.hpp"

#define VERSION_MAJOR 1
#define VERSION_MINOR 0
#define VERSION_PATCH 0

#define OAPP_SETTINGS_FILE_PATH "resources/settings.json"
#define OAPP_LANGUAGES_FILE_PATH "resources/languages.json"
#define OAPP_LOGO_FILE_PATH "resources/logo.png"
#define OAPP_ICON_FILE_PATH "resources/icon.png"

#define OAPP_WIN_SCORE 1000

#define COMPARE_COLOR(_c1, _c2) (_c1.r == _c2.r && _c1.g == _c2.g && _c1.b == _c2.b && _c1.a == _c2.a)

inline Color colorLerp(const Color _c1, const Color _c2, const f32_t _factor)
{
    f32_t f_c = CLAMP(_factor, 0.0f, 1.0f);
    Color result;

    result.r = LERP(_c1.r, _c2.r, f_c);
    result.g = LERP(_c1.g, _c2.g, f_c);
    result.b = LERP(_c1.b, _c2.b, f_c);
    result.a = LERP(_c1.a, _c2.a, f_c);

    return result;
};

inline f32_t vector2Distance(const f32_t _v1x, const f32_t _v1y, const f32_t _v2x, const f32_t _v2y)
{
    const f32_t result = sqrtf(POW2(_v1x - _v2x) + POW2(_v1y - _v2y));

    return result;
}

inline f32_t randomRange(const f32_t _min, const f32_t _max)
{
    const f32_t result = _min + TOF32(rand()) / TOF32(RAND_MAX) * _max;
    return result;
};

inline Vector2 vector2Follow(Vector2 _object_position, Vector2 _following_position, f32_t _speed)
{
    const f32_t clamped_speed = CLAMP(_speed, 0.0f, 1.0f);

    const Vector2 result = Vector2{
        _following_position.x + (_object_position.x - _following_position.x) * clamped_speed,
        _following_position.y + (_object_position.y - _following_position.y) * clamped_speed};

    return result;
};

inline bool circlesIntersect(const v3f32 _c1, const v3f32 _c2)
{
    bool result = false;

    const f32_t distance = sqrtf(POW2(_c1.x - _c2.x) + POW2(_c1.y - _c2.y));

    result = (distance <= (_c1.z + _c2.z));

    return result;
};

// I stole this peace of code from examples.
inline i32_t *codepointRemoveDuplicates(i32_t *_codepoints, i32_t _codepoint_count, i32_t *_codepoints_result_count)
{
    i32_t codepoints_no_dups_count = _codepoint_count;
    i32_t *codepoints_no_dups = (i32_t *)calloc(_codepoint_count, sizeof(i32_t));
    memcpy(codepoints_no_dups, _codepoints, _codepoint_count * sizeof(i32_t));

    for (i32_t i = 0; i < codepoints_no_dups_count; i++)
    {
        for (i32_t j = i + 1; j < codepoints_no_dups_count; j++)
        {
            if (codepoints_no_dups[i] == codepoints_no_dups[j])
            {
                for (i32_t k = j; k < codepoints_no_dups_count; k++)
                    codepoints_no_dups[k] = codepoints_no_dups[k + 1];

                codepoints_no_dups_count--;
                j--;
            };
        };
    };
    *_codepoints_result_count = codepoints_no_dups_count;
    return codepoints_no_dups;
}

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
    i32_t cursor_keys;

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

OApp::OApp()
{
    this->window_size = v2u32{800, 600};
    this->window_size_restored = this->window_size;
    this->window_current_fps = this->settings.window_target_fps;
    this->window_flags = static_cast<u32_t>(
        ConfigFlags::FLAG_MSAA_4X_HINT |
        ConfigFlags::FLAG_WINDOW_RESIZABLE);
    this->window_quit = false;

    this->frame_delta = 1.0;
    this->cursor_position = Vector2{0.0f, 0.0f};

    OAppSettings__loadFromFile(&this->settings, OAPP_SETTINGS_FILE_PATH);
    this->temp_settings = this->settings;

    OAppElemStyle__initDefault(&this->style);

    OAppLanguage__loadListFromFile(&this->languages_list, OAPP_LANGUAGES_FILE_PATH);
    OAppLanguage__loadFromFile(&this->language, OAPP_LANGUAGES_FILE_PATH, this->settings.language);

    this->game_stats.score = 0;
    this->game_stats.status = OAppGameStatus::gamestatus_begin;

    this->current_layer = OAppLayer::layer_intro;
    this->new_layer = this->current_layer;
};

i32_t OApp::init()
{
    return EXIT_SUCCESS;
};

i32_t OApp::run()
{
    // Initializing variables.
    {
        // raylib stuff.
        {
            SetConfigFlags(this->window_flags);

            const char *window_title = TextFormat(
                "%s %u.%u.%u",
                reinterpret_cast<char *>(const_cast<char *>(OAL__get(&this->language, "title"))),
                VERSION_MAJOR,
                VERSION_MINOR,
                VERSION_PATCH);

            InitWindow(
                TOI32(this->window_size.x),
                TOI32(this->window_size.y),
                window_title);

            SetConfigFlags(this->window_flags);

            SetTargetFPS(this->settings.window_target_fps);
            SetExitKey(0);
        };

        // Load font;
        {
            this->reloadFont();
        };

        // Load logo texture.
        {
            this->logo_texture = LoadTexture(OAPP_LOGO_FILE_PATH);
            SetTextureFilter(this->logo_texture, TEXTURE_FILTER_POINT);
        };

        // Hide cursor.
        {
            HideCursor();
        };

        // Set icon.
        {
            Image icon = LoadImage(OAPP_ICON_FILE_PATH);
            SetWindowIcon(icon);

            UnloadImage(icon);
        };

        // Open window fullscreen if needed.
        {
            if (this->settings.window_fullscreen)
            {
                this->toggleFullscreen();
            };
        }
    };

#ifdef _DEBUG
    SetTraceLogLevel(TraceLogLevel::LOG_ERROR);
#endif

    while (!this->window_quit)
    {
        // Update.
        {
            this->window_quit = WindowShouldClose();
            this->window_size.x = GetScreenWidth();
            this->window_size.y = GetScreenHeight();

            if (!IsWindowFullscreen())
            {
                this->window_size_restored = this->window_size;
            };

            Vector2 temp_cursor_position = GetMousePosition();
            temp_cursor_position.x = temp_cursor_position.x * this->settings.cursor_speed;
            temp_cursor_position.y = temp_cursor_position.y * this->settings.cursor_speed;

            this->cursor_position = vector2Follow(
                this->cursor_position,
                temp_cursor_position,
                this->settings.cursor_smoothing);

            this->cursor_keys = IsMouseButtonPressed(
                                    MouseButton::MOUSE_BUTTON_LEFT) ||
                                IsKeyPressed(KeyboardKey::KEY_SPACE);

            this->settings.cursor_smoothing = CLAMP(this->settings.cursor_smoothing, 0.0f, 1.0f);

            this->style.font_size_title = MIN(this->window_size.x, this->window_size.y) * this->settings.font_size_title;
            this->style.font_size_text = MIN(this->window_size.x, this->window_size.y) * this->settings.font_size_text;

            SetTextLineSpacing(this->style.font_size_text);

            this->current_layer = this->new_layer;

            this->window_current_fps = GetFPS();
            this->window_current_fps =
                this->window_current_fps == 0 ? this->settings.window_target_fps : this->window_current_fps;

            this->frame_delta = TOF64(OAPP_WINDOW_DEFAULT_FPS) / TOF64(this->window_current_fps);
        };

        // Draw.
        BeginDrawing();
        ClearBackground(OAPP_BLACK);

        switch (this->current_layer)
        {
        case OAppLayer::layer_intro:
            this->layerIntro();
            break;
        case OAppLayer::layer_main_menu:
            this->layerMainMenu();
            break;
        case OAppLayer::layer_settings:
            this->layerSettings();
            break;
        case OAppLayer::layer_game:
            this->layerGame();
            break;
        default:
            this->layerMainMenu();
            break;
        };

#ifdef _DEBUG
        DrawText(
            TextFormat(
                "FPS: %lu\n"
                "Frame delta: %.5f\n"
                "Cursor pos: %.0f x %.0f",
                this->window_current_fps,
                this->frame_delta,
                this->cursor_position.x,
                this->cursor_position.y),
            10,
            10,
            20,
            WHITE);
#endif

        DrawCircle(
            this->cursor_position.x,
            this->cursor_position.y,
            this->settings.cursor_size,
            OAPP_YELLOW);

        EndDrawing();
    };

    return EXIT_SUCCESS;
};

void OApp::quit()
{
    UnloadFont(this->main_font);
    UnloadTexture(this->logo_texture);
    CloseWindow();
};

void OApp::toggleFullscreen()
{
    if (IsWindowFullscreen())
    {
        ToggleFullscreen();
        SetWindowSize(this->window_size_restored.x, this->window_size_restored.y);
    }
    else
    {
        const i32_t monitor = GetCurrentMonitor();
        SetWindowSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
        ToggleFullscreen();
    };
}

void OApp::reloadFont()
{
    if (IsFontReady(this->main_font))
    {
        UnloadFont(this->main_font);
    };

    std::string codepoints_string;
    codepoints_string.reserve(512);
    codepoints_string += "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZüö";
    codepoints_string += "0123456789_-=+,.<>:%{}()[]!?";
    codepoints_string += this->language.codepoints;

    i32_t codepoint_count = 0;
    i32_t *codepoints = LoadCodepoints(codepoints_string.c_str(), &codepoint_count);

    i32_t codepoints_no_dups_count = 0;
    i32_t *codepoints_no_dups = codepointRemoveDuplicates(codepoints, codepoint_count, &codepoints_no_dups_count);
    UnloadCodepoints(codepoints);

    const u32_t font_size = this->window_size.y / 5; // really sorry for this magic number :(
    this->main_font = LoadFontEx(this->settings.font_path.c_str(), font_size, codepoints_no_dups, codepoints_no_dups_count);
    SetTextureFilter(this->main_font.texture, TEXTURE_FILTER_BILINEAR);

    this->style.font = this->main_font;
    free(codepoints_no_dups);
};

void OApp::prepareCircles()
{
    // Clear it.
    this->circles.clear();
    this->circles.reserve(this->settings.circle_count);

    // Prepare game circles.
    for (usize_t i = 0; i < this->settings.circle_count; i++)
    {
        v3f32 temp_circle;

        temp_circle.x = randomRange(0, OAPP_GAME_GRID_SIZE);
        temp_circle.y = randomRange(0, OAPP_GAME_GRID_SIZE);
        temp_circle.z = OAPP_GAME_CIRCLE_SIZE;

        this->circles.push_back(temp_circle);
    };
};

void OApp::setNewLayer(OAppLayer _new_layer)
{
    this->previous_layer = this->current_layer;
    this->new_layer = _new_layer;
};

void OApp::resetGame()
{
    this->game_stats.score = 0;
    this->game_stats.status = OAppGameStatus::gamestatus_begin;
    this->prepareCircles();
};

void OApp::layerIntro()
{
    // Draw logo texture
    {
        Rectangle logo_texture_source = Rectangle{
            0.0f,
            0.0f,
            TOF32(this->logo_texture.width),
            TOF32(this->logo_texture.height)};

        Rectangle logo_texture_dest = logo_texture_source;

        const f32_t logo_texture_proportional_height = MIN(this->window_size.x, this->window_size.y) / 8.0f;
        logo_texture_dest.width = floor(logo_texture_dest.width / logo_texture_dest.height * logo_texture_proportional_height);
        logo_texture_dest.height = floor(logo_texture_proportional_height);

        logo_texture_dest.x = floor(this->window_size.x / 2.0f - logo_texture_dest.width / 2.0f);
        logo_texture_dest.y = floor(this->window_size.y / 2.0f - logo_texture_dest.height / 2.0f);

        DrawTexturePro(
            this->logo_texture,
            logo_texture_source,
            logo_texture_dest,
            Vector2{0.0f, 0.0f},
            0,
            WHITE);
    };

    // Draw blinking text
    {
        static f32_t last_update = 0;
        static Color text_color = OAPP_WHITE;
        const f32_t current_time = GetTime();

        // Once per second change color;
        if (current_time - last_update >= 1.0f)
        {
            last_update = current_time;

            if (COMPARE_COLOR(text_color, OAPP_WHITE))
            {
                text_color = OAPP_RED;
            }
            else
            {
                text_color = OAPP_WHITE;
            };
        };

        basic_label(OAL__get(&this->language, "intro_blinking_text"),
                    this->window_size.x / 2.0f,
                    this->window_size.y - this->style.font_size_text,
                    (textalign_left | textalign_right | textalign_down),
                    labeltype_text,
                    &text_color,
                    &this->style);
    };

    if (cursor_keys)
    {
        this->setNewLayer(OAppLayer::layer_main_menu);
    };

    this->window_quit |= IsKeyPressed(KeyboardKey::KEY_ESCAPE);
};

void OApp::layerMainMenu()
{
    // In percent.
    const v2f32 padding = v2f32{
        TOF32(MAX(this->style.font_size_text, this->window_size.x * 0.05f)),
        TOF32(MAX(this->style.font_size_text, this->window_size.y * 0.2f))};
    // Title.
    {
        basic_label(
            OAL__get(&this->language, "title"),
            padding.x,
            padding.y,
            textalign_none,
            labeltype_title,
            &this->style.text_default,
            &this->style);
    };

    // Buttons.
    {
        v2i32 button_position = v2i32{
            TOI32(padding.x),
            TOI32(padding.y + this->style.font_size_title * 2.0f)};

        const u32_t button_width = MIN(this->window_size.x, this->window_size.y) / 2;

        i32_t start_button = basic_button(OAL__get(&this->language, "button_begin"),
                                          button_position.x,
                                          button_position.y,
                                          button_width,
                                          this->style.font_size_text,
                                          this->cursor_position,
                                          this->cursor_keys,
                                          &this->style);

        button_position.y += this->style.font_size_text + this->style.border_thick * 2 + this->style.padding.y * 2;

        i32_t button_settings = basic_button(OAL__get(&this->language, "button_settings"),
                                             button_position.x,
                                             button_position.y,
                                             button_width,
                                             this->style.font_size_text,
                                             this->cursor_position,
                                             this->cursor_keys,
                                             &this->style);

        button_position.y += this->style.font_size_text + this->style.border_thick * 2 + this->style.padding.y * 2;

        this->window_quit |= basic_button(OAL__get(&this->language, "button_exit"),
                                          button_position.x,
                                          button_position.y,
                                          button_width,
                                          this->style.font_size_text,
                                          this->cursor_position,
                                          this->cursor_keys,
                                          &this->style);

        if (start_button)
        {
            this->resetGame();

            this->setNewLayer(OAppLayer::layer_game);
        };

        if (button_settings)
        {
            this->setNewLayer(OAppLayer::layer_settings);
        };
    };
};

void OApp::layerSettings()
{
    // In percent.
    static f32_t scrolling_y;

    v2f32 padding = v2f32{
        TOF32(MAX(this->style.font_size_text, this->window_size.x * 0.05f)),
        TOF32(MAX(this->style.font_size_text, this->window_size.y * 0.1f))};

    v2f32 offset = padding;

    offset.y -= scrolling_y;

    v2f32 button_position = v2f32{0, 0};

    const u32_t button_width = MIN(this->window_size.x, this->window_size.y) / 2;

    // Settings title.
    {
        basic_label(
            OAL__get(&this->language, "title_settings"),
            offset.x + button_position.x,
            offset.y + button_position.y,
            textalign_none,
            labeltype_title,
            &this->style.text_default,
            &this->style);

        button_position.y += this->style.font_size_title * 2;
    };

    // Cursor smoothing.
    {
        basic_slider(TextFormat("%s: %.2f%s", OAL__get(&this->language, "slider_cursor_smoothing"), this->temp_settings.cursor_smoothing * 100.0f, "%"),
                     offset.x + button_position.x,
                     offset.y + button_position.y,
                     button_width,
                     this->style.font_size_text,
                     this->cursor_position,
                     this->cursor_keys,
                     &this->temp_settings.cursor_smoothing,
                     0.0f,
                     1.0f,
                     &this->style);
    };

    // Cursor speed.
    {
        button_position.y += this->style.font_size_text * 2;
        static f32_t new_cursor_speed = this->temp_settings.cursor_speed;
        basic_slider(TextFormat("%s: %.0f%s (undone)", OAL__get(&this->language, "slider_cursor_speed"), this->temp_settings.cursor_speed * 100.0f, "%"),
                     offset.x + button_position.x,
                     offset.y + button_position.y,
                     button_width,
                     this->style.font_size_text,
                     this->cursor_position,
                     this->cursor_keys,
                     &new_cursor_speed,
                     1.0f,
                     2.0f,
                     &this->style);
    };

    // Circles count.
    {
        button_position.y += this->style.font_size_text * 2;
        f32_t circles_count_value = this->temp_settings.circle_count;
        i32_t slider_circles_count = basic_slider(TextFormat("%s: %lu", OAL__get(&this->language, "slider_circles_count"), this->temp_settings.circle_count),
                                                  offset.x + button_position.x,
                                                  offset.y + button_position.y,
                                                  button_width,
                                                  this->style.font_size_text,
                                                  this->cursor_position,
                                                  this->cursor_keys,
                                                  &circles_count_value,
                                                  1.0f,
                                                  10.0f,
                                                  &this->style);
        this->temp_settings.circle_count = floorf(circles_count_value + 0.5f);

        if (slider_circles_count)
        {
            this->prepareCircles();
        };
    };

    // Circles speed.
    {
        button_position.y += this->style.font_size_text * 2;
        basic_slider(TextFormat("%s: %.2f%s", OAL__get(&this->language, "slider_circles_narrowing_speed"), this->temp_settings.circle_narrowing_speed * 1000, "%"),
                     offset.x + button_position.x,
                     offset.y + button_position.y,
                     button_width,
                     this->style.font_size_text,
                     this->cursor_position,
                     this->cursor_keys,
                     &this->temp_settings.circle_narrowing_speed,
                     0.0001f,
                     0.01f,
                     &this->style);
    };

    // Choose language.
    {
        button_position.y += this->style.font_size_text * 2;
        const v2f32 temp_padding = this->style.padding;
        this->style.padding = v2f32{0, 0};

        i32_t language_index_at_list = 0;
        i32_t language_index_at_list_new = 0;

        for (usize_t i = 0; i < this->languages_list.size(); i++)
        {
            if (this->languages_list.at(i) == this->language.name)
            {
                language_index_at_list = i;
                language_index_at_list_new = i;
                break;
            };
        };

        i32_t button_language_previous = basic_button("-",
                                                      offset.x + button_position.x,
                                                      offset.y + button_position.y,
                                                      this->style.font_size_text,
                                                      this->style.font_size_text,
                                                      this->cursor_position,
                                                      this->cursor_keys,
                                                      &this->style);
        if (button_language_previous)
        {
            language_index_at_list_new--;
        };

        basic_label(
            this->language.title.c_str(),
            offset.x + button_position.x + this->style.font_size_text * 2,
            offset.y + button_position.y,
            textalign_none,
            labeltype_text,
            &this->style.text_default,
            &this->style);

        i32_t button_language_next = basic_button("-",
                                                  offset.x + button_position.x + button_width - this->style.font_size_text,
                                                  offset.y + button_position.y,
                                                  this->style.font_size_text,
                                                  this->style.font_size_text,
                                                  this->cursor_position,
                                                  this->cursor_keys,
                                                  &this->style);

        if (button_language_next)
        {
            language_index_at_list_new++;
        };

        if (language_index_at_list_new != language_index_at_list)
        {
            language_index_at_list_new = MOD(
                language_index_at_list_new,
                TOI32(this->languages_list.size()));

            OAppLanguage__loadFromFile(
                &this->language,
                OAPP_LANGUAGES_FILE_PATH,
                this->languages_list.at(language_index_at_list_new));

            this->reloadFont();
            this->temp_settings.language = this->language.name;

            const char *new_window_title = TextFormat(
                "%s %u.%u.%u",
                reinterpret_cast<char *>(const_cast<char *>(OAL__get(&this->language, "title"))),
                VERSION_MAJOR,
                VERSION_MINOR,
                VERSION_PATCH);

            SetWindowTitle(new_window_title);
        };

        this->style.padding = temp_padding;
    };

    // Fullscreen.
    {
        button_position.y += this->style.font_size_text * 2;
        this->temp_settings.window_fullscreen = IsWindowFullscreen();
        i32_t toggle_window_fullscreen_mode = basic_toggle(OAL__get(&this->language, "button_fullscreen"),
                                                           offset.x + button_position.x,
                                                           offset.y + button_position.y,
                                                           this->style.font_size_text,
                                                           this->cursor_position,
                                                           this->cursor_keys,
                                                           &this->temp_settings.window_fullscreen,
                                                           &this->style);

        if (toggle_window_fullscreen_mode)
        {
            this->toggleFullscreen();
        };
    };

    // Hard mode.
    {
        button_position.y += this->style.font_size_text * 2;
        basic_toggle(OAL__get(&this->language, "button_hard_mode"),
                     offset.x + button_position.x,
                     offset.y + button_position.y,
                     this->style.font_size_text,
                     this->cursor_position,
                     this->cursor_keys,
                     &this->temp_settings.hard_mode,
                     &this->style);
    };

    // Use and Close buttons.
    {
        button_position.y += this->style.font_size_text * 2;
        button_position.y += this->style.font_size_text * 2;
        i32_t button_use_settings = basic_button(OAL__get(&this->language, "button_use"),
                                                 offset.x + button_position.x,
                                                 offset.y + button_position.y,
                                                 button_width,
                                                 this->style.font_size_text,
                                                 this->cursor_position,
                                                 this->cursor_keys,
                                                 &this->style);

        button_position.y += this->style.font_size_text * 2;
        i32_t button_close_settings = basic_button(OAL__get(&this->language, "button_close"),
                                                   offset.x + button_position.x,
                                                   offset.y + button_position.y,
                                                   button_width,
                                                   this->style.font_size_text,
                                                   this->cursor_position,
                                                   this->cursor_keys,
                                                   &this->style);

        if (button_use_settings)
        {
            this->settings = this->temp_settings;
            OAppSettings__saveToFile(&this->settings, OAPP_SETTINGS_FILE_PATH);
        };

        if (button_close_settings || IsKeyPressed(KeyboardKey::KEY_ESCAPE))
        {
            this->settings = this->temp_settings;
            this->setNewLayer(this->previous_layer);
        };
    };

    button_position.y += this->style.font_size_text * 2.0f;
    button_position.y += this->style.font_size_text * 2.0f;
    button_position.y += this->style.font_size_text * 2.0f;

    // Page Y-scrolling.
    if (button_position.y + padding.y > this->window_size.y)
    {
        basic_slider_vertical(
            this->window_size.x - this->style.font_size_text * 2.0f,
            this->style.font_size_text,
            this->style.font_size_text,
            this->window_size.y - this->style.font_size_text * 2.0f,
            this->cursor_position,
            this->cursor_keys,
            &scrolling_y,
            0,
            button_position.y - this->window_size.y,
            &this->style);

        scrolling_y -= GetMouseWheelMove();

        if (scrolling_y < 0)
        {
            scrolling_y = 0;
        };

        if (scrolling_y > button_position.y - this->window_size.y)
        {
            scrolling_y = button_position.y - this->window_size.y;
        }
    };
};

void OApp::layerGame()
{
    static i32_t special_counter = 0;

    if (!this->circles.size() ||
        this->settings.circle_count != this->circles.size())
    {
        this->prepareCircles();
    };

    const f32_t margin = 0.1f; // In percent;

    v4f32 natural_game_zone;
    v2f32 natural_block_size;
    f32_t natural_circle_radius;

    natural_game_zone.x = TOF32(this->window_size.x) * margin;
    natural_game_zone.y = TOF32(this->window_size.y) * margin;
    natural_game_zone.z = TOF32(this->window_size.x) * (1.0f - (margin * 2));
    natural_game_zone.w = TOF32(this->window_size.y) * (1.0f - (margin * 2));

    natural_block_size.x = TOF32(natural_game_zone.z) / OAPP_GAME_GRID_SIZE;
    natural_block_size.y = TOF32(natural_game_zone.w) / OAPP_GAME_GRID_SIZE;

    natural_circle_radius = MIN(natural_block_size.x, natural_block_size.y);

    bool was_hit = false;

    // Drawing circle loop.
    for (usize_t i = 0; i < this->circles.size(); i++)
    {
        v3f32 *temp_circle_bounds = &this->circles.at(i);

        if (this->game_stats.status == OAppGameStatus::gamestatus_play)
        {
            temp_circle_bounds->z -= this->settings.circle_narrowing_speed * this->frame_delta;
        };

        if (temp_circle_bounds->z <= 0.0f)
        {
            temp_circle_bounds->z = 0.0f;
            this->game_stats.status = OAppGameStatus::gamestatus_lose;
        };

        v3f32 natural_circle_bounds = this->circles.at(i);
        Color temp_circle_color = colorLerp(
            OAPP_WHITE,
            OAPP_RED,
            1.0f - (natural_circle_bounds.z / OAPP_GAME_CIRCLE_SIZE));

        natural_circle_bounds.x = natural_circle_bounds.x * natural_block_size.x + natural_game_zone.x;
        natural_circle_bounds.y = natural_circle_bounds.y * natural_block_size.y + natural_game_zone.y;
        natural_circle_bounds.z = natural_circle_bounds.z * natural_circle_radius;

        const bool is_cursor_circle_intersect = circlesIntersect(
            v3f32{
                natural_circle_bounds.x,
                natural_circle_bounds.y,
                natural_circle_bounds.z,
            },
            v3f32{
                this->cursor_position.x,
                this->cursor_position.y,
                this->settings.cursor_size});

        if (is_cursor_circle_intersect &&
            this->cursor_keys)
        {
            if (this->game_stats.status == OAppGameStatus::gamestatus_play)
            {
                this->game_stats.score++;
            };

            special_counter++;

            was_hit = true;
            temp_circle_bounds->x = randomRange(0, OAPP_GAME_GRID_SIZE);
            temp_circle_bounds->y = randomRange(0, OAPP_GAME_GRID_SIZE);
            temp_circle_bounds->z = OAPP_GAME_CIRCLE_SIZE;
        };

        if (this->game_stats.score >= OAPP_WIN_SCORE)
        {
            this->game_stats.status = OAppGameStatus::gamestatus_win;
        };

        DrawCircle(
            natural_circle_bounds.x,
            natural_circle_bounds.y,
            natural_circle_bounds.z,
            temp_circle_color);
    };

    if (!was_hit &&
        this->cursor_keys &&
        this->game_stats.status == OAppGameStatus::gamestatus_play)
    {
        this->game_stats.score--;
        special_counter--;
        if (this->settings.hard_mode)
        {
            this->game_stats.status = OAppGameStatus::gamestatus_lose;
        };
    };

    if (this->game_stats.score < 0)
    {
        this->game_stats.score = 0;
    };

    // Game status texts.
    if (this->game_stats.status == OAppGameStatus::gamestatus_begin)
    {
        special_counter = 0;

        if (this->cursor_keys)
        {
            this->game_stats.status = OAppGameStatus::gamestatus_play;
        };

        {
            basic_label(
                OAL__get(&this->language, "title_game_press_space"),
                this->window_size.x / 2.0f,
                this->window_size.y / 2.0f,
                (textalign_left | textalign_right | textalign_top | textalign_down),
                labeltype_title,
                &this->style.text_active,
                &this->style);
        };

        {
            basic_label(
                OAL__get(&this->language, "text_game_press_esc"),
                this->window_size.x / 2.0f,
                this->window_size.y - this->style.font_size_text,
                (textalign_left | textalign_right | textalign_down),
                labeltype_text,
                &this->style.text_active,
                &this->style);
        };
    };

    if (this->game_stats.status == OAppGameStatus::gamestatus_play)
    {
        const char *label_text = TextFormat(
            "%s: %u",
            OAL__get(&this->language, "text_game_score"),
            this->game_stats.score);

        basic_label(
            label_text,
            30,
            30,
            (textalign_none),
            labeltype_text,
            &this->style.text_active,
            &this->style);
    };

    if (this->game_stats.status == OAppGameStatus::gamestatus_lose)
    {
        {
            basic_label(
                OAL__get(&this->language, "title_game_lose"),
                this->window_size.x / 2.0f,
                this->window_size.y / 2.0f,
                (textalign_left | textalign_right | textalign_top | textalign_down),
                labeltype_title,
                &this->style.text_active,
                &this->style);

            if (this->settings.hard_mode)
            {
                basic_label(
                    OAL__get(&this->language, "title_hard_mode"),
                    this->window_size.x / 2.0f,
                    this->window_size.y / 2.0f + this->style.font_size_title * 2,
                    (textalign_left | textalign_right | textalign_top | textalign_down),
                    labeltype_text,
                    &this->style.text_active,
                    &this->style);
            };
        };
        {
            const char *label_text = TextFormat(
                "%s: %u",
                OAL__get(&this->language, "text_game_score"),
                this->game_stats.score);

            basic_label(
                label_text,
                this->window_size.x / 2.0f,
                this->window_size.y / 2.0f + this->style.font_size_title,
                (textalign_left | textalign_right | textalign_top | textalign_down),
                labeltype_text,
                &this->style.text_active,
                &this->style);
        };
        {
            basic_label(
                OAL__get(&this->language, "text_game_result_helper"),
                this->window_size.x / 2.0f,
                this->window_size.y - this->style.font_size_text,
                (textalign_left | textalign_right | textalign_down),
                labeltype_text,
                &this->style.text_active,
                &this->style);
        };
        if (IsKeyPressed(KeyboardKey::KEY_ESCAPE))
        {
            this->setNewLayer(OAppLayer::layer_main_menu);
        };

        if (IsKeyPressed(KeyboardKey::KEY_R))
        {
            this->resetGame();
        };
    };

    if (this->game_stats.status == OAppGameStatus::gamestatus_win)
    {
        {
            basic_label(
                OAL__get(&this->language, "title_game_win"),
                this->window_size.x / 2.0f,
                this->window_size.y / 2.0f,
                (textalign_left | textalign_right | textalign_top | textalign_down),
                labeltype_title,
                &this->style.text_active,
                &this->style);

            if (this->settings.hard_mode)
            {
                basic_label(
                    OAL__get(&this->language, "title_hard_mode"),
                    this->window_size.x / 2.0f,
                    this->window_size.y / 2.0f + this->style.font_size_title * 2,
                    (textalign_left | textalign_right | textalign_top | textalign_down),
                    labeltype_text,
                    &this->style.text_active,
                    &this->style);
            };
        };
        {
            const char *label_text = TextFormat(
                "%s: %u",
                OAL__get(&this->language, "text_game_score"),
                this->game_stats.score);

            basic_label(
                label_text,
                this->window_size.x / 2.0f,
                this->window_size.y / 2.0f + this->style.font_size_title,
                (textalign_left | textalign_right | textalign_top | textalign_down),
                labeltype_text,
                &this->style.text_active,
                &this->style);
        };

        {
            basic_label(
                OAL__get(&this->language, "text_game_result_helper"),
                this->window_size.x / 2.0f,
                this->window_size.y - this->style.font_size_text,
                (textalign_left | textalign_right | textalign_down),
                labeltype_text,
                &this->style.text_active,
                &this->style);
        };

        if (special_counter == 1917)
        {
            this->game_stats.score = special_counter;

            const char *special_win_text = "Wir fürchten nicht, ja nicht,\n"
                                           "Den Donner der Kanonen,\n"
                                           "Ob er uns gleich, ja gleich!\n"
                                           "Zum Untergange ruft.\n"
                                           "Drum wollen wir es nochmals wiederholen:\n"
                                           "Der Tod im Felde ist der schönste Tod.\n";

            basic_label(
                special_win_text,
                this->window_size.x / 2.0f,
                this->style.font_size_text,
                (textalign_left | textalign_right),
                labeltype_text,
                &this->style.text_active,
                &this->style);
        };

        if (IsKeyPressed(KeyboardKey::KEY_ESCAPE))
        {
            this->setNewLayer(OAppLayer::layer_main_menu);
        };

        if (IsKeyPressed(KeyboardKey::KEY_R))
        {
            this->resetGame();
        };
    };

    if ((this->game_stats.status == OAppGameStatus::gamestatus_play ||
         this->game_stats.status == OAppGameStatus::gamestatus_begin) &&
        IsKeyPressed(KeyboardKey::KEY_ESCAPE))
    {
        this->game_stats.status = OAppGameStatus::gamestatus_lose;
    };
};
