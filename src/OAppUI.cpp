#include "OAppUI.hpp"

void OAppElemStyle__initDefault(OAppElemStyle *_style)
{
	if (!_style)
	{
		return;
	};

	_style->font_spacing = 2;
	_style->border_thick = 4.0f;
	_style->padding = v2f32{30.0f, 15.0f};
	_style->back_default = OAPP_BLACK;
	_style->back_active = OAPP_BLACK;
	_style->back_hover = OAPP_BLACK;
	_style->border_default = OAPP_WHITE;
	_style->border_active = OAPP_RED;
	_style->border_hover = OAPP_YELLOW;
	_style->text_default = OAPP_WHITE;
	_style->text_active = OAPP_RED;
	_style->text_hover = OAPP_YELLOW;
};

void basic_label(
	const char *_text,
	const i32_t _x,
	const i32_t _y,
	const u32_t _align,
	const u32_t _label_type,
	const Color *_color,
	const OAppElemStyle *_style)
{
	Vector2 text_pos;
	u32_t text_size;

	switch (_label_type)
	{
	case OAppElemStyleLabelType::labeltype_title:
		text_size = _style->font_size_title;
		break;
	default:
		text_size = _style->font_size_text;
		break;
	}

	text_pos = MeasureTextEx(
		_style->font,
		_text,
		text_size,
		_style->font_spacing);

	if (HASFLAG(
			_align,
			OAppElemStyleTextAlign::textalign_none))
	{
		text_pos.x = _x;
		text_pos.y = _y;
	}
	else
	{
		if (HASFLAG(
				_align,
				OAppElemStyleTextAlign::textalign_left |
					OAppElemStyleTextAlign::textalign_right))
		{
			text_pos.x = _x - text_pos.x / 2.0f;
		}
		else
		{
			if (HASFLAG(
					_align,
					OAppElemStyleTextAlign::textalign_right))
			{
				text_pos.x = _x - text_pos.x;
			}
			else
			{
				text_pos.x = _x;
			};
		};

		if (HASFLAG(
				_align,
				OAppElemStyleTextAlign::textalign_top |
					OAppElemStyleTextAlign::textalign_down))
		{
			text_pos.y = _y - text_pos.y / 2.0f;
		}
		else
		{
			if (HASFLAG(
					_align,
					OAppElemStyleTextAlign::textalign_down))
			{
				text_pos.y = _y - text_pos.y;
			}
			else
			{
				text_pos.y = _y;
			};
		}
	};

	Color temp_color = _style->text_default;

	if (_color)
	{
		temp_color = *_color;
	};

	DrawTextEx(
		_style->font,
		_text,
		text_pos,
		text_size,
		_style->font_spacing,
		temp_color);
};

