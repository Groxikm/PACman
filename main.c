#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <unistd.h>
#include <windows.h>
#include <math.h>

#include "structs.h"

void setCursorPosition(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hConsole, coord);
}

void hideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 1; // Set the cursor size to 1 (invisible)
    cursorInfo.bVisible = FALSE; // Hide the cursor
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

int width_of_file(char *level_file_name) {
    int length=0;
    char ch;
    FILE *fptr = fopen(level_file_name, "r");

    while ((ch=fgetc(fptr))!='\n') length++;

    fclose(fptr);
    return length;
}

int height_of_file(char *level_file_name) {
    int length=0;
    char ch;
    FILE *fptr = fopen(level_file_name, "r");

    while ((ch=fgetc(fptr))!=EOF) 
        if (ch=='\n') length++;
    
    fclose(fptr);
    return ++length;
}

void find_pos_of_(struct object *obj, struct lvl_data *lvl) {
    for (int i = 0; i < lvl->h; i++) 
        for (int j = 0; j < lvl->w+1; j++) 
            if (*(lvl->map+i*(lvl->w+1)+j)== obj->character) { // lvl->w+1 for '\n' symbol
                obj->x=j;
                obj->y=i;
            }
}

void load_level(char *level_file_name, char *array) {
    FILE *fptr = fopen(level_file_name, "r");
    int i = 0;
    char ch;
    while ((ch=fgetc(fptr))!=EOF) {
        if (ch==wall_c || ch==player_c || ch==void_c || ch==magic_stone_c || ch==ghost_c || ch=='\n') 
            *(array+i) = ch;
        else 
            *(array+i) = void_c;
        i++;
    }
    *(array+i)='\0';
    fclose(fptr);
}

void load_level_by_number(int level_number, struct object *player, struct object *ghost, struct lvl_data *lvl) {
    system("cls");
    char levelFileName[File_name_limit];
    sprintf(levelFileName, "level%d.txt", level_number);

    if (lvl->map != NULL) free(lvl->map);
    
    lvl->h = height_of_file(levelFileName);
    lvl->w = width_of_file(levelFileName);
    lvl->map = (char*) malloc(lvl->h * (lvl->w + 1) * sizeof(char));
    load_level(levelFileName, lvl->map);

    find_pos_of_(ghost, lvl);
    find_pos_of_(player, lvl);
}

void change_pos(char figure_type, char change_type, int start, int end, struct lvl_data *lvl) {
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
            if (is_the_character_after_movement_(magic_stone_c, player, lvl, -step, 0)) load_level_by_number(++lvl->number, player, ghost,  lvl); //(*(lvl->map +player->x +(player->y-1)*(lvl->w+1))==magic_stone_c)
            else
            if (is_the_character_after_movement_(ghost_c, player, lvl, -step, 0)) break;
            else
            if (!is_the_character_after_movement_(wall_c, player, lvl, -step, 0)) {
                player->y-=step;
                change_pos(player_c, void_c, player->x +(player->y+step)*(lvl->w+1), player->x+player->y*(lvl->w+1), lvl);
            }
        }
        if (GetAsyncKeyState(VK_DOWN) & 1) {
            if (is_the_character_after_movement_(magic_stone_c, player, lvl, step, 0)) load_level_by_number(++lvl->number, player, ghost, lvl); //(*(lvl->map +player->x +(player->y+1)*(lvl->w+1))==magic_stone_c)
            else
            if (is_the_character_after_movement_(ghost_c, player, lvl, step, 0)) break;
            else
            if (!is_the_character_after_movement_(wall_c, player, lvl, step, 0)) {
                player->y+=step;
                change_pos(player_c, void_c, player->x +(player->y-step)*(lvl->w+1), player->x+player->y*(lvl->w+1), lvl);
            }
        }
        if (GetAsyncKeyState(VK_LEFT) & 1) {
            if (is_the_character_after_movement_(magic_stone_c, player, lvl, 0, -step)) load_level_by_number(++lvl->number, player, ghost, lvl); //(*(lvl->map +player->x-1 +player->y*(lvl->w+1))==magic_stone_c)
            else
            if (is_the_character_after_movement_(ghost_c, player, lvl, 0, -step)) break;
            else
            if (!is_the_character_after_movement_(wall_c, player, lvl, 0, -step)) {
                player->x-=step;
                change_pos(player_c, void_c, player->x +step +player->y*(lvl->w+1), player->x+player->y*(lvl->w+1), lvl);
            }
        }
        if (GetAsyncKeyState(VK_RIGHT) & 1) {
            if (is_the_character_after_movement_(magic_stone_c, player, lvl, 0, step)) load_level_by_number(++lvl->number, player, ghost, lvl); //(*(lvl->map +player->x+1 +player->y*(lvl->w+1))==magic_stone_c)
            else
            if (is_the_character_after_movement_(ghost_c, player, lvl, 0, step)) break;
            else
            if (!is_the_character_after_movement_(wall_c, player, lvl, 0, step)) {
                player->x+=step;
                change_pos(player_c, void_c, player->x -step +player->y*(lvl->w+1), player->x +player->y*(lvl->w+1), lvl);
            }
        }
        //ghost physics. horizontal moving
        if (!is_the_character_after_movement_(wall_c, ghost, lvl, 0, ghost->velocity))
            if(is_the_character_after_movement_(player_c, ghost, lvl, 0, ghost->velocity)) break; 
            else {
                ghost->x += ghost->velocity;
                change_pos(ghost_c, void_c, ghost->x-ghost->velocity+ghost->y*(lvl->w+1), ghost->x+ghost->y*(lvl->w+1), lvl);
            }
        else ghost->velocity *= -1;

        printf(lvl->map);
        usleep(43333);
        setCursorPosition(0,0);
    }
}

int main() {
    struct object player = {.x=-1, .y=-1, .character = player_c};
    struct object magic_stone = {.x=-1, .y=-1, .character = magic_stone_c};
    struct object ghost = {.x=-1, .y=-1, .character = ghost_c, .velocity = 1};
    struct lvl_data lvl = {.w=-1, .h=-1, .ghosts_evaded = 0, .magic_stones = 0, .number = 1};

    hideCursor();
    load_level_by_number(lvl.number, &player, &ghost, &lvl);
    game_loop(&player, &ghost, &lvl);

    free(lvl.map);
    printf("END GAME...");
    sleep(1);
    return 0;
}