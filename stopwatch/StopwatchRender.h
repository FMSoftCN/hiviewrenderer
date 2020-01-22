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

#ifndef __WATCHFACE_RENDER_H__
#define __WATCHFACE_RENDER_H__

#include <stdint.h>

#define INVALID_HANDLE 0

#define INVALID_MODE          0 
#define ACTIVE_MODE           1
#define PASSIVE_MODE          2
#define MIXED_MODE            3


typedef uintptr_t HVIEW;
typedef uintptr_t HCONTEXT;

#ifdef __cplusplus
extern "C" {
#endif

void initialize(HVIEW v, HCONTEXT c);
int create(HVIEW view, HCONTEXT context, int* activeModeIntervalMs);
int pre_render(HVIEW view, HCONTEXT context);
void render(HVIEW v, HCONTEXT context, float x, float y, float width, float height);
void post_render(HVIEW view, HCONTEXT context);
void destroy(HVIEW view, HCONTEXT context);
void terminate(HVIEW view, HCONTEXT context);


#ifdef __cplusplus
} // extern "C"
#endif

#endif // __WATCHFACE_RENDER_H__