#pragma once

#include "ne_defines.h"

#include <string>
#include <vector>
#include <math.h>

#include <raylib.h>

#define OAPP_WHITE \
    Color { 244, 236, 236, 255 }

#define OAPP_RED \
    Color { 197, 13, 13, 255 }

#define OAPP_YELLOW \
    Color { 229, 200, 37, 255 }

#define OAPP_BLACK \
    Color { 32, 30, 30, 255 }

typedef enum OAppCursorState
{
    cursor_up = 0,
    cursor_pressed,
    cursor_down
} OAppCursorState;

typedef struct OAppElemStyle
{
    Font font;
    u32_t font_size_title;
    u32_t font_size_text;
    u32_t font_spacing;

    f32_t border_thick;
    v2f32 padding;

    Color back_default;
    Color back_active;
    Color back_hover;

    Color border_default;
    Color border_active;
    Color border_hover;

    Color text_default;
    Color text_active;
    Color text_hover;
} OAppElemStyle;

typedef enum OAppElemStyleTextAlign
{
    textalign_none = 1 << 0,
    textalign_left = 1 << 1,
    textalign_right = 1 << 2,
    textalign_top = 1 << 3,
    textalign_down = 1 << 4,

} OAppElemStyleTextAlign;

typedef enum OAppElemStyleLabelType
{
    labeltype_text = 0,
    labeltype_title,
} OAppElemStyleLabelType;

void OAppElemStyle__initDefault(OAppElemStyle *_style);

void basic_label(
    const char *_text,
    const i32_t _x,
    const i32_t _y,
    const u32_t _align,
    const u32_t _label_type,
    const Color *_color,
    const OAppElemStyle *_style);

i32_t basic_button(
    const char *_text,
    const i32_t _x,
    const i32_t _y,
    const u32_t _min_w,
    const u32_t _min_h,
    const Vector2 _cursor_positions,
    const OAppCursorState _cursor_state,
    const OAppElemStyle *_style);

i32_t basic_slider(
    const char *_text,
    const i32_t _x,
    const i32_t _y,
    const u32_t _min_w,
    const u32_t _min_h,
    const Vector2 _cursor_positions,
    const OAppCursorState _cursor_state,
    f32_t *_value,
    const f32_t _value_min,
    const f32_t _value_max,
    const OAppElemStyle *_style);

i32_t basic_slider_vertical(
    const i32_t _x,
    const i32_t _y,
    const u32_t _min_w,
    const u32_t _min_h,
    const Vector2 _cursor_positions,
    const OAppCursorState _cursor_state,
    f32_t *_value,
    const f32_t _value_min,
    const f32_t _value_max,
    const OAppElemStyle *_style);

i32_t basic_toggle(
    const char *_text,
    const i32_t _x,
    const i32_t _y,
    const u32_t _min_size,
    const Vector2 _cursor_positions,
    const OAppCursorState _cursor_state,
    bool *_value,
    const OAppElemStyle *_style);
