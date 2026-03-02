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
#include <ncurses.h>
#include "defs_funcs.h" 


void init_buttons(positions_t pos, button_t *buttons) {
    
    buttons[0].x=pos.x_play_key;
    buttons[0].y=pos.y_play_key;
    sprintf(buttons[0].label,"II");
    buttons[0].pressed = 0;
    
    buttons[1].x=pos.x_vol_up;
    buttons[1].y=pos.y_vol_up;
    sprintf(buttons[1].label,"Vol+");
    buttons[1].pressed = 0;

    buttons[2].x=pos.x_vol_down;
    buttons[2].y=pos.y_vol_down;
    sprintf(buttons[2].label,"Vol-");
    buttons[2].pressed = 0;

    buttons[3].x=pos.x_off_key;
    buttons[3].y=pos.y_off_key;
    sprintf(buttons[3].label,"OFF");
    buttons[3].pressed = 0;

    buttons[4].x=pos.x_prev_key;
    buttons[4].y=pos.y_prev_key;
    sprintf(buttons[4].label,"<<");
    buttons[4].pressed = 0;

    buttons[5].x=pos.x_next_key;
    buttons[5].y=pos.y_next_key;
    sprintf(buttons[5].label,">>");
    buttons[5].pressed = 0;

}

void draw_button(button_t button) {
    mvaddstr(button.y, button.x, button.label);
    if(button.pressed == 1) {
        move(button.y,button.x);
        chgat(4,A_BOLD, 0, NULL);
    }
}

void draw_bar(int x, int y, char label, double value) {
    int bars = (int) (value * 14 + 0.5);
    char bar_start[4] = {label, ':', '[','\0'};
    
    mvaddstr(y,x,bar_start);

    for (int i = 0; i < 16; i++){
        if (i < bars) {
            attron(COLOR_PAIR(1));
            mvaddch(y, x + 3 + i, '=');
            attroff(COLOR_PAIR(1));
        } else {
            mvaddch(y, x + 3 + i, ' ');
        }
    }

    mvaddch(y, x + 18, ']');
    
}

void print_logo(positions_t pos, char *logo){
    attron(A_DIM | A_ITALIC);
    mvaddstr(pos.y_logo,pos.x_logo,logo);
    attroff(A_DIM | A_ITALIC);
}

void print_stream_info(positions_t pos, char *info, int width, int *offset) {
    int len = strlen(info);
    attron(A_ITALIC | A_DIM);
    if (len <= width) {
        mvaddstr(pos.y_stream_info, pos.x_stream_info, info);
        for (int i = strlen(info); i < width; i++){
            mvaddch(pos.y_stream_info, pos.x_stream_info + i, ' ');
        }
    } else{
        char total_info[512];
        sprintf(total_info, "%s     ",info);    
        int total_length = len + 5; // 3 for the gap

        int start = *offset % total_length; // if offset < total_length then we start from the offset, else from the begining
        
        for (int i = 0; i < width; i++) {
            if (start + i < total_length) {
                mvaddch(pos.y_stream_info, pos.x_stream_info + i, total_info[start+i]);
            } else {
                mvaddch(pos.y_stream_info, pos.x_stream_info + i, total_info[start + i - total_length]);
            }
        }

    }
    attroff(A_ITALIC | A_DIM);
}

void print_radio(positions_t *pos) { //arguments needed for positions for play/pause and volume control
    printw("\n\n");
    printw("\t\t               //\t\tInfo:\n");
    printw("\t\t             //\t\tSpace for play/pause\n");
    printw("\t\t           //\t\tArrows for vol/stations\n");
    printw("\t\t         //\t\tEsc [OR] q for exit\n");
    printw("\t\t       //\t\tOr just use mouse..\n");
    printw("\t\t     //\n");
    printw("\t  ----------------------------------------------------------\n");
    printw("\t  |\t\t\t\t\t\t\t   |\n");
    printw("\t  |\tRadio Player\t\t");
    getyx(stdscr, pos->y_play_key, pos->x_play_key);
    getyx(stdscr, pos->y_vol_up, pos->x_vol_up);
    pos->x_vol_up += 5;
    printw("\t\t\t   |\n");
    printw("\t  |\t\t\t\t\t   _______\t   |\n");
    printw("\t  |\t-------------------------\t //|__|__|\\\\\t   |\n");
    printw("\t  |\t|");
    getyx(stdscr, pos->y_play_state, pos->x_play_state);
    printw("\t\t\t|\t|__|__|__|__|\t   |\n");
    printw("\t  |\t|");
    getyx(stdscr, pos->y_vol_state, pos->x_vol_state);
    printw("\t\t\t|\t|__|__|__|__|\t   |\n");
    printw("\t  |\t|\t\t\t|\t|__|__|__|__|\t   |\n");
    printw("\t  |\t|");
    getyx(stdscr, pos->y_l_bar_pos, pos->x_l_bar_pos);
    printw("\t\t\t|\t \\\\|__|__|//\t   |\n");
    printw("\t  |\t|");
    getyx(stdscr, pos->y_r_bar_pos, pos->x_r_bar_pos);
    printw("\t\t\t|\t\t\t   |\n");
    printw("\t  |\t-------------------------\t");
    getyx(stdscr, pos->y_logo, pos->x_logo);
    printw("\t\t   |\n");
    printw("\t  |\t\t\t\t\t\t\t   |\n");
    getyx(stdscr, pos->y_bye, pos->x_bye);
    printw("\t  ----------------------------------------------------------\n");
}
