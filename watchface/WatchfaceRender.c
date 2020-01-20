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
#include <string.h>
#include <strings.h>
#include <math.h>
#include "HiViewCanvas.h"
#include "WatchfaceRender.h"

#define TRUE           1
#define FALSE          0
#define M_PI           3.14159265358979323846

int g_watchface_mode = ACTIVE_MODE;
HVIEW g_watchface_view = INVALID_HANDLE;
HCONTEXT g_watchface_context = INVALID_HANDLE; 

int g_auto_watch = 1;

int g_time_h = 0;
int g_time_m = 0;
int g_time_s = 0;
int g_time_ms = 0;

int fire_hour_event = 0;
int fire_minute_event = 0;
int fire_second_event = 0;
int fire_hands_moved_event = 0;

int g_need_re_render = 1;


int get_data_move_attribute(HVIEW v)
{
    const char* move = hiview_get_attribute(v, "data-move");
    if (!move)
    {
        return 1;
    }

    if (strcasecmp(move, "static") == 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

void init_time_by_data_time_attribute(HVIEW v)
{
    const char* data_time = hiview_get_attribute(v, "data-time");
    if (!data_time)
        return;
    int h = 0;
    int m = 0;
    int s = 0;
    int ret = sscanf(data_time, "%d:%d:%d", &h, &m, &s);
    if (ret != EOF)
    {
        g_time_h = h;
        g_time_m = m;
        g_time_s = s;
    }
}

void initialize(HVIEW v, HCONTEXT c)
{
    init_time_by_data_time_attribute(v);
}

int create(HVIEW view, HCONTEXT context, int* activeModeIntervalMs)
{
    *activeModeIntervalMs = 10;
    g_watchface_view = view;
    g_watchface_context = context;
    return g_watchface_mode;
}

void destroy()
{
}

void terminate()
{
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

int pre_render(HVIEW v, HCONTEXT c)
{
    int need_re_render = 0;
    fire_hour_event = 0;
    fire_minute_event = 0;
    fire_second_event = 0;
    g_auto_watch = get_data_move_attribute(v);
    if (g_auto_watch)
    {
        double ct = hview_canvas_get_local_time_ms(c);

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

        int ms = fmod(ct, 1000);

        if (hour != g_time_h)
        {
            g_time_h = hour;
            fire_hour_event = 1;
            need_re_render = 1;
        }

        if (minute != g_time_m)
        {
            g_time_m = minute;
            fire_minute_event = 1;
            need_re_render = 1;
        }

        if (second != g_time_s)
        {
            g_time_s = second;
            fire_second_event = 1;
            need_re_render = 1;
        }

        if (ms != g_time_ms)
        {
            g_time_ms = ms;
            need_re_render = 1;
        }
    }

    return need_re_render || g_need_re_render;
}

void render(HCONTEXT c, float x, float y, float width, float height)
{
    g_need_re_render = 0;
    const char* aa = hiview_get_css_property(g_watchface_view, "-hi-max");
    float minL = fmin(width, height);
    float cx = minL / 2; 
    float cy = minL / 2;
    float cr = minL / 2;


    float h = g_time_h;
    float m = g_time_m;
    float s = g_time_s;
    float ms = g_time_ms;

    paintMinuteHand(c, cx, cy, cr * 0.7f, M_PI * 2 * (m / 60 + (s / 60) * (1.0f / 60)));
    paintHourHand(c, cx, cy, cr * 0.7f, M_PI * 2 * (h / 12 + (m / 60) * (1.0f / 12)));
    paintSecondHand(c, cx, cy, cr * 0.7f, M_PI * 2 * (s / 60 + (ms / 1000) * (1.0f / 60)));
}

void post_render(HVIEW view, HCONTEXT context)
{
    char buf[20] = {0};
    if (fire_hour_event)
    {
        hiview_canvas_send_event(view, "hourmoved", 0, 0);
        strcpy(buf, "hour");
    }

    if (fire_minute_event)
    {
        hiview_canvas_send_event(view, "minutemoved", 0, 0);
        if (strlen(buf))
        {
            strcat(buf, ",");
            strcat(buf, "minute");
        }
        else
        {
            strcpy(buf, "minute");
        }
    }

    if (fire_second_event)
    {
        hiview_canvas_send_event(view, "secondmoved", 0, 0);
        if (strlen(buf))
        {
            strcat(buf, ",");
            strcat(buf, "second");
        }
        else
        {
            strcpy(buf, "second");
        }
    }

    if (strlen(buf))
        hiview_canvas_send_hands_moved_event(view, buf);

    sprintf(buf, "%02d:%02d:%02d", g_time_h, g_time_m, g_time_s);
    hiview_set_attribute(view, "data-time", buf);
}


