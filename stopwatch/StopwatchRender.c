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
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <math.h>
#include <ctype.h>
#include "HiViewCanvas.h"
#include "StopwatchRender.h"

#define TRUE           1
#define FALSE          0
#define M_PI           3.14159265358979323846

enum stopwatch_action
{
    NEW,
    START,
    PAUSE,
    STOP,
    RESET
};

typedef struct watchface_data
{
    int watchface_mode;

    double time_start;
    double time_lapse;

    int time_h;
    int time_m;
    int time_s;
    int time_ms;

    int need_re_render;

    int has_hand_m;
    int has_hand_h;
    int has_hand_s;

    float minuteScale;
    float hourScale;
    float secondScale;
} Stopwatch;

char * strtrimall( char *src)
{
    int  nIndex1;
    int  nLen;

    if (src == NULL)
        return NULL;

    if (src [0] == '\0')
        return src;

    nLen = strlen (src);

    nIndex1 = 0;
    while (isspace ((int)src[nIndex1]))
        nIndex1 ++;

    if (nIndex1 == nLen) {
        *src = '\0';
        return src;
    }

    strcpy (src, src + nIndex1);

    nLen = strlen (src);
    nIndex1 = nLen - 1;
    while (isspace ((int)src[nIndex1]))
        nIndex1 --;

    src [nIndex1 + 1] = '\0';

    return src;
}


Stopwatch* init_struct(HVIEW v, HCONTEXT c)
{
    Stopwatch* sw = (Stopwatch*)malloc(sizeof(Stopwatch));

    sw->watchface_mode = ACTIVE_MODE;

    sw->time_start = hview_canvas_get_local_time_ms(c);
    sw->time_h = 0;
    sw->time_m = 0;
    sw->time_s = 0;
    sw->time_ms = 0;
    sw->need_re_render = 1;

    sw->has_hand_m = 1;
    sw->has_hand_h = 1;
    sw->has_hand_s = 1;

    sw->minuteScale = 60;
    sw->hourScale = 12;
    sw->secondScale = 60;

    hview_set_extra(v, sw);
    return sw;
}

Stopwatch* get_stopwatch(HVIEW v)
{
    return hview_get_extra(v);
}

int get_data_move_attribute(HVIEW v)
{
    char* move = hiview_get_attribute(v, "data-move");
    if (!move)
    {
        return 1;
    }

    int ret = strcasecmp(move, "static") == 0  ? 0 : 1;
    free(move);
    return ret;
}

void init_time_by_data_time_attribute(HVIEW v)
{
    char* data_time = hiview_get_attribute(v, "data-time");
    if (!data_time)
        return;

    int h = 0;
    int m = 0;
    int s = 0;
    int ret = sscanf(data_time, "%d:%d:%d", &h, &m, &s);
    if (ret != EOF)
    {
        Stopwatch* sw = get_stopwatch(v);
        sw->time_h = h;
        sw->time_m = m;
        sw->time_s = s;
    }

    free(data_time);
}

void update_hands_info_by_param(HVIEW v)
{
    char* param_hands = hiview_get_param(v, "faces");
    if (!param_hands)
        return;

    Stopwatch* sw = get_stopwatch(v);
    sw->has_hand_h = 0;
    sw->has_hand_m = 0;
    sw->has_hand_s = 0;

    char* token = strtok(param_hands, ",");
    while( token != NULL )
    {
        printf("token=%s\n", token);
        if (strcasecmp(strtrimall(token), "hour") == 0)
        {
            sw->has_hand_h = 1;
        }
        else if (strcasecmp(strtrimall(token), "minute") == 0)
        {
            sw->has_hand_m = 1;
        }
        else if (strcasecmp(strtrimall(token), "second") == 0)
        {
            sw->has_hand_s = 1;
        }

        token = strtok(NULL, ",");
    }
    free(param_hands);
}


void initialize(HVIEW v, HCONTEXT c)
{
    init_struct(v, c);
    init_time_by_data_time_attribute(v);
    update_hands_info_by_param(v);
}

int create(HVIEW view, HCONTEXT context, int* activeModeIntervalMs)
{
    *activeModeIntervalMs = 10;

    Stopwatch* sw = get_stopwatch(view);
    return sw->watchface_mode;
}

void destroy(HVIEW view, HCONTEXT context)
{
    Stopwatch* sw = get_stopwatch(view);
    free(sw);
}

void terminate(HVIEW view, HCONTEXT context)
{
}

void on_param_change(HVIEW v, HCONTEXT c, const char* name, const char* value)
{
    printf("########################## StopwatchRender  on_param_change name=%s|value=%s\n", name, value);
}

