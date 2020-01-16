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

#include <stdio.h>
#include <math.h>
#include "HiViewCanvas.h"
#include "WatchfaceRender.h"

#define TRUE           1
#define FALSE          0
#define M_PI           3.14159265358979323846

int g_watchface_mode = ACTIVE_MODE;

void initialize()
{
    printf("%s:%d:%s\n", __FILE__, __LINE__, __func__);
}

int create(HVIEW view, HCONTEXT context, int* activeModeIntervalMs)
{
    printf("%s:%d:%s\n", __FILE__, __LINE__, __func__);
    *activeModeIntervalMs = 10;
    return g_watchface_mode;
}

void destroy()
{
    printf("%s:%d:%s\n", __FILE__, __LINE__, __func__);
}

void terminate()
{
    printf("%s:%d:%s\n", __FILE__, __LINE__, __func__);
}

void paintHourHand(HCONTEXT c, float cx, float cy, float r, float angleInRadians)
{
    float x = 0;
    float y = 0;

    hiview_canvas_save(c);
    hiview_canvas_translate(c, cx, cy);
    hiview_canvas_rotate(c, angleInRadians);

    hiview_canvas_begin_path(c);
	hiview_canvas_arc(c, x, y, r * 0.02f, 0, 360 * M_PI/180, FALSE);
    hiview_canvas_rect(c, x - r * 0.01f, y - r * 0.13f, r * 0.02f, r * 0.12f);
	hiview_canvas_arc(c, x, y - r * 0.14f, r * 0.02f, 0, 360 * M_PI/180, FALSE);
    hiview_canvas_rect(c, x - r * 0.02f, y - r * 0.45f, r * 0.04f, r * 0.31f);
	hiview_canvas_arc(c, x, y - r * 0.45f, r * 0.02f, 0, 360 * M_PI/180, FALSE);

    hiview_canvas_close_path(c);
    hiview_canvas_set_fill_style_by_color(c, 0xFFFFFFFF);
    hiview_canvas_fill(c);

    hiview_canvas_begin_path(c);
	hiview_canvas_arc(c, x, y - r * 0.14f, r * 0.01f, 0, 360 * M_PI/180, FALSE);
    hiview_canvas_rect(c, x - r * 0.01f, y - r * 0.45f, r * 0.02f, r * 0.31f);
	hiview_canvas_arc(c, x, y - r * 0.45f, r * 0.01f, 0, 360 * M_PI/180, FALSE);
    hiview_canvas_set_fill_style_by_color(c, 0xFF000000);
    hiview_canvas_fill(c);

    hiview_canvas_restore(c);
}

void paintMinuteHand(HCONTEXT c, float cx, float cy, float r, float angleInRadians)
{
    float x = 0;
    float y = 0;

    hiview_canvas_save(c);
    hiview_canvas_translate(c, cx, cy);
    hiview_canvas_rotate(c, angleInRadians);

    hiview_canvas_begin_path(c);

	hiview_canvas_arc(c, x, y, r * 0.02f, 0, 360 * M_PI/180, FALSE);
    hiview_canvas_rect(c, x - r * 0.01f, y - r * 0.13f, r * 0.02f, r * 0.12f);
	hiview_canvas_arc(c, x, y - r * 0.14f, r * 0.02f, 0, 360 * M_PI/180, FALSE);
    hiview_canvas_rect(c, x - r * 0.02f, y - r * 0.82f, r * 0.04f, r * 0.68f);
	hiview_canvas_arc(c, x, y - r * 0.82f, r * 0.02f, 0, 360 * M_PI/180, FALSE);

    hiview_canvas_set_fill_style_by_color(c, 0xFFFFFFFF);
    hiview_canvas_close_path(c);
    hiview_canvas_fill(c);

    hiview_canvas_begin_path(c);
	hiview_canvas_arc(c, x, y - r * 0.14f, r * 0.01f, 0, 360 * M_PI/180, FALSE);
    hiview_canvas_rect(c, x - r * 0.01f, y - r * 0.82f, r * 0.02f, r * 0.68f);
	hiview_canvas_arc(c, x, y - r * 0.82f, r * 0.01f, 0, 360 * M_PI/180, FALSE);
    hiview_canvas_set_fill_style_by_color(c, 0xFF000000);
    hiview_canvas_close_path(c);
    hiview_canvas_fill(c);

    hiview_canvas_restore(c);
}

void paintSecondHand(HCONTEXT c, float cx, float cy, float r, float angleInRadians)
{
    float x = 0;
    float y = 0;

    hiview_canvas_save(c);
    hiview_canvas_translate(c, cx, cy);
    hiview_canvas_rotate(c, angleInRadians);

    hiview_canvas_begin_path(c);
    hiview_canvas_rect(c, x - r * 0.005f, y - r * 0.90f, r * 0.01f, r * 1.09f);
	hiview_canvas_arc(c, x, y, r * 0.016f, 0, 360 * M_PI/180, FALSE);
    hiview_canvas_close_path(c);
    hiview_canvas_set_fill_style_by_color(c, 0xFFFF0000);
    hiview_canvas_fill(c);

    hiview_canvas_begin_path(c);
	hiview_canvas_arc(c, x, y, r * 0.01f, 0, 360 * M_PI/180, FALSE);
    hiview_canvas_close_path(c);
    hiview_canvas_set_fill_style_by_color(c, 0xFF000000);
    hiview_canvas_fill(c);

    hiview_canvas_restore(c);
}

void render(HCONTEXT c, float x, float y, float width, float height)
{
    double ct = hview_canvas_get_local_time_ms(c);

    float minL = fmin(width, height);
    float cx = minL / 2; 
    float cy = minL / 2;
    float cr = minL / 2;

    int hour = fmod(floor(ct/ 3600000),24);
    if(hour < 0)
    {
        hour  += 24;
    }

    int minute = fmod(floor(ct / 60000), 60);
    if(minute < 0)
    {
        minute  += 60;
    }

    int second = fmod(floor(ct / 1000 ), 60);
    if(second < 0)
    {
        second += 60;
    }

    float h = hour;
    float m = minute;
    float s = second;
    float ms = fmod(ct, 1000);

    paintMinuteHand(c, cx, cy, cr * 0.7f, M_PI * 2 * (m / 60 + (s / 60) * (1.0f / 60)));
    paintHourHand(c, cx, cy, cr * 0.7f, M_PI * 2 * (h / 12 + (m / 60) * (1.0f / 12)));
    paintSecondHand(c, cx, cy, cr * 0.7f, M_PI * 2 * (s / 60 + (ms / 1000) * (1.0f / 60)));
}

