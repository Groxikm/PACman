#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <unistd.h>
#include <windows.h>
#include <math.h>

#include "structs.h"

const int box_size_width = 100;
const int box_size_height = 30;
const int max_level_len = 1000;

const char wall_c = '#';
const char player_c = 'A';
const char void_c = ' ';
const char magic_stone_c = 'O';
const char magic_button = 'n';

int ghosts_ivaded = 0;
int magic_stones = 0;
int level_w = 0;
int level_h = 0;

char *level_array;
struct player p = {.x=-1, .y=-1};
struct player ms = {.x=-1, .y=-1};

int length_of_file(char *level_file_name) {
    int length=0;
    char ch;
    FILE *fptr = fopen(level_file_name, "r");

    while ((ch=fgetc(fptr))!=EOF) {
        length++;
    }

    fclose(fptr);

    return length;
}

int width_of_file(char *level_file_name) {
    int length=0;
    char ch;
    FILE *fptr = fopen(level_file_name, "r");

    while ((ch=fgetc(fptr))!='\n') {
        length++;
    }

    fclose(fptr);

    return length;
}

int height_of_file(char *level_file_name) {
    int length=0;
    char ch;
    FILE *fptr = fopen(level_file_name, "r");

    while ((ch=fgetc(fptr))!=EOF) {
        if (ch=='\n') length++;
    }
    length++;

    fclose(fptr);

    return length;
}

void find_pos_of_player() {
    for (int i = 0; i < level_h; i++) {
        for (int j = 0; j < level_w+1; j++) {
            if (*(level_array+i*(level_w+1)+j)==player_c) { // level_w+1 - because '\n' symbol
                p.x=j;
                p.y=i;
            }
        }
    }
}

void find_pos_of_magic_stone() {
    for (int i = 0; i < level_h; i++) {
        for (int j = 0; j < level_w+1; j++) {
            if (*(level_array+i*(level_w+1)+j)==magic_stone_c) { // level_w+1 - because '\n' symbol
                ms.x=j;
                ms.y=i;
            }
        }
    }
}

void game_render() {
    system("cls");
    //printf("TRAVELLER GAME: \n inventory: {magic stones: %d} \n", magic_stones);
    //printf("p_x: %d, p_y: %d \n", p.x, p.y);
    printf(level_array);
}

void physics_loop() {
    // person
}

void change_pos(char figure_type, char change_type, int start, int end) {
    *(level_array+start) = change_type;
    *(level_array+end) = figure_type;
}

void load_level(char *level_file_name, char *array) {
    FILE *fptr = fopen(level_file_name, "r");

    //level_h = height_of_file("level1.txt");
    //level_w = width_of_file("level1.txt");
    //array = (char*) malloc(level_h*(level_w+1)*sizeof(char)+1);

    int i=0;
    char ch;
    while ((ch=fgetc(fptr))!=EOF) {
        if (ch==wall_c || ch==player_c || ch==void_c || ch==magic_stone_c || ch==magic_button || ch=='\n') {
            *(array+i) = ch;
        }
        else {
            *(array+i) = void_c;
        }
        i++;
    }
    *(array+i)='\0';

    // close the file
    fclose(fptr);

    //find_pos_of_player();
}

void load_level1() {
    level_h = height_of_file("level1.txt");
    level_w = width_of_file("level1.txt");
    level_array = (char*) malloc(level_h*(level_w+1)*sizeof(char)+1);
    load_level("level1.txt", level_array);

    find_pos_of_player();
    find_pos_of_magic_stone();
    //change_pos(player_c, void_c, p.x+(p.y)*(level_w+1), p.x+(p.y+1)*(level_w+1)); // go 1 step down
}

void load_level2() {
    free(level_array);
    level_h = height_of_file("level2.txt");
    level_w = width_of_file("level2.txt");
    level_array = (char*) malloc(level_h*(level_w+1)*sizeof(char)+1);
    load_level("level2.txt", level_array);

    find_pos_of_player();
    find_pos_of_magic_stone();
    //change_pos(player_c, void_c, p.x+(p.y)*(level_w+1), p.x+(p.y+1)*(level_w+1)); // go 1 step down
}

void game_loop() {
    while (!GetAsyncKeyState(VK_ESCAPE)) {
        if (GetAsyncKeyState(VK_UP) & 1) {
            if (*(level_array+p.x+(p.y-1)*(level_w+1))==magic_stone_c) load_level2();
            else
            if (*(level_array+p.x+(p.y-1)*(level_w+1))!=wall_c) {
                p.y=p.y-1;
                change_pos(player_c, void_c, p.x+(p.y+1)*(level_w+1), p.x+p.y*(level_w+1));
            }
        }
        if (GetAsyncKeyState(VK_DOWN) & 1) {
            if (*(level_array+p.x+(p.y+1)*(level_w+1))==magic_stone_c) load_level2();
            else
            if (*(level_array+p.x+(p.y+1)*(level_w+1))!=wall_c) {
                p.y=p.y+1;
                change_pos(player_c, void_c, p.x+(p.y-1)*(level_w+1), p.x+p.y*(level_w+1));
            }
        }
        if (GetAsyncKeyState(VK_LEFT) & 1) {
            if (*(level_array+p.x-1+p.y*(level_w+1))==magic_stone_c) load_level2();
            else
            if (*(level_array+p.x-1+p.y*(level_w+1))!=wall_c) {
                p.x=p.x-1;
                change_pos(player_c, void_c, p.x+1+p.y*(level_w+1), p.x+p.y*(level_w+1));
            }
        }
        if (GetAsyncKeyState(VK_RIGHT) & 1) {
            if (*(level_array+p.x+1+p.y*(level_w+1))==magic_stone_c) load_level2();
            else
            if (*(level_array+p.x+1+p.y*(level_w+1))!=wall_c) {
                p.x=p.x+1;
                change_pos(player_c, void_c, p.x-1+p.y*(level_w+1), p.x+p.y*(level_w+1));
            }
        }

        game_render();

        sleep(0.9);
    }
}

int main() {

    // load level 1
    char *header = "level1.txt";
    load_level1();

    game_loop();
    free(level_array);
    sleep(1);

    return 0;
}