i32_t basic_button(
	const char *_text,
	const i32_t _x,
	const i32_t _y,
	const u32_t _min_w,
	const u32_t _min_h,
	const Vector2 _cursor_positions,
	const OAppCursorState _cursor_state,
	const OAppElemStyle *_style)
{
	if (!_style)
	{
		return 0;
	};

	Color back_color = _style->back_default;
	Color border_color = _style->border_default;
	Color text_color = _style->text_default;

	i32_t result = 0;
	static i32_t previous_result = 0;

	Vector2 text_pos = MeasureTextEx(
		_style->font,
		_text,
		_style->font_size_text,
		_style->font_spacing);

	Rectangle back_pos = Rectangle{
		TOF32(_x),
		TOF32(_y),
		TOF32(MAX(_min_w, text_pos.x + _style->padding.x + _style->border_thick)),
		TOF32(MAX(_min_h, text_pos.y + _style->padding.y + _style->border_thick)),
	};

	if (_cursor_positions.x > back_pos.x && _cursor_positions.x < back_pos.x + back_pos.width &&
		_cursor_positions.y > back_pos.y && _cursor_positions.y < back_pos.y + back_pos.height)
	{
		back_color = _style->back_hover;
		border_color = _style->border_hover;
		text_color = _style->text_hover;
		result = 0;

		if (_cursor_state == cursor_pressed)
		{
			back_color = _style->back_active;
			border_color = _style->border_active;
			text_color = _style->text_active;

			result = previous_result ? 0 : 1;
			previous_result = 1;
		}
		else
		{
			previous_result = 0;
		};
	}
	else
	{
		result = 0;
	};

	text_pos.x = _x + back_pos.width / 2 - text_pos.x / 2;
	text_pos.y = _y + back_pos.height / 2 - text_pos.y / 2;

	DrawRectangleLinesEx(
		back_pos,
		_style->border_thick,
		border_color);

	DrawTextEx(
		_style->font,
		_text,
		text_pos,
		_style->font_size_text,
		_style->font_spacing,
		text_color);

	return result;
};

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
	const OAppElemStyle *_style)
{
	if (!_style || !_value)
	{
		return 0;
	};

	Color back_color = _style->back_default;
	Color border_color = _style->border_default;
	Color text_color = _style->text_default;

	i32_t result = 0;
	static i32_t previous_result = 0;

	const Rectangle slider_bounds = Rectangle{
		TOF32(_x),
		TOF32(_y),
		TOF32(_min_w),
		TOF32(_min_h),
	};

	Rectangle slide_button_bounds = Rectangle{
		TOF32(_x) + TOF32(_min_w - _min_h) * (*_value - _value_min) / (_value_max - _value_min),
		TOF32(_y),
		TOF32(_min_h),
		TOF32(_min_h),
	};

	Vector2 text_pos = MeasureTextEx(
		_style->font,
		_text,
		_style->font_size_text,
		_style->font_spacing);

	text_pos.x = TOF32(_x) + TOF32(_min_w) + _style->font_size_text;
	text_pos.y = TOF32(_y) + TOF32(_min_h / 2.0f) - TOF32(text_pos.y / 2.0f);

	if (_cursor_positions.x > slider_bounds.x && _cursor_positions.x < slider_bounds.x + slider_bounds.width &&
		_cursor_positions.y > slider_bounds.y && _cursor_positions.y < slider_bounds.y + slider_bounds.height)
	{
		border_color = _style->border_hover;

		result = 0;

		if (_cursor_state == cursor_pressed ||
			_cursor_state == cursor_down)
		{
			border_color = _style->border_active;

			const f32_t new_value = _value_min + ((_cursor_positions.x - _x) / _min_w) * (_value_max - _value_min);

			*_value = new_value;

			result = previous_result ? 0 : 1;
			previous_result = 1;
		}
		else
		{
			previous_result = 0;
		};
	}
	else
	{
		result = 0;
	};

	DrawRectangleRec(
		Rectangle{
			TOF32(_x),
			TOF32(_y + _min_h / 2.0f - _style->border_thick / 2),
			TOF32(_min_w),
			TOF32(_style->border_thick)},
		border_color);

	DrawRectangleRec(
		slide_button_bounds,
		back_color);

	DrawRectangleLinesEx(
		slide_button_bounds,
		_style->border_thick,
		border_color);

	DrawTextEx(
		_style->font,
		_text,
		text_pos,
		_style->font_size_text,
		_style->font_spacing,
		text_color);

	return result;
};

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
	const OAppElemStyle *_style)
{
	if (!_style || !_value)
	{
		return 0;
	};

	Color back_color = _style->back_default;
	Color border_color = _style->border_default;

	i32_t result = 0;
	static i32_t previous_result = 0;

	const Rectangle slider_bounds = Rectangle{
		TOF32(_x),
		TOF32(_y),
		TOF32(_min_w),
		TOF32(_min_h),
	};

	Rectangle slide_button_bounds = Rectangle{
		TOF32(_x),
		TOF32(_y) + TOF32(_min_h - _min_w) * (*_value - _value_min) / (_value_max - _value_min),
		TOF32(_min_w),
		TOF32(_min_w),
	};

	if (_cursor_positions.x > slider_bounds.x && _cursor_positions.x < slider_bounds.x + slider_bounds.width &&
		_cursor_positions.y > slider_bounds.y && _cursor_positions.y < slider_bounds.y + slider_bounds.height)
	{
		border_color = _style->border_hover;

		result = 0;

		if (_cursor_state == cursor_pressed ||
			_cursor_state == cursor_down)
		{
			border_color = _style->border_active;

			const f32_t new_value = _value_min + ((_cursor_positions.y - _y) / _min_h) * (_value_max - _value_min);

			*_value = new_value;

			result = previous_result ? 0 : 1;
			previous_result = 1;
		}
		else
		{
			previous_result = 0;
		};
	}
	else
	{
		result = 0;
	};

	DrawRectangleRec(
		Rectangle{
			TOF32(_x + _min_w / 2.0f - _style->border_thick / 2),
			TOF32(_y),
			TOF32(_style->border_thick),
			TOF32(_min_h)},
		border_color);

	DrawRectangleRec(
		slide_button_bounds,
		back_color);

	DrawRectangleLinesEx(
		slide_button_bounds,
		_style->border_thick,
		border_color);

	return result;
};

