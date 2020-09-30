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
#include "AnimationRender.h"

#define TRUE           1
#define FALSE          0

typedef struct animation_data
{
    int animation_mode;
    int need_re_render;

    float data_interval;
    int data_loop;

    HPARAM* params;
    int paramSize;

    int currentIndex;
    int currentPosX;
    int currentPosY;
    float currentScale;
    float currentDelay;

    double timeAnimationEnd;

} Animation;

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


Animation* init_animation(HVIEW v)
{
    Animation* an = (Animation*)malloc(sizeof(Animation));

    an->animation_mode = ACTIVE_MODE;

    an->need_re_render = 1;

    an->data_loop = 1;
    an->data_interval = 0.0;

    an->params = NULL;
    an->paramSize = 0;

    an->currentIndex = -1;
    an->currentPosX = 0;
    an->currentPosY = 0;
    an->currentScale = 1.0;
    an->currentDelay = 0.0;
    an->timeAnimationEnd = 0.0;

    hiview_set_extra(v, an);
    return an;
}

Animation* get_animation(HVIEW v)
{
    return hiview_get_extra(v);
}

void init_params(HVIEW v)
{
    int count = 0;
    Animation* an = get_animation(v);
    an->params = hiview_get_all_param_object(v, &count);
    an->paramSize = count;
}

void init_attribute(HVIEW v)
{
    Animation* an = get_animation(v);
    char* buf = hiview_get_attribute(v, "data-interval");
    if (buf)
    {
        float value = 0;
        int ret = sscanf(buf, "%f", &value);
        if (ret != EOF)
        {
            an->data_interval = value;
        }
        free(buf);
    }

    buf = hiview_get_attribute(v, "data-loop");
    if (buf)
    {
        int value = 0;
        int ret = sscanf(buf, "%d", &value);
        if (ret != EOF)
        {
            an->data_loop = value;
        }
        free(buf);
    }

}

void update_current_param(HVIEW v, HCONTEXT c, int index)
{
    Animation* an = get_animation(v);
    if (index == an->currentIndex || index > an->paramSize)
        return;

    HPARAM p = an->params[index];
    an->currentIndex = index;
    char* value = hview_get_param_value(p);
    if (!value)
        return;

    int i = 0;
    char* token = strtok(value, " ");
    while( token != NULL )
    {
        int v = 0;
        if (i < 2)
        {
            int ret = sscanf(token, "%d", &v);
            if (ret != EOF)
            {
                if (i == 0)
                    an->currentPosX = v;
                else if (i == 1)
                    an->currentPosY = v;
            }
        }
        else
        {
            float fv = 0.0;
            int ret = EOF;
            if (strstr(token, "x"))
            {
                ret = sscanf(token, "%f", &fv);
                if (ret != EOF)
                {
                    an->currentScale = fv;
                }
            }
            else if (strstr(token, "s"))
            {
                ret = sscanf(token, "%f", &fv);
                if (ret != EOF)
                {
                    an->currentDelay = fv;
                }
            }
        }

        i++;
        token = strtok(NULL, " ");
    }

    if (an->currentDelay <= 0)
    {
        an->currentDelay = an->data_interval;
    }
    an->timeAnimationEnd = hview_canvas_get_local_time_ms(c) + an->currentDelay * 1000;

    free(value);
}


void initialize(HVIEW v, HCONTEXT c)
{
    init_animation(v);
    init_attribute(v);
    init_params(v);
}

int create(HVIEW view, HCONTEXT context, int* activeModeIntervalMs)
{
    *activeModeIntervalMs = 10;

    Animation* an = get_animation(view);
    return an->animation_mode;
}

void destroy(HVIEW view, HCONTEXT context)
{
    Animation* an = get_animation(view);
    if (an->params)
        free(an->params);
    free(an);
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

int pre_render(HVIEW v, HCONTEXT c)
{
    int need_re_render = 0;

    Animation* an = get_animation(v);

    double now = hview_canvas_get_local_time_ms(c); 
    if (hview_canvas_get_local_time_ms(c) > an->timeAnimationEnd)
    {
        int index = (an->currentIndex + 1 >= an->paramSize) ? 0 : (an->currentIndex  + 1);
        need_re_render = index != an->currentIndex ? 1 : 0;
        update_current_param(v, c, index);
    }

    return need_re_render || an->need_re_render;
}

void render(HVIEW v, HCONTEXT c, float x, float y, float width, float height)
{
    Animation* an = get_animation(v);
    an->need_re_render = 0;
    if (an->params[an->currentIndex])
        hiview_canvas_draw_param_image(v, c, an->params[an->currentIndex], an->currentPosX, an->currentPosY);
}

void post_render(HVIEW view, HCONTEXT context)
{
}


