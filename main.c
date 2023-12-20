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

int length_of_file(char *level_file_name) {
    int length=0;
    char ch;
    FILE *fptr = fopen(level_file_name, "r");

    while ((ch=fgetc(fptr))!=EOF) length++;

    fclose(fptr);
    return length;
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

    while ((ch=fgetc(fptr))!=EOF) if (ch=='\n') length++;
    
    length++;
    fclose(fptr);
    return length;
}

void find_pos_of_(struct object *obj, struct lvl_data *lvl) {
    for (int i = 0; i < lvl->h; i++) 
        for (int j = 0; j < lvl->w+1; j++) 
            if (*(lvl->map+i*(lvl->w+1)+j)== obj->character) { // lvl->w+1 - because '\n' symbol
                obj->x=j;
                obj->y=i;
            }
}

void change_pos(char figure_type, char change_type, int start, int end, struct lvl_data *lvl) {
    *(lvl->map+start) = change_type;
    *(lvl->map+end) = figure_type;
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

void load_level_by_number(int level_number, struct object *player, struct object *ghost, struct object *magic_stone, struct lvl_data *lvl) {
    system("cls");
    char levelFileName[File_name_limit];
    sprintf(levelFileName, "level%d.txt", level_number);

    if (lvl->map != NULL) free(lvl->map);
    
    lvl->h = height_of_file(levelFileName);
    lvl->w = width_of_file(levelFileName);
    lvl->map = (char*) malloc(lvl->h * (lvl->w + 1) * sizeof(char));
    load_level(levelFileName, lvl->map);

    find_pos_of_(&ghost, &lvl);
    find_pos_of_(&player, &lvl);
    find_pos_of_(&magic_stone, &lvl);
}

void game_loop(struct object *player, struct object *ghost, struct object *magic_stone, struct lvl_data *lvl) {
    while (!GetAsyncKeyState(VK_ESCAPE)) {
        if (GetAsyncKeyState(VK_UP) & 1) {
            if (*(lvl->map+player->x+(player->y-1)*(lvl->w+1))==magic_stone_c) load_level_by_number(++lvl->number, &player, &ghost, &magic_stone, &lvl);
            else
            if (*(lvl->map+player->x+(player->y-1)*(lvl->w+1))==ghost_c) break;
            else
            if (*(lvl->map+player->x+(player->y-1)*(lvl->w+1))!=wall_c) {
                player->y=player->y-1;
                change_pos(player_c, void_c, player->x+(player->y+1)*(lvl->w+1), player->x+player->y*(lvl->w+1), &lvl);
            }
        }
        if (GetAsyncKeyState(VK_DOWN) & 1) {
            if (*(lvl->map+player->x+(player->y+1)*(lvl->w+1))==magic_stone_c) load_level_by_number(++lvl->number, &player, &ghost, &magic_stone, &lvl);
            else
            if (*(lvl->map+player->x+(player->y+1)*(lvl->w+1))==ghost_c) break;
            else
            if (*(lvl->map+player->x+(player->y+1)*(lvl->w+1))!=wall_c) {
                player->y=player->y+1;
                change_pos(player_c, void_c, player->x+(player->y-1)*(lvl->w+1), player->x+player->y*(lvl->w+1), &lvl);
            }
        }
        if (GetAsyncKeyState(VK_LEFT) & 1) {
            if (*(lvl->map+player->x-1+player->y*(lvl->w+1))==magic_stone_c) load_level_by_number(++lvl->number, &player, &ghost, &magic_stone, &lvl);
            else
            if (*(lvl->map+player->x-1+player->y*(lvl->w+1))==ghost_c) break;
            else
            if (*(lvl->map+player->x-1+player->y*(lvl->w+1))!=wall_c) {
                player->x=player->x-1;
                change_pos(player_c, void_c, player->x+1+player->y*(lvl->w+1), player->x+player->y*(lvl->w+1), &lvl);
            }
        }
        if (GetAsyncKeyState(VK_RIGHT) & 1) {
            if (*(lvl->map+player->x+1+player->y*(lvl->w+1))==magic_stone_c) load_level_by_number(++lvl->number, &player, &ghost, &magic_stone, &lvl);
            else
            if (*(lvl->map+player->x+1+player->y*(lvl->w+1))==ghost_c) break;
            else
            if (*(lvl->map + player->x + 1 + player->y*(lvl->w+1))!=wall_c) {
                player->x=player->x+1;
                change_pos(player_c, void_c, player->x-1+player->y*(lvl->w+1), player->x+player->y*(lvl->w+1), &lvl);
            }
        }
        //ghost physics
        if (*(lvl->map + ghost->x + ghost->velocity + ghost->y*(lvl->w+1))!=wall_c) 
            if(*(lvl->map + ghost->x + ghost->velocity + ghost->y*(lvl->w+1))==player_c) break; 
            else {
                ghost->x += ghost->velocity;
                change_pos(ghost_c, void_c, ghost->x-ghost->velocity+ghost->y*(lvl->w+1), ghost->x+ghost->y*(lvl->w+1));
            }
        else ghost->velocity *= -1;

        printf(lvl->map);
        usleep(33333);
        setCursorPosition(0,0);
    }
}

int main() {
    char *header = "level1.txt";
    struct object player = {.x=-1, .y=-1, .character = player_c};
    struct object magic_stone = {.x=-1, .y=-1, .character = magic_stone_c};
    struct object ghost = {.x=-1, .y=-1, .character = ghost_c, .velocity = 1};
    struct lvl_data lvl = {.w=-1, .h=-1, .ghosts_evaded = 0, .magic_stones = 0, .number = 1};
    load_level_by_number(lvl.number, &player, &ghost, &magic_stone, &lvl);
    game_loop(&player, &ghost, &magic_stone, &lvl);

    free(lvl.map);
    printf("END GAME...");
    sleep(1);
    return 0;
}