#
# 
#                           IMPORTANT NOTICE
# 
#  The following open source license statement does not apply to any
#  entity in the Exception List published by FMSoft.
# 
#  For more information, please visit:
# 
#  https://www.fmsoft.cn/exception-list
# 
#
#   This file is part of HybridOS, a developing operating system based on
#   MiniGUI. HybridOs will support embedded systems and smart IoT devices.
#
#   Copyright (C) 2020 Beijing FMSoft Technologies Co., Ltd.
#
#   This program is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
#   Or,
#
#   As this program is a library, any link to this program must follow
#   GNU General Public License version 3 (GPLv3). If you cannot accept
#   GPLv3, you need to be licensed from FMSoft.
#
#   If you have got a commercial license of this program, please use it
#   under the terms and conditions of the commercial license.
#
#   For more information about the commercial license, please refer to
#   <http://www.minigui.com/blog/minigui-licensing-policy/>.
#

cmake_minimum_required(VERSION 3.10)

project(stopwatch-default)

list(APPEND stopwatch_INCLUDE_DIRECTORIES
    .
)

list(APPEND stopwatch_SOURCES
    StopwatchRender.c 
)

include_directories(${stopwatch_INCLUDE_DIRECTORIES})
add_library(stopwatch-default SHARED ${stopwatch_SOURCES})

install(TARGETS stopwatch-default DESTINATION /opt/view-renderers/views.fmsoft.cn/)
install(TARGETS stopwatch-default DESTINATION /opt/view-renderers/views.default/)
