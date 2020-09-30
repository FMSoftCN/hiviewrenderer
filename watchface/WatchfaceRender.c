/* 
 * Copyright (C) 2020 Beijing FMSoft Technologies Co., Ltd.
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 * 
 * Or,
 * 
 * As this component is a program released under LGPLv3, which claims
 * explicitly that the program could be modified by any end user
 * even if the program is conveyed in non-source form on the system it runs.
 * Generally, if you distribute this program in embedded devices,
 * you might not satisfy this condition. Under this situation or you can
 * not accept any condition of LGPLv3, you need to get a commercial license
 * from FMSoft, along with a patent license for the patents owned by FMSoft.
 * 
 * If you have got a commercial/patent license of this program, please use it
 * under the terms and conditions of the commercial license.
 * 
 * For more information about the commercial license and patent license,
 * please refer to
 * <https://hybridos.fmsoft.cn/blog/hybridos-licensing-policy/>.
 * 
 * Also note that the LGPLv3 license does not apply to any entity in the
 * Exception List published by Beijing FMSoft Technologies Co., Ltd.
 * 
 * If you are or the entity you represent is listed in the Exception List,
 * the above open source or free software license does not apply to you
 * or the entity you represent. Regardless of the purpose, you should not
 * use the software in any way whatsoever, including but not limited to
 * downloading, viewing, copying, distributing, compiling, and running.
 * If you have already downloaded it, you MUST destroy all of its copies.
 * 
 * The Exception List is published by FMSoft and may be updated
 * from time to time. For more information, please see
 * <https://www.fmsoft.cn/exception-list>.
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
    int watchface_mode;

    int auto_watch;

    int time_h;
    int time_m;
    int time_s;
    int time_ms;

    int fire_hour_event;
    int fire_minute_event;
    int fire_second_event;
    int fire_hands_moved_event;

    int need_re_render;

    int has_hand_h;
    int has_hand_m;
    int has_hand_s;
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

    wf->watchface_mode = ACTIVE_MODE;
    wf->auto_watch = 1;
    wf->time_h = 0;
    wf->time_m = 0;
    wf->time_s = 0;
    wf->time_ms = 0;
    wf->fire_hour_event = 0;
    wf->fire_minute_event = 0;
    wf->fire_second_event = 0;
    wf->fire_hands_moved_event = 0;
    wf->need_re_render = 1;
    wf->has_hand_h = 1;
    wf->has_hand_m = 1;
    wf->has_hand_s = 1;

    hiview_set_extra(v, wf);
    return wf;
}

Watchface* get_watchface(HVIEW v)
{
    return hiview_get_extra(v);
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
        Watchface* wf = get_watchface(v);
        wf->time_h = h;
        wf->time_m = m;
        wf->time_s = s;
    }
    free(data_time);
}

void update_hands_info_by_param(HVIEW v)
{
    char* param_hands = hiview_get_param(v, "hands");
    if (!param_hands)
        return;

    Watchface* wf = get_watchface(v);
    wf->has_hand_h = 0;
    wf->has_hand_m = 0;
    wf->has_hand_s = 0;

    char* token = strtok(param_hands, ",");
    while( token != NULL )
    {
        if (strcasecmp(strtrimall(token), "hour") == 0)
        {
            wf->has_hand_h = 1;
        }
        else if (strcasecmp(strtrimall(token), "minute") == 0)
        {
            wf->has_hand_m = 1;
        }
        else if (strcasecmp(strtrimall(token), "second") == 0)
        {
            wf->has_hand_s = 1;
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
}

void on_attribute_change(HVIEW v, HCONTEXT c, const char* name, const char* value)
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

    wf->auto_watch = get_data_move_attribute(v);
    if (wf->auto_watch)
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

        if (hour != wf->time_h)
        {
            wf->time_h = hour;
            wf->fire_hour_event = 1;
            need_re_render = 1;
        }

        if (minute != wf->time_m)
        {
            wf->time_m = minute;
            wf->fire_minute_event = 1;
            need_re_render = 1;
        }

        if (second != wf->time_s)
        {
            wf->time_s = second;
            wf->fire_second_event = 1;
            need_re_render = 1;
        }

        if (ms != wf->time_ms)
        {
            wf->time_ms = ms;
            need_re_render = 1;
        }
    }

    return need_re_render || wf->need_re_render;
}

void render(HVIEW v, HCONTEXT c, float x, float y, float width, float height)
{
    Watchface* wf = get_watchface(v);
    wf->need_re_render = 0;

    const char* aa = hiview_get_css_property(v, "-hi-max");
    float minL = fmin(width, height);
    float cx = minL / 2; 
    float cy = minL / 2;
    float cr = minL / 2;


    float h = wf->time_h;
    float m = wf->time_m;
    float s = wf->time_s;
    float ms = wf->time_ms;



    if (wf->has_hand_m)
        paintMinuteHand(c, cx, cy, cr * 0.7f, M_PI * 2 * (m / 60 + (s / 60) * (1.0f / 60)));

    if (wf->has_hand_h)
        paintHourHand(c, cx, cy, cr * 0.7f, M_PI * 2 * (h / 12 + (m / 60) * (1.0f / 12)));

    if (wf->has_hand_s)
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

    sprintf(buf, "%02d:%02d:%02d", wf->time_h, wf->time_m, wf->time_s);
    hiview_set_attribute(view, "data-time", buf);
}


