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
#include "WatchfaceRender.h"

#define TRUE           1
#define FALSE          0
#define M_PI           3.14159265358979323846

typedef struct watchface_data
{
    int g_watchface_mode;
    HVIEW g_watchface_view;
    HCONTEXT g_watchface_context; 

    int g_auto_watch;

    int g_time_h;
    int g_time_m;
    int g_time_s;
    int g_time_ms;

    int fire_hour_event;
    int fire_minute_event;
    int fire_second_event;
    int fire_hands_moved_event;

    int g_need_re_render;

    int g_has_hand_h;
    int g_has_hand_m;
    int g_has_hand_s;
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


Watchface* init_watchface(HVIEW v)
{
    Watchface* wf = (Watchface*)malloc(sizeof(Watchface));

    wf->g_watchface_mode = ACTIVE_MODE;
    wf->g_watchface_view = INVALID_HANDLE;
    wf->g_watchface_context = INVALID_HANDLE; 
    wf->g_auto_watch = 1;
    wf->g_time_h = 0;
    wf->g_time_m = 0;
    wf->g_time_s = 0;
    wf->g_time_ms = 0;
    wf->fire_hour_event = 0;
    wf->fire_minute_event = 0;
    wf->fire_second_event = 0;
    wf->fire_hands_moved_event = 0;
    wf->g_need_re_render = 1;
    wf->g_has_hand_h = 1;
    wf->g_has_hand_m = 1;
    wf->g_has_hand_s = 1;

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
        wf->g_time_h = h;
        wf->g_time_m = m;
        wf->g_time_s = s;
    }
}

void update_hands_info_by_param(HVIEW v)
{
    char* param_hands = hiview_get_param(v, "hands");
    if (!param_hands)
        return;

    Watchface* wf = get_watchface(v);
    wf->g_has_hand_h = 0;
    wf->g_has_hand_m = 0;
    wf->g_has_hand_s = 0;

    char* token = strtok(param_hands, ",");
    while( token != NULL )
    {
        if (strcasecmp(strtrimall(token), "hour") == 0)
        {
            wf->g_has_hand_h = 1;
        }
        else if (strcasecmp(strtrimall(token), "minute") == 0)
        {
            wf->g_has_hand_m = 1;
        }
        else if (strcasecmp(strtrimall(token), "second") == 0)
        {
            wf->g_has_hand_s = 1;
        }

        token = strtok(NULL, ",");
    }
    free(param_hands);
}


void initialize(HVIEW v, HCONTEXT c)
{
    init_watchface(v);
    init_time_by_data_time_attribute(v);
    update_hands_info_by_param(v);
}

int create(HVIEW view, HCONTEXT context, int* activeModeIntervalMs)
{
    *activeModeIntervalMs = 10;

    Watchface* wf = get_watchface(view);
    wf->g_watchface_view = view;
    wf->g_watchface_context = context;
    return wf->g_watchface_mode;
}

void destroy(HVIEW view, HCONTEXT context)
{
    Watchface* wf = get_watchface(view);
    free(wf);
}

void terminate(HVIEW view, HCONTEXT context)
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
    Watchface* wf = get_watchface(v);
    wf->fire_hour_event = 0;
    wf->fire_minute_event = 0;
    wf->fire_second_event = 0;

    wf->g_auto_watch = get_data_move_attribute(v);
    if (wf->g_auto_watch)
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

        if (hour != wf->g_time_h)
        {
            wf->g_time_h = hour;
            wf->fire_hour_event = 1;
            need_re_render = 1;
        }

        if (minute != wf->g_time_m)
        {
            wf->g_time_m = minute;
            wf->fire_minute_event = 1;
            need_re_render = 1;
        }

        if (second != wf->g_time_s)
        {
            wf->g_time_s = second;
            wf->fire_second_event = 1;
            need_re_render = 1;
        }

        if (ms != wf->g_time_ms)
        {
            wf->g_time_ms = ms;
            need_re_render = 1;
        }
    }

    return need_re_render || wf->g_need_re_render;
}

void render(HVIEW v, HCONTEXT c, float x, float y, float width, float height)
{
    Watchface* wf = get_watchface(v);
    wf->g_need_re_render = 0;

    const char* aa = hiview_get_css_property(v, "-hi-max");
    float minL = fmin(width, height);
    float cx = minL / 2; 
    float cy = minL / 2;
    float cr = minL / 2;


    float h = wf->g_time_h;
    float m = wf->g_time_m;
    float s = wf->g_time_s;
    float ms = wf->g_time_ms;



    if (wf->g_has_hand_m)
        paintMinuteHand(c, cx, cy, cr * 0.7f, M_PI * 2 * (m / 60 + (s / 60) * (1.0f / 60)));

    if (wf->g_has_hand_h)
        paintHourHand(c, cx, cy, cr * 0.7f, M_PI * 2 * (h / 12 + (m / 60) * (1.0f / 12)));

    if (wf->g_has_hand_s)
        paintSecondHand(c, cx, cy, cr * 0.7f, M_PI * 2 * (s / 60 + (ms / 1000) * (1.0f / 60)));
}

void post_render(HVIEW view, HCONTEXT context)
{
    char buf[20] = {0};
    Watchface* wf = get_watchface(view);

    if (wf->fire_hour_event)
    {
        hiview_canvas_send_event(view, "hourmoved", 0, 0);
        strcpy(buf, "hour");
    }

    if (wf->fire_minute_event)
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

    if (wf->fire_second_event)
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

    sprintf(buf, "%02d:%02d:%02d", wf->g_time_h, wf->g_time_m, wf->g_time_s);
    hiview_set_attribute(view, "data-time", buf);
}


