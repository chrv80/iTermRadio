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
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <string.h>
#include <mpv/client.h>
#include "defs_funcs.h" 

int load_streams_from_list(char *list_path, char streams[][256]){
    FILE *list = fopen(list_path,"r");

    // if default file does not exist in the same directory or the file given does not exist, it checks for default file in home folder
    if (!list) {
        char *home = getenv("HOME");
        if (home == NULL) {
            printf("Home folder not found!\n");
            exit(-1);
        }
        char home_list_path[256] = {0};
        sprintf(home_list_path,"%s/.radiolist.lst",home);
        list = fopen(home_list_path,"r");
    }

    if (!list) {
        printf("Radio list file not found!\n");
        exit(-2);
    }
    
    int line = 0;
    
    while (fgets(streams[line], 256, list)) {
        streams[line][strcspn(streams[line ],"\n")]='\0';
        line++;
        if (line > 99) {
            break;
        }
    }
    fclose(list);

    return line;
}


int main(int argc, char *argv[]) {
    
    int inp;
    mpv_handle *player;
    int player_init_status = 0;
    char streams[100][256] = {{0}};
    int stream_count = 1;
    int current_stream = 0;

    char *logo = "  by chrv80";

    // for not very fast scrolling info
    int loops = 0;
    int offset = 0;

    // mouse inputs
    MEVENT mouse_event;

    // volume control
    double volume = 50.0;
    char volume_str[20] = {0};
    double l_vu;
    double r_vu;
    sprintf(volume_str,"Volume: %.1f",volume);

    // play/pause control
    int pause = 0;
    char play_state[10] = "Playing";
    positions_t positions = {0};
    button_t buttons[NUM] = {{0}};
    // metadata
    const char *af_add[4] = {"af", "add", "@astats:lavfi=[astats=metadata=1:reset=1:measure_overall=none]", NULL};
    char *station_name = NULL;

    
    if (argc > 3) {
        printf("%s\n", USAGE);
        return -1;
    }

    if (argc == 1) {
        stream_count = load_streams_from_list("radiolist.lst", streams);
    } else {
        if (strcmp(argv[1],"--help") == 0) {
            printf("%s\n", HELP);
            return -1;
        }

        if (strcmp(argv[1],"--stream") == 0) {
            strcpy(streams[0],argv[2]);
        }

        if (strcmp(argv[1],"--list") == 0) {
            stream_count = load_streams_from_list(argv[2], streams);
        } else {
            printf("Unknown option!\n%s\n", USAGE);
            return -1;
        }
    }

    player_init_status = init_mpv(&player, &volume);
    
    if (player_init_status < 0) {
        printf("Error initializing mpv player!\nError: %d\n", player_init_status);
    }     
    
    // initialize player screen
    initscr();
    keypad(stdscr,TRUE);
    curs_set(0);

    // for colors
    if(has_colors()){
        start_color();
        use_default_colors();
    }
    init_pair(1, COLOR_GREEN, -1);


    timeout(20); 
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
    mouseinterval(0);
    print_radio(&positions);
    cbreak();
    noecho();

    // initialize positions for other buttons
    positions.x_vol_down = positions.x_vol_up + 6;
    positions.y_vol_down = positions.y_vol_up;
    positions.x_off_key = positions.x_vol_down + 6;
    positions.y_off_key = positions.y_vol_down;
    positions.x_prev_key = positions.x_play_key - 6;
    positions.y_prev_key = positions.y_play_key;
    positions.x_next_key = positions.x_off_key + 6;
    positions.y_next_key = positions.y_off_key;
    positions.x_stream_info = positions.x_play_state;
    positions.y_stream_info = positions.y_play_state + 2;
    positions.y_bye += 3;

    //draw buttons
    init_buttons(positions,buttons);
    for(int i = 0; i<NUM; i++){
        draw_button(buttons[i]);
    }

    mvaddstr(positions.y_play_state,positions.x_play_state, play_state);
    mvaddstr(positions.y_vol_state,positions.x_vol_state,volume_str);

    // keep metadata
    mpv_command(player, af_add);

    // start playing
    const char *play[3] = {"loadfile", streams[current_stream], NULL};  
    mpv_command(player,play);
    
    inp = getch();
    
    while (1) {

        if(inp == KEY_MOUSE || inp == KEY_UP || inp == KEY_DOWN || inp == ' ' || inp == 'q' || inp == 'Q' || inp == ESC || inp == KEY_RIGHT || inp == KEY_LEFT){
            if (inp == KEY_MOUSE) {    
                if (getmouse(&mouse_event) == OK){
                    if (mouse_event.bstate & BUTTON1_PRESSED) {
                        if (mouse_event.y == positions.y_vol_up && mouse_event.x >= positions.x_vol_up && mouse_event.x <= positions.x_vol_up + 4){
                            buttons[1].pressed = 1;
                            draw_button(buttons[1]);
                        } else if (mouse_event.y == positions.y_vol_down && mouse_event.x >= positions.x_vol_down && mouse_event.x <= positions.x_vol_down + 4){
                            buttons[2].pressed = 1;
                            draw_button(buttons[2]);
                        } else if (mouse_event.y == positions.y_play_key && mouse_event.x >= positions.x_play_key && mouse_event.x <= positions.x_play_key + 2){
                            buttons[0].pressed = 1;
                            draw_button(buttons[0]);
                        } else if (mouse_event.y == positions.y_off_key && mouse_event.x >= positions.x_off_key && mouse_event.x <= positions.x_off_key + 2){
                            buttons[3].pressed = 1;
                            draw_button(buttons[3]);
                        } else if (mouse_event.y == positions.y_prev_key && mouse_event.x >= positions.x_prev_key && mouse_event.x <= positions.x_prev_key + 2){
                            buttons[4].pressed = 1;
                            draw_button(buttons[4]);
                        } else if (mouse_event.y == positions.y_next_key && mouse_event.x >= positions.x_next_key && mouse_event.x <= positions.x_next_key + 2){
                            buttons[5].pressed = 1;
                            draw_button(buttons[5]);
                        }

                    } else if (mouse_event.bstate & BUTTON1_RELEASED) {
                        if (buttons[1].pressed == 1){
                            buttons[1].pressed = 0;
                            draw_button(buttons[1]);
                            volume_increase(&player, &volume, volume_str);
                        } else if (buttons[2].pressed == 1){
                            buttons[2].pressed = 0;
                            draw_button(buttons[2]);
                            volume_decrease(&player, &volume, volume_str);
                        } else if (buttons[0].pressed == 1){
                            buttons[0].pressed = 0;
                            toggle_play_pause(&player, &pause, play_state, &buttons[0]);
                            if (pause == 1) {
                                draw_bar(positions.x_l_bar_pos,positions.y_l_bar_pos,'L',0);
                                draw_bar(positions.x_r_bar_pos,positions.y_r_bar_pos,'R',0);
                            }
                            draw_button(buttons[0]);
                        } else if (buttons[3].pressed == 1){
                            buttons[3].pressed = 0;
                            draw_button(buttons[3]);
                            break;
                        } else if (buttons[4].pressed == 1){
                            buttons[4].pressed = 0;
                            draw_button(buttons[4]);
                            if(current_stream > 0){
                                current_stream--;
                                const char *play[4] = {"loadfile", streams[current_stream], "replace", NULL};
                                mpv_command(player,play);
                            }
                            
                        } else if (buttons[5].pressed == 1){
                            buttons[5].pressed = 0;
                            draw_button(buttons[5]);
                            if(current_stream < stream_count -1){
                                current_stream++;
                                const char *play[4] = {"loadfile", streams[current_stream], "replace", NULL};
                                mpv_command(player,play);
                            }
                        }
                    }

                }
            } else if (inp == KEY_UP) {
                buttons[1].pressed = 1;
                draw_button(buttons[1]);
                volume_increase(&player, &volume, volume_str);
                usleep(100000);
                buttons[1].pressed = 0;
                draw_button(buttons[1]);
            } else if (inp == KEY_DOWN){
                buttons[2].pressed = 1;
                draw_button(buttons[2]);
                volume_decrease(&player, &volume, volume_str);
                usleep(100000);
                buttons[2].pressed = 0;
                draw_button(buttons[2]);
            } else if (inp == ' ') {
                buttons[0].pressed = 1;
                draw_button(buttons[0]);
                toggle_play_pause(&player, &pause, play_state, &buttons[0]);
                usleep(100000);
                buttons[0].pressed = 0;
                if (pause == 1) {
                    draw_bar(positions.x_l_bar_pos,positions.y_l_bar_pos,'L',0);
                    draw_bar(positions.x_r_bar_pos,positions.y_r_bar_pos,'R',0);
                }
                draw_button(buttons[0]);
            } else if (inp == 'q' || inp == 'Q' || inp == ESC) {
                buttons[3].pressed = 1;
                draw_button(buttons[3]);
                usleep(100000);
                buttons[3].pressed = 0;
                draw_button(buttons[3]);
                break;
            } else if (inp == KEY_RIGHT){
                if(current_stream < stream_count -1){
                    buttons[5].pressed = 1;
                    draw_button(buttons[5]);
                    current_stream++;
                    const char *play[4] = {"loadfile", streams[current_stream], "replace", NULL};
                    mpv_command(player,play);
                    usleep(100000);
                    buttons[5].pressed = 0;
                    draw_button(buttons[5]);
                }
            } else if (inp == KEY_LEFT){
                if(current_stream > 0){
                    buttons[4].pressed = 1;
                    draw_button(buttons[4]);
                    current_stream--;
                    const char *play[4] = {"loadfile", streams[current_stream], "replace", NULL};
                    mpv_command(player,play);
                    usleep(100000);
                    buttons[4].pressed = 0;
                    draw_button(buttons[4]);
                }
            }
        }

        mpv_event *event = mpv_wait_event(player, 0);
        if (event->event_id == MPV_EVENT_SHUTDOWN){
            break;
        }

        if (pause == 0) {
            get_vu_meter_values(player,&l_vu, &r_vu);
            draw_bar(positions.x_l_bar_pos,positions.y_l_bar_pos,'L',l_vu);
            draw_bar(positions.x_r_bar_pos,positions.y_r_bar_pos,'R',r_vu);
        }

        mvaddstr(positions.y_play_state,positions.x_play_state, play_state);
        mvaddstr(positions.y_vol_state,positions.x_vol_state,volume_str);
        loops = (loops + 1) % 512; // for not increasing to infinity, it slows down the scrolling speed
        if (loops % 8 == 0) {
            offset++;
        }

        // for printing stream information
        if (mpv_get_property(player, "metadata/by-key/icy-name", MPV_FORMAT_STRING, &station_name) == 0 && station_name) {
            print_stream_info(positions, station_name, 23, &offset);
            mpv_free(station_name);
        } else {
            print_stream_info(positions, streams[current_stream], 23, &offset);
        }

        print_logo(positions,logo);
       
        inp = getch();
        
    }

    mpv_terminate_destroy(player);
    
    mvaddstr(positions.y_bye,positions.x_bye,"\rBYE...");
    refresh();
    sleep(1);
    endwin();
    return 0;
}