void paintStopWatchDial(HCONTEXT c, float cx, float cy, float length, float width, double scale)
{
    float x = 0;
    float y = 0;

    int circle = (float)scale;
    for (int i = 0; i < circle; i++)
    {
        float angleInRadians = M_PI * 2 * ((float)i / circle);

        hiview_canvas_save(c);
        hiview_canvas_translate(c, cx, cy);
        hiview_canvas_rotate(c, angleInRadians);

        hiview_canvas_begin_path(c);
        hiview_canvas_rect(c, x - width * 0.5f, y - length * 0.82f, width, length * 0.05f);
        hiview_canvas_close_path(c);
        hiview_canvas_set_fill_style_by_color(c, 0xFFFFFFFF);
        hiview_canvas_fill(c);

        hiview_canvas_restore(c);
    }
}

void paintStopWatchHand(HCONTEXT c, float cx, float cy, float length, float width, float angleInRadians, int color)
{
    float x = 0;
    float y = 0;

    hiview_canvas_save(c);
    hiview_canvas_translate(c, cx, cy);
    hiview_canvas_rotate(c, angleInRadians);

    hiview_canvas_begin_path(c);
    hiview_canvas_rect(c, x - width * 0.5f, y - length * 0.84f, width, length);
    hiview_canvas_arc(c, x, y, width * 1.6f, 0, 360 * M_PI/180, FALSE);
    hiview_canvas_close_path(c);
    hiview_canvas_set_fill_style_by_color(c, color);
    hiview_canvas_fill(c);

    hiview_canvas_begin_path(c);
    hiview_canvas_arc(c, x, y, width, 0, 360 * M_PI/180, FALSE);
    hiview_canvas_close_path(c);
    hiview_canvas_set_fill_style_by_color(c, 0xFF000000);
    hiview_canvas_fill(c);

    hiview_canvas_restore(c);
}


int pre_render(HVIEW v, HCONTEXT c)
{
    int need_re_render = 1;
    Stopwatch* sw = get_stopwatch(v);

    double ct = hview_canvas_get_local_time_ms(c);
    double diff = ct - sw->time_start;

    if (diff <= 0)
        return 0;

    sw->time_h = floor(diff/ 3600000);
    sw->time_m = fmod(floor(diff / 60000), 60);
    sw->time_s = fmod(floor(diff / 1000 ), 60);
    sw->time_ms = fmod(diff, 1000);

    return need_re_render | sw->need_re_render;
}

void render(HVIEW v, HCONTEXT c, float x, float y, float width, float height)
{
    Stopwatch* sw = get_stopwatch(v);
    sw->need_re_render = 0;

    const char* aa = hiview_get_css_property(v, "-hi-max");
    float minL = fmin(width, height);
    float cx = minL / 2; 
    float cy = minL / 2;
    float r = minL / 2;

    float h = sw->time_h;
    float m = sw->time_m;
    float s = sw->time_s;
    float ms = sw->time_ms;

    if (sw->has_hand_m)
    {
        float mcx = cx;
        float mcy = cy - r * 0.7f * 0.9f * 0.50f;
        paintStopWatchDial(c, mcx, mcy, r * 0.75f * 0.25, r * 0.006f, sw->minuteScale);
        paintStopWatchHand(c, mcx, mcy, r * 0.75f * 0.25, r * 0.006f, M_PI * 2 * ((float)m / sw->minuteScale + ((float) s / 60) * (1.0f / sw->minuteScale)), 0xFFFFFF00);
    }

    if (sw->has_hand_h)
    {
        float hcx = cx;
        float hcy = cy + r * 0.7f * 0.9f * 0.50f;
        paintStopWatchDial(c, hcx, hcy, r * 0.75f * 0.25, r * 0.006f, sw->hourScale);
        paintStopWatchHand(c, hcx, hcy, r * 0.75f * 0.25, r * 0.006f, M_PI * 2 * ((float)h / sw->hourScale + ((float)m / 60) * (1.0f / sw->hourScale)), 0xFFFFFF00);
    }

    if (sw->has_hand_s)
    {
        paintStopWatchDial(c, cx, cy, r * 0.75f, r * 0.01f, sw->secondScale);
        paintStopWatchHand(c, cx, cy, r * 0.75f, r * 0.01f, M_PI * 2 * ((float)s / sw->secondScale + ((float)ms / 1000) * (1.0f / sw->secondScale)), 0xFFFFFF00);
    }
}

void post_render(HVIEW view, HCONTEXT context)
{
}


