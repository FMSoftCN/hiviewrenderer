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
#include "AnimationRender.h"

#define TRUE           1
#define FALSE          0

typedef struct animation_data
{
    int animation_mode;
    int need_re_render;

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

    hiview_set_extra(v, an);
    return an;
}

Animation* get_animation(HVIEW v)
{
    return hiview_get_extra(v);
}

void initialize(HVIEW v, HCONTEXT c)
{
    init_animation(v);
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
    free(an);
}

void terminate(HVIEW view, HCONTEXT context)
{
}

void on_param_change(HVIEW v, HCONTEXT c, const char* name, const char* value)
{
}

int pre_render(HVIEW v, HCONTEXT c)
{
    int need_re_render = 0;
    Animation* an = get_animation(v);
    return need_re_render || an->need_re_render;
}

void render(HVIEW v, HCONTEXT c, float x, float y, float width, float height)
{
    printf("..................................................................animation render\n");
}

void post_render(HVIEW view, HCONTEXT context)
{
}


