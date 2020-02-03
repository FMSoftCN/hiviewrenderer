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

typedef struct watchface_data
{
    int watchface_mode;

    double time_start;

    int time_h;
    int time_m;
    int time_s;
    int time_ms;

    int need_re_render;

    int displayMinute;
    int displayHour;
    int displaySecond;

    float minuteScale;
    float hourScale;
    float secondScale;
} Watchface;

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


Watchface* init_watchface(HVIEW v, HCONTEXT c)
{
    Watchface* wf = (Watchface*)malloc(sizeof(Watchface));

    wf->watchface_mode = ACTIVE_MODE;

    wf->time_start = hview_canvas_get_local_time_ms(c);
    wf->time_h = 0;
    wf->time_m = 0;
    wf->time_s = 0;
    wf->time_ms = 0;
    wf->need_re_render = 1;

    wf->displayMinute = 1;
    wf->displayHour = 1;
    wf->displaySecond = 1;

    wf->minuteScale = 60;
    wf->hourScale = 12;
    wf->secondScale = 60;

    hview_set_extra(v, wf);
    return wf;
}

Watchface* get_watchface(HVIEW v)
{
    return hview_get_extra(v);
}

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
        Watchface* wf = get_watchface(v);
        wf->time_h = h;
        wf->time_m = m;
        wf->time_s = s;
    }
}

void update_hands_info_by_param(HVIEW v)
{
}


void initialize(HVIEW v, HCONTEXT c)
{
    init_watchface(v, c);
    init_time_by_data_time_attribute(v);
    update_hands_info_by_param(v);
}

int create(HVIEW view, HCONTEXT context, int* activeModeIntervalMs)
{
    *activeModeIntervalMs = 10;

    Watchface* wf = get_watchface(view);
    return wf->watchface_mode;
}

void destroy(HVIEW view, HCONTEXT context)
{
    Watchface* wf = get_watchface(view);
    free(wf);
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
    Watchface* wf = get_watchface(v);

    double ct = hview_canvas_get_local_time_ms(c);
    double diff = ct - wf->time_start;

    if (diff <= 0)
        return 0;

    wf->time_h = floor(diff/ 3600000);
    wf->time_m = fmod(floor(diff / 60000), 60);
    wf->time_s = fmod(floor(diff / 1000 ), 60);
    wf->time_ms = fmod(diff, 1000);

    return need_re_render | wf->need_re_render;
}

void render(HVIEW v, HCONTEXT c, float x, float y, float width, float height)
{
    Watchface* wf = get_watchface(v);
    wf->need_re_render = 0;

    const char* aa = hiview_get_css_property(v, "-hi-max");
    float minL = fmin(width, height);
    float cx = minL / 2; 
    float cy = minL / 2;
    float r = minL / 2;

    float h = wf->time_h;
    float m = wf->time_m;
    float s = wf->time_s;
    float ms = wf->time_ms;

    if (wf->displayMinute)
    {
        float mcx = cx;
        float mcy = cy - r * 0.7f * 0.9f * 0.50f;
        paintStopWatchDial(c, mcx, mcy, r * 0.75f * 0.25, r * 0.006f, wf->minuteScale);
        paintStopWatchHand(c, mcx, mcy, r * 0.75f * 0.25, r * 0.006f, M_PI * 2 * ((float)m / wf->minuteScale + ((float) s / 60) * (1.0f / wf->minuteScale)), 0xFFFFFF00);
    }

    if (wf->displayHour)
    {
        float hcx = cx;
        float hcy = cy + r * 0.7f * 0.9f * 0.50f;
        paintStopWatchDial(c, hcx, hcy, r * 0.75f * 0.25, r * 0.006f, wf->hourScale);
        paintStopWatchHand(c, hcx, hcy, r * 0.75f * 0.25, r * 0.006f, M_PI * 2 * ((float)h / wf->hourScale + ((float)m / 60) * (1.0f / wf->hourScale)), 0xFFFFFF00);
    }

    if (wf->displaySecond)
    {
        paintStopWatchDial(c, cx, cy, r * 0.75f, r * 0.01f, wf->secondScale);
        paintStopWatchHand(c, cx, cy, r * 0.75f, r * 0.01f, M_PI * 2 * ((float)s / wf->secondScale + ((float)ms / 1000) * (1.0f / wf->secondScale)), 0xFFFFFF00);
    }
}

void post_render(HVIEW view, HCONTEXT context)
{
}