i32_t basic_toggle(
	const char *_text,
	const i32_t _x,
	const i32_t _y,
	const u32_t _min_size,
	const Vector2 _cursor_positions,
	const OAppCursorState _cursor_state,
	bool *_value,
	const OAppElemStyle *_style)
{
	if (!_style || !_value)
	{
		return 0;
	};

	Color border_color = _style->border_default;

	i32_t result = 0;
	static i32_t previous_result = 0;

	Rectangle toggle_center_bounds = Rectangle{
		TOF32(_x) + _style->border_thick * 2.0f,
		TOF32(_y) + _style->border_thick * 2.0f,
		TOF32(_min_size) - _style->border_thick * 4.0f,
		TOF32(_min_size) - _style->border_thick * 4.0f};

	Rectangle toggle_border_bounds = Rectangle{
		TOF32(_x),
		TOF32(_y),
		TOF32(_min_size),
		TOF32(_min_size)};

	Vector2 text_pos = MeasureTextEx(
		_style->font,
		_text,
		_style->font_size_text,
		_style->font_spacing);

	Rectangle toggle_bounds = Rectangle{
		TOF32(_x),
		TOF32(_y),
		TOF32(_min_size) + text_pos.x + _style->font_size_text,
		TOF32(_min_size)};

	text_pos.x = TOF32(_x) + TOF32(_min_size) + _style->font_size_text;
	text_pos.y = TOF32(_y) + TOF32(_min_size / 2.0f) - TOF32(text_pos.y / 2.0f);

	if (_cursor_positions.x > toggle_bounds.x && _cursor_positions.x < toggle_bounds.x + toggle_bounds.width &&
		_cursor_positions.y > toggle_bounds.y && _cursor_positions.y < toggle_bounds.y + toggle_bounds.height)
	{
		border_color = _style->border_hover;

		result = 0;

		if (_cursor_state == cursor_pressed)
		{
			border_color = _style->border_active;

			if (!previous_result)
			{
				*_value = !*_value;
			};

			result = previous_result ? 0 : 1;
			previous_result = 1;
		}
		else
		{
			previous_result = 0;
		};
	}
	else
	{
		result = 0;
	};

	DrawRectangleLinesEx(toggle_border_bounds, _style->border_thick, border_color);

	if (*_value)
	{
		DrawRectangleRec(toggle_center_bounds, border_color);
	};

	DrawTextEx(
		_style->font,
		_text,
		text_pos,
		_style->font_size_text,
		_style->font_spacing,
		border_color);

	return result;
};