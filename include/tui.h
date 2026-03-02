/*
 * iTermRadio
 * Copyright (C) 2026 chrv80
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <https://www.gnu.org/licenses/>.
 */

#ifndef TUI_H
#define TUI_H

typedef struct positions_s {
    // volume status and keys positions
    int x_vol_state;
    int y_vol_state;
    int x_vol_up;
    int y_vol_up; 
    int x_vol_down; 
    int y_vol_down;
    // play/pause status and key position
    int x_play_state;
    int y_play_state;
    int x_play_key;
    int y_play_key;
    int y_off_key;
    int x_off_key;
    int x_l_bar_pos;
    int y_l_bar_pos;
    int x_r_bar_pos;
    int y_r_bar_pos;
    int x_prev_key;
    int y_prev_key;
    int x_next_key;
    int y_next_key;
    int x_stream_info;
    int y_stream_info;
    int x_logo;
    int y_logo;
    int x_bye;
    int y_bye;
} positions_t;

typedef struct button_s {
    int x,y;
    int pressed;
    char label[20];
} button_t;

#endif