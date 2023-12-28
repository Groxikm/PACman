#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <unistd.h>
#include <windows.h>
#include <math.h>

#include "structs.h"

void find_pos_of_(struct object *obj, struct lvl_data *lvl) {
    for (int i = 0; i < lvl->h; i++) 
        for (int ii = 0; ii < lvl->w+1; ii++) 
            if (*(lvl->map+i*(lvl->w+1)+ii)== obj->character) { // lvl->w+1 for '\n' symbol
                obj->x=ii;
                obj->y=i;
            }
}

void ghost_instance_init(struct lvl_data *lvl, int position){
    int y_pos = position / lvl->w;
    int x_pos = position % lvl->w;
    lvl->ghost_array[lvl->ghosts_amount].x = x_pos;
    lvl->ghost_array[lvl->ghosts_amount].y = y_pos;
    lvl->ghost_array[lvl->ghosts_amount].character = ghost_c; // Example character
    lvl->ghost_array[lvl->ghosts_amount].velocity = 1;
}

void load_level(char *level_file_name, struct lvl_data *lvl) {
    FILE *fptr = fopen(level_file_name, "r");
    int i;
    lvl->ghosts_amount, i = 0;
    char ch;
    while ((ch=fgetc(fptr))!=EOF) {
        if (ch==wall_c || ch==player_c || ch==void_c || ch==magic_stone_c || ch==ghost_c || ch=='\n') {
            if (ch==ghost_c && lvl->ghosts_amount < ghost_limit) {lvl->ghosts_amount++; ghost_instance_init(lvl,i);} // initializing ghosts
            *(lvl->map+i) = ch;
            }
        else 
            *(lvl->map+i) = void_c;
        i++;
    }
    *(lvl->map+i)='\0';
    fclose(fptr);
}

void load_level_by_number(int level_number, struct object *player, struct lvl_data *lvl) {
    if (lvl->map != NULL) free(lvl->map);
    system("cls");
    char levelFileName[File_name_limit];
    sprintf(levelFileName, "level%d.txt", level_number);
    
    lvl->h = height_of_file(levelFileName);
    lvl->w = width_of_file(levelFileName);
    lvl->map = (char*) malloc(lvl->h * (lvl->w + 1) * sizeof(char));
    load_level(levelFileName, lvl);

    find_pos_of_(player, lvl);
}

void change_object_pos(char figure_type, char change_type, int y_change, int x_change, struct object *object, struct lvl_data *lvl) {    
    int start = object->x +(object->y)*(lvl->w+1);
    int end = object->x+x_change +(object->y+y_change)*(lvl->w+1);
    object->y += y_change;
    object->x += x_change;
    *(lvl->map+start) = change_type;
    *(lvl->map+end) = figure_type; 
}

int is_the_character_after_movement_(char character, struct object *object, struct lvl_data *lvl, int y_change, int x_change){
    if (*(lvl->map +object->x+x_change +(object->y+y_change)*(lvl->w+1))==character) 
        return 1;
    return 0;
}

int game_loop(struct object *player, struct lvl_data *lvl) {
    while (!GetAsyncKeyState(VK_ESCAPE)) {
        if (GetAsyncKeyState(VK_UP) & 1) {
            if (is_the_character_after_movement_(magic_stone_c, player, lvl, -step, 0)) load_level_by_number(++lvl->number, player, lvl); //(*(lvl->map +player->x +(player->y-1)*(lvl->w+1))==magic_stone_c)
            else
            if (!is_the_character_after_movement_(wall_c, player, lvl, -step, 0)) {
                change_object_pos(player->character, void_c, -step, 0, player, lvl); //player->y-=step;
            }
        }
        if (GetAsyncKeyState(VK_DOWN) & 1) {
            if (is_the_character_after_movement_(magic_stone_c, player, lvl,  step, 0)) load_level_by_number(++lvl->number, player, lvl); //(*(lvl->map +player->x +(player->y+1)*(lvl->w+1))==magic_stone_c)
            else
            if (!is_the_character_after_movement_(wall_c, player, lvl,  step, 0)) {
                change_object_pos(player->character, void_c, step, 0, player, lvl); //player->y+=step;
            }
        }
        if (GetAsyncKeyState(VK_LEFT) & 1) {
            if (is_the_character_after_movement_(magic_stone_c, player, lvl, 0, -step)) load_level_by_number(++lvl->number, player, lvl); //(*(lvl->map +player->x-1 +player->y*(lvl->w+1))==magic_stone_c)
            else
            if (!is_the_character_after_movement_(wall_c, player, lvl, 0, -step)) {
                change_object_pos(player->character, void_c, 0, -step, player, lvl); //player->x-=step;
            }
        }
        if (GetAsyncKeyState(VK_RIGHT) & 1) {
            if (is_the_character_after_movement_(magic_stone_c, player, lvl, 0,  step)) load_level_by_number(++lvl->number, player, lvl); //(*(lvl->map +player->x+1 +player->y*(lvl->w+1))==magic_stone_c)
            else
            if (!is_the_character_after_movement_(wall_c, player, lvl, 0,  step)) {
                change_object_pos(player->character, void_c, 0, step, player, lvl); //player->x+=step;
            }
        }

        for(int ii = 0; ii <= lvl->ghosts_amount; ii++) {
        if (!is_the_character_after_movement_(wall_c, &lvl->ghost_array[ii], lvl, 0, lvl->ghost_array[ii].velocity))
            if(is_the_character_after_movement_(player->character, &lvl->ghost_array[ii], lvl, 0, 0) 
            || is_the_character_after_movement_(player->character, &lvl->ghost_array[ii], lvl, 0, lvl->ghost_array[ii].velocity)) return 2;
                else 
                change_object_pos(ghost_c, void_c, 0, lvl->ghost_array[ii].velocity, &lvl->ghost_array[ii], lvl);
        else lvl->ghost_array[ii].velocity *= -1;
        }

        printf(lvl->map); printf("\nghost amount: %d", lvl->ghosts_amount);
        usleep(43333);
        setCursorPosition(0,0);
    }
    return 0;
}

int main() {
    struct object player = {.x=-1, .y=-1, .character = player_c};
    struct object magic_stone = {.x=-1, .y=-1, .character = magic_stone_c};
    struct object ghost = {.x=-1, .y=-1, .character = ghost_c, .velocity = 1};
    struct lvl_data lvl = {.w=-1, .h=-1, .magic_stones = 0, .number = 1};

    hideCursor();
    load_level_by_number(lvl.number, &player, &lvl);
    switch (game_loop(&player, &lvl)){
        case 0:
        printf("END GAME... PRESSED ESC \n");
        break;
        case 2:
        printf("END GAME... CAUGHT BY GHOST \n");
        break;
    }

    free(lvl.map);
    
    sleep(1);
    return 0;
}