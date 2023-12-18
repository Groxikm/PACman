#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <unistd.h>
#include <windows.h>
#include <math.h>

#include "structs.h"

struct object p = {.x=-1, .y=-1, .character = player_c};
struct object ms = {.x=-1, .y=-1, .character = magic_stone_c};
struct object ghost = {.x=-1, .y=-1, .character = ghost_c, .velocity = 1};
struct lvl_data lvl = {.w=-1, .h=-1, .ghosts_evaded = 0, .magic_stones = 0};

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

void find_pos_of_(struct object *obj) {
    for (int i = 0; i < lvl.h; i++) 
        for (int j = 0; j < lvl.w+1; j++) 
            if (*(lvl.map+i*(lvl.w+1)+j)== obj->character) { // lvl.w+1 - because '\n' symbol
                obj->x=j;
                obj->y=i;
            }
}

void change_pos(char figure_type, char change_type, int start, int end) {
    *(lvl.map+start) = change_type;
    *(lvl.map+end) = figure_type;
}

void load_level(char *level_file_name, char *array) {
    FILE *fptr = fopen(level_file_name, "r");

    int i=0;
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

void load_level1() {
    lvl.h = height_of_file("level1.txt");
    lvl.w = width_of_file("level1.txt");
    lvl.map = (char*) malloc(lvl.h*(lvl.w+1)*sizeof(char));
    load_level("level1.txt", lvl.map);

    find_pos_of_(&ghost);
    find_pos_of_(&p);
    find_pos_of_(&ms);
    
}

void load_level2() {
    free(lvl.map);
    lvl.h = height_of_file("level2.txt");
    lvl.w = width_of_file("level2.txt");
    lvl.map = (char*) malloc(lvl.h*(lvl.w+1)*sizeof(char));
    load_level("level2.txt", lvl.map);

    find_pos_of_(&p);
    find_pos_of_(&ms);
    find_pos_of_(&ghost);
}

void game_loop() {
    while (!GetAsyncKeyState(VK_ESCAPE)) {
        if (GetAsyncKeyState(VK_UP) & 1) {
            if (*(lvl.map+p.x+(p.y-1)*(lvl.w+1))==magic_stone_c) load_level2();
            else
            if (*(lvl.map+p.x+(p.y-1)*(lvl.w+1))==ghost_c) break;
            else
            if (*(lvl.map+p.x+(p.y-1)*(lvl.w+1))!=wall_c) {
                p.y=p.y-1;
                change_pos(player_c, void_c, p.x+(p.y+1)*(lvl.w+1), p.x+p.y*(lvl.w+1));
            }
        }
        if (GetAsyncKeyState(VK_DOWN) & 1) {
            if (*(lvl.map+p.x+(p.y+1)*(lvl.w+1))==magic_stone_c) load_level2();
            else
            if (*(lvl.map+p.x+(p.y+1)*(lvl.w+1))==ghost_c) break;
            else
            if (*(lvl.map+p.x+(p.y+1)*(lvl.w+1))!=wall_c) {
                p.y=p.y+1;
                change_pos(player_c, void_c, p.x+(p.y-1)*(lvl.w+1), p.x+p.y*(lvl.w+1));
            }
        }
        if (GetAsyncKeyState(VK_LEFT) & 1) {
            if (*(lvl.map+p.x-1+p.y*(lvl.w+1))==magic_stone_c) load_level2();
            else
            if (*(lvl.map+p.x-1+p.y*(lvl.w+1))==ghost_c) break;
            else
            if (*(lvl.map+p.x-1+p.y*(lvl.w+1))!=wall_c) {
                p.x=p.x-1;
                change_pos(player_c, void_c, p.x+1+p.y*(lvl.w+1), p.x+p.y*(lvl.w+1));
            }
        }
        if (GetAsyncKeyState(VK_RIGHT) & 1) {
            if (*(lvl.map+p.x+1+p.y*(lvl.w+1))==magic_stone_c) load_level2();
            else
            if (*(lvl.map+p.x+1+p.y*(lvl.w+1))==ghost_c) break;
            else
            if (*(lvl.map + p.x + 1 + p.y*(lvl.w+1))!=wall_c) {
                p.x=p.x+1;
                change_pos(player_c, void_c, p.x-1+p.y*(lvl.w+1), p.x+p.y*(lvl.w+1));
            }
        }
        //ghost physics
        if (*(lvl.map + ghost.x + ghost.velocity + ghost.y*(lvl.w+1))!=wall_c) 
            if(*(lvl.map + ghost.x + ghost.velocity + ghost.y*(lvl.w+1))==player_c) break; 
            else {
                ghost.x += ghost.velocity;
                change_pos(ghost_c, void_c, ghost.x-ghost.velocity+ghost.y*(lvl.w+1), ghost.x+ghost.y*(lvl.w+1));
            }
        else ghost.velocity *= -1;


        printf(lvl.map);
        usleep(33333);
        setCursorPosition(0,0);
    }
}

int main() {
    char *header = "level1.txt";
    load_level1();
    game_loop();

    free(lvl.map);
    printf("END GAME...");
    sleep(1);
    return 0;
}