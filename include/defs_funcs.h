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

#ifndef DEFS_FUNCS_H
#define DEFS_FUNCS_H


#define USAGE "Usage: player [OR] player --stream URL [OR] player --list radiolist.lst\n For more help: player --help"
#define HELP "Usage: player [OR] player --stream URL [OR] player --list radiolist.lst\nBy default iTermRadio opens radiolist.lst file in the same directory or .radiolist.lst in home folder.\nThe file is a text file, each line is a internet station URL.\nFor usage you can use keyboard or mouse (in ppc OS X Leopard mouse input does not work, other mac os versions are not tested).\nFor keyboard use, \"q\" or ESC for exit, up/down arrows for volume, left/right arrows to change stations, space for play/pause"
#define NUM 6
#define ESC 27

#include "tui.h"
#include <mpv/client.h>

int load_streams_from_list(char *list_path, char streams[][256]);
void init_buttons(positions_t pos, button_t *buttons);
void draw_button(button_t button);
void print_stream_info(positions_t pos, char *info, int width, int *offset);
int init_mpv(mpv_handle **player, double *volume);
int mpv_find_key(mpv_node *map, char *key, double *return_value);
void get_vu_meter_values(mpv_handle *player, double *l_vu, double *r_vu);
void draw_bar(int x, int y, char label, double value);
void print_logo(positions_t pos, char *logo);
void volume_increase(mpv_handle ** player, double *volume, char *volume_str);
void volume_decrease(mpv_handle ** player, double *volume, char *volume_str);
void toggle_play_pause(mpv_handle **player, int *pause, char *play_state, button_t *play_button);
void print_radio(positions_t *pos);

#endif
