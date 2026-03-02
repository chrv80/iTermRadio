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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mpv/client.h>
#include "defs_funcs.h" 

// initializing mpv
int init_mpv(mpv_handle **player, double *volume) {
    *player = mpv_create();
        if (!*player) {
        return -1;
    }    
    
    if (mpv_set_option_string(*player, "input-default-bindings", "yes") < 0) {
        return -2;
    }
    
    if (mpv_set_option_string(*player, "input-vo-keyboard", "no") < 0) {
        return -3;
    }
    
    if (mpv_set_option_string(*player, "video", "no") < 0){
        return -4;
    }
    if (mpv_initialize(*player) < 0) {
        return -5;
    }
    
    mpv_set_property(*player, "volume", MPV_FORMAT_DOUBLE, volume);
    return 1;
}

// get metadata for VU meter

int mpv_find_key(mpv_node *map, char *key, double *return_value){
    if (!map || map->format != MPV_FORMAT_NODE_MAP) {
        return 0;
    }
    mpv_node_list *list = map-> u.list;
    for (int i = 0; i < list->num; i++){
        if (list->keys && list->keys[i] && (strcmp(list->keys[i],key) == 0)){
            mpv_node *map_value = &list->values[i];
            if (map_value->format == MPV_FORMAT_DOUBLE){
                *return_value = map_value->u.double_;
                return 1;
            } else if (map_value->format == MPV_FORMAT_INT64) {
                *return_value = (double) map_value->u.int64;
                return 1;
            } else if (map_value->format == MPV_FORMAT_STRING && map_value->u.string) {
                *return_value = atof(map_value->u.string);
                return 1;
            }
        }
    }
    return 0;
}

void get_vu_meter_values(mpv_handle *player, double *l_vu, double *r_vu){
    mpv_node map = {0};
    double l_db = -120, r_db = -120;

    mpv_get_property(player, "af-metadata/astats", MPV_FORMAT_NODE, &map);
    mpv_find_key(&map, "lavfi.astats.1.Peak_level", &l_db);
    mpv_find_key(&map, "lavfi.astats.2.Peak_level", &r_db);

    mpv_free_node_contents(&map);
    

    if (l_db < -60) {
        l_db = -60;
    } else if (l_db > 0) {
        l_db = 0;
    }
    if (r_db < -60) {
        r_db = -60;
    } else if (r_db > 0) {
        r_db = 0;
    }

    *l_vu = (l_db + 10) / 10;
    *r_vu = (r_db + 10) / 10;

}

// mpv control functions from UI

void volume_increase(mpv_handle ** player, double *volume, char *volume_str){
    *volume += 5;
    if (*volume > 100) {
        *volume = 100;
    }
    mpv_set_property(*player, "volume", MPV_FORMAT_DOUBLE, volume);
    sprintf(volume_str,"Volume: %.1f",*volume);
}

void volume_decrease(mpv_handle ** player, double *volume, char *volume_str){
    *volume -= 5;
    if (*volume < 0) {
        *volume = 0;
    }
    mpv_set_property(*player, "volume", MPV_FORMAT_DOUBLE, volume);
    sprintf(volume_str,"Volume: %.1f",*volume);
}

void toggle_play_pause(mpv_handle **player, int *pause, char *play_state, button_t *play_button){
    if ((*pause = (*pause + 1) % 2) == 1){
        sprintf(play_state, "Pause    ");
        sprintf(play_button->label,"|>");       
    } else {
        sprintf(play_state, "Playing");
        sprintf(play_button->label,"II");
    }
    mpv_set_property(*player, "pause", MPV_FORMAT_FLAG, pause);
}
