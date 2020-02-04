/*
 * Copyright (C) 2019 Beijing FMSoft Technologies Co., Ltd.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef __HIVIEW_CANVAS_H__
#define __HIVIEW_CANVAS_H__

#include <stdint.h>

#define INVALID_HANDLE 0

#define LINE_BUTT_CAP    0
#define LINE_ROUND_CAP   1
#define LINE_SQUARE_CAP  2

#define LINE_MITER_JOIN  0
#define LINE_ROUND_JOIN  1
#define LINE_BEVEL_JOIN  2


#define TEXT_ALIGN_START   0
#define TEXT_ALIGN_END     1
#define TEXT_ALIGN_LEFT    2
#define TEXT_ALIGN_RIGHT   3
#define TEXT_ALIGN_CENTER  4

#define TEXT_BASELINE_TOP          0
#define TEXT_BASELINE_HANGING      1
#define TEXT_BASELINE_MIDDLE       2
#define TEXT_BASELINE_ALPHABETIC   3
#define TEXT_BASELINE_IDEOGRAPHIC  4
#define TEXT_BASELINE_BOTTOM       5


#define COMPOSITE_CLEAR                     0
#define COMPOSITE_COPY                      1
#define COMPOSITE_SOURCE_OVER               2
#define COMPOSITE_SOURCE_IN                 3
#define COMPOSITE_SOURCE_OUT                4
#define COMPOSITE_SOURCE_ATOP               5
#define COMPOSITE_DESTINATION_OVER          6
#define COMPOSITE_DESTINATION_IN            7
#define COMPOSITE_DESTINATION_OUT           8
#define COMPOSITE_DESTINATION_ATOP          9
#define COMPOSITE_XOR                       10
#define COMPOSITE_DARKER                    11
#define COMPOSITE_LIGHTER                   12
#define COMPOSITE_DIFFERENCE                13



typedef uintptr_t HVIEW;
typedef uintptr_t HCONTEXT;
typedef uintptr_t HCANVASSTYLE;
typedef uintptr_t HCANVASGRADIENT;
typedef uintptr_t HCANVASPATTERN;
typedef uintptr_t HHTMLIMAGEELEMENT;
typedef uintptr_t HIMAGEDATA;
typedef uintptr_t HTIMER;


#ifdef __cplusplus
extern "C" {
#endif


HCONTEXT hiview_canvas_get_context(HVIEW view);
void hiview_canvas_save(HCONTEXT context);
void hiview_canvas_restore(HCONTEXT context);

void hiview_canvas_set_fill_style_by_color(HCONTEXT c, int color);
void hiview_canvas_set_fill_style_by_canvas_gradient(HCONTEXT c,HCANVASGRADIENT gradient);
void hiview_canvas_set_fill_style_by_canvas_pattern(HCONTEXT c, HCANVASPATTERN pattern);
HCANVASSTYLE hiview_canvas_get_fill_style(HCONTEXT c);

void hiview_canvas_set_stroke_style_by_color(HCONTEXT c, int color);
void hiview_canvas_set_stroke_style_by_canvas_gradient(HCONTEXT c,HCANVASGRADIENT gradient);
void hiview_canvas_set_stroke_style_by_canvas_pattern(HCONTEXT c, HCANVASPATTERN pattern);
HCANVASSTYLE hiview_canvas_get_stroke_style(HCONTEXT c);

void hiview_canvas_set_shadow_color(HCONTEXT c, int color);
int hiview_canvas_get_shadow_color(HCONTEXT c);

void hiview_canvas_set_shadow_blur(HCONTEXT c, float blur);
float hiview_canvas_get_shadow_blur(HCONTEXT c);

void hiview_canvas_set_shadow_offset_x(HCONTEXT c, float offset);
float hiview_canvas_get_shadow_offset_x(HCONTEXT c);

void hiview_canvas_set_shadow_offset_y(HCONTEXT c, float offset);
float hiview_canvas_get_shadow_offset_y(HCONTEXT c);

HCANVASGRADIENT hiview_canvas_create_linear_gradient(HCONTEXT c, float x0, float y0, float x1, float y1);
HCANVASGRADIENT hiview_canvas_create_radial_gradient(HCONTEXT c, float x0, float y0, float r0, float x1, float y1, float r1);
void hiview_canvas_add_gradient_color_stop(HCANVASGRADIENT gradient, float value, int color);


int hiview_canvas_get_line_cap(HCONTEXT c);
void hiview_canvas_set_line_cap(HCONTEXT c, int value);

int hiview_canvas_get_line_join(HCONTEXT c);
void hiview_canvas_set_line_join(HCONTEXT c, int value);

float hiview_canvas_get_line_width(HCONTEXT c);
void hiview_canvas_set_line_width(HCONTEXT c, float value);

float hiview_canvas_get_miter_limit(HCONTEXT c);
void hiview_canvas_set_miter_limit(HCONTEXT c, float value);


void hiview_canvas_rect(HCONTEXT c, float x, float y, float w, float h);
void hiview_canvas_fill_rect(HCONTEXT c, float x, float y, float w, float h);
void hiview_canvas_stroke_rect(HCONTEXT c, float x, float y, float w, float h);
void hiview_canvas_clear_rect(HCONTEXT c, float x, float y, float w, float h);


void hiview_canvas_fill(HCONTEXT c);
void hiview_canvas_stroke(HCONTEXT c);
void hiview_canvas_begin_path(HCONTEXT c);
void hiview_canvas_close_path(HCONTEXT c);
void hiview_canvas_clip(HCONTEXT c);


void hiview_canvas_move_to(HCONTEXT c, float x, float y);
void hiview_canvas_line_to(HCONTEXT c, float x, float y);
void hiview_canvas_quadratic_curve_to(HCONTEXT c, float cpx, float cpy, float x, float y);
void hiview_canvas_bezier_curve_to(HCONTEXT c, float cp1x, float cp1y, float cp2x, float cp2y, float x, float y);
void hiview_canvas_arc(HCONTEXT c, float x, float y, float r, float sa, float ea, int anticlockwise);
void hiview_canvas_arc_to(HCONTEXT c, float x0, float y0, float x1, float y1, float radius);
int hiview_canvas_is_point_in_path(HCONTEXT c, float x, float y);


void hiview_canvas_scale(HCONTEXT c, float sx, float sy);
void hiview_canvas_rotate(HCONTEXT c, float angleInRadians);
void hiview_canvas_translate(HCONTEXT c, float tx, float ty);
void hiview_canvas_transform(HCONTEXT c, float m11, float m12, float m21, float m22, float dx, float dy);
void hiview_canvas_set_transform(HCONTEXT c, float m11, float m12, float m21, float m22, float dx, float dy);


int hiview_canvas_get_text_align(HCONTEXT c);
void hiview_canvas_set_text_align(HCONTEXT c, int v);

const char* hiview_canvas_get_font(HCONTEXT c);
void hiview_canvas_set_font(HCONTEXT c, const char* font);

void hiview_canvas_fill_text(HCONTEXT c, const char* text, float x, float y);
void hiview_canvas_stroke_text(HCONTEXT c, const char* text, float x, float y);
float hiview_canvas_measure_text_width(HCONTEXT c, const char* text);


int hiview_canvas_get_image_data_width(HIMAGEDATA imgData);
int hiview_canvas_get_image_data_height(HIMAGEDATA imgData);
void hiview_canvas_set_image_data_content(HIMAGEDATA imgData, int index, int data);
int hiview_canvas_get_image_data_content(HIMAGEDATA imgData, int index);


HIMAGEDATA hiview_canvas_create_image_data(HCONTEXT c, float width, float height);
HIMAGEDATA hiview_canvas_create_image_data_by_image_data(HCONTEXT c, HIMAGEDATA imgData);
HIMAGEDATA hiview_canvas_get_image_data(HCONTEXT c, float sx, float sy, float sw, float sh);
void hiview_canvas_put_image_data(HCONTEXT c, HIMAGEDATA imgData, float dx, float dy);
void hiview_canvas_put_image_data_with_dirty_info(HCONTEXT c, HIMAGEDATA imgData, float dx, float dy, float dirtyX, float dirtyY, float dirtyWidth, float dirtyHeight);


float hiview_canvas_get_global_alpha(HCONTEXT c);
void hiview_canvas_set_global_alpha(HCONTEXT c, float alpha);

int hiview_canvas_get_global_composite_operation(HCONTEXT c);
void hiview_canvas_set_global_composite_operation(HCONTEXT c, int composite);

HTIMER hiview_canvas_start_one_shot_timer(HCONTEXT c, long intervalMilliseconds, void (*func)());
HTIMER hiview_canvas_start_repeating_timer(HCONTEXT c, long intervalMilliseconds, void (*func)());
void hiview_canvas_stop_timer(HCONTEXT c, HTIMER timer);
void hiview_canvas_re_render(HCONTEXT c);
double hview_canvas_get_time_ms(HCONTEXT c);
double hview_canvas_get_local_time_ms(HCONTEXT c);

char* hiview_get_attribute(HVIEW v, const char* name);
void hiview_set_attribute(HVIEW v, const char* name, const char* value);
char* hiview_get_css_property(HVIEW v, const char* name);

void hiview_canvas_send_event(HVIEW v, const char* event, int isCanBubble, int isCancelable);
void hiview_canvas_send_hands_moved_event(HVIEW v, const char* info);

/* Memory for the new string is obtained with malloc, and can be freed with free */
char* hiview_get_param(HVIEW v, const char* name);


void hview_set_extra(HVIEW v, void* extra);
void* hview_get_extra(HVIEW v);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __HIVIEW_CANVAS_H__
