#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <unistd.h>
#include <windows.h>
#include <math.h>

#include "structs.h"

void find_pos_of_(struct object *obj, struct lvl_data *lvl) {
    for (int i = 0; i < lvl->h; i++) 
        for (int j = 0; j < lvl->w+1; j++) 
            if (*(lvl->map+i*(lvl->w+1)+j)== obj->character) { // lvl->w+1 for '\n' symbol
                obj->x=j;
                obj->y=i;
            }
}

void load_level(char *level_file_name, struct lvl_data *lvl) {
    FILE *fptr = fopen(level_file_name, "r");
    int i;
    lvl->ghosts_amount, i = 0;
    char ch;
    while ((ch=fgetc(fptr))!=EOF) {
        if (ch==wall_c || ch==player_c || ch==void_c || ch==magic_stone_c || ch==ghost_c || ch=='\n') {
            *(lvl->map+i) = ch;
            if (ch==ghost_c) lvl->ghosts_amount++;
            }
        else 
            *(lvl->map+i) = void_c;
        i++;
    }
    *(lvl->map+i)='\0';
    fclose(fptr);
}

void load_level_by_number(int level_number, struct object *player, struct object *ghost, struct lvl_data *lvl) {
    if (lvl->map != NULL) free(lvl->map);
    system("cls");
    char levelFileName[File_name_limit];
    sprintf(levelFileName, "level%d.txt", level_number);
    
    lvl->h = height_of_file(levelFileName);
    lvl->w = width_of_file(levelFileName);
    lvl->map = (char*) malloc(lvl->h * (lvl->w + 1) * sizeof(char));
    load_level(levelFileName, lvl);

    find_pos_of_(ghost, lvl);
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

void game_loop(struct object *player, struct object *ghost, struct lvl_data *lvl) {
    while (!GetAsyncKeyState(VK_ESCAPE)) {
        if (GetAsyncKeyState(VK_UP) & 1) {
            if (is_the_character_after_movement_(magic_stone_c, player, lvl, -step, 0)) load_level_by_number(++lvl->number, player, ghost, lvl); //(*(lvl->map +player->x +(player->y-1)*(lvl->w+1))==magic_stone_c)
            else
            if (is_the_character_after_movement_(ghost_c, player, lvl, -step, 0)) break;
            else
            if (!is_the_character_after_movement_(wall_c, player, lvl, -step, 0)) {
                change_object_pos(player->character, void_c, -step, 0, player, lvl); //player->y-=step;
            }
        }
        if (GetAsyncKeyState(VK_DOWN) & 1) {
            if (is_the_character_after_movement_(magic_stone_c, player, lvl, step, 0)) load_level_by_number(++lvl->number, player, ghost, lvl); //(*(lvl->map +player->x +(player->y+1)*(lvl->w+1))==magic_stone_c)
            else
            if (is_the_character_after_movement_(ghost_c, player, lvl, step, 0)) break;
            else
            if (!is_the_character_after_movement_(wall_c, player, lvl, step, 0)) {
                change_object_pos(player->character, void_c, step, 0, player, lvl); //player->y+=step;
            }
        }
        if (GetAsyncKeyState(VK_LEFT) & 1) {
            if (is_the_character_after_movement_(magic_stone_c, player, lvl, 0, -step)) load_level_by_number(++lvl->number, player, ghost, lvl); //(*(lvl->map +player->x-1 +player->y*(lvl->w+1))==magic_stone_c)
            else
            if (is_the_character_after_movement_(ghost_c, player, lvl, 0, -step)) break;
            else
            if (!is_the_character_after_movement_(wall_c, player, lvl, 0, -step)) {
                change_object_pos(player->character, void_c, 0, -step, player, lvl); //player->x-=step;
            }
        }
        if (GetAsyncKeyState(VK_RIGHT) & 1) {
            if (is_the_character_after_movement_(magic_stone_c, player, lvl, 0, step)) load_level_by_number(++lvl->number, player, ghost, lvl); //(*(lvl->map +player->x+1 +player->y*(lvl->w+1))==magic_stone_c)
            else
            if (is_the_character_after_movement_(ghost_c, player, lvl, 0, step)) break;
            else
            if (!is_the_character_after_movement_(wall_c, player, lvl, 0, step)) {
                change_object_pos(player->character, void_c, 0, step, player, lvl); //player->x+=step;
            }
        }

        if (!is_the_character_after_movement_(wall_c, ghost, lvl, 0, ghost->velocity))
            if(is_the_character_after_movement_(player->character, ghost, lvl, 0, ghost->velocity)) break; 
            else 
            {change_object_pos(ghost_c, void_c, 0, ghost->velocity, ghost, lvl); }
        else ghost->velocity *= -1;

        printf(lvl->map); printf("\nghost amount: %d", lvl->ghosts_amount);
        usleep(43333);
        setCursorPosition(0,0);
    }
}

int main() {
    struct object player = {.x=-1, .y=-1, .character = player_c};
    struct object magic_stone = {.x=-1, .y=-1, .character = magic_stone_c};
    struct object ghost = {.x=-1, .y=-1, .character = ghost_c, .velocity = 1};
    struct lvl_data lvl = {.w=-1, .h=-1, .magic_stones = 0, .number = 1};

    hideCursor();
    load_level_by_number(lvl.number, &player, &ghost, &lvl);
    game_loop(&player, &ghost, &lvl);

    free(lvl.map);
    printf("END GAME...");
    sleep(1);
    return 0;
}