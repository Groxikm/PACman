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
const char player_c = 'P';
const char void_c = ' ';
const char magic_stone = 'O';
const char magic_button = 'n';

int ghosts_evaded = 0;
int magic_stones = 0;
int level_w = 0;
int level_h = 0;

char *level_array;
struct player p = {.x=-1, .y=-1};

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

void load_level(char *level_file_name, char *array) {
    FILE *fptr = fopen(level_file_name, "r");

    int i=0;
    char ch;
    while ((ch=fgetc(fptr))!=EOF) {
        if (ch==wall_c || ch==player_c || ch==void_c || ch==magic_stone || ch==magic_button || ch=='\n') {
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
}

void find_pos_of_player(struct player *p) {
    for (int i = 0; i < level_h; i++) {
        for (int j = 0; j < level_w; j++) {
            if (*(level_array+i*(level_w+1)+j)==player_c) { // level_w+1 - because '\n' symbol
                p->x=j;
                p->y=i;
            }
        }
    }
}

void game_render() {
    system("cls");
    //printf("TRAVELLER GAME: \n inventory: {magic stones: %d} \n", magic_stones);
    printf(level_array);
}

void physics_loop() {
    // person
}

void change_pos(char figure_type, char change_type, int start, int end) {
    *(level_array+start) = change_type;
    *(level_array+end) = figure_type;
}

void load_level1(char* level_array_ptr) {
    level_h = height_of_file("level1.txt");
    level_w = width_of_file("level1.txt");
    char level_array_content[level_h*level_w+1]; // +1 for special '\0' null character
    level_array_ptr = level_array_content;
    load_level("level1.txt", level_array_ptr);
    find_pos_of_player(&p);

    //change_pos(player_c, void_c, p.x+(p.y-1)*(level_w+1), p.x+p.y*(level_w+1));
    // *(level_array+4)=' ';
    printf("h = %d , w = %d\n", level_h, level_w);
    printf(level_array_ptr);
    
}

void game_loop() {
    while (!(GetAsyncKeyState(VK_ESCAPE) & 0x8000)) {
        if (GetAsyncKeyState(VK_UP) & 1) {
            if (*(level_array+p.x+(p.y-1)*(level_w+1))!=wall_c) {
                p.y=p.y-1;
                //change_pos(player_c, void_c, p.x+(p.y+1)*(level_w+1), p.x+p.y*(level_w+1));
            }
        }
        if (GetAsyncKeyState(VK_DOWN) & 1) {
            if (*(level_array+p.x+(p.y+1)*(level_w+1))!=wall_c) {
                p.y=p.y+1;
                //change_pos(player_c, void_c, p.x+(p.y-1)*(level_w+1), p.x+p.y*(level_w+1));
            }
        }
        if (GetAsyncKeyState(VK_LEFT) & 1) {
            if (*(level_array+p.x-1+p.y*(level_w+1))!=wall_c) {
                p.x=p.x-1;
                //change_pos(player_c, void_c, p.x+1+p.y*(level_w+1), p.x+p.y*(level_w+1));
            }
        }
        if (GetAsyncKeyState(VK_RIGHT) & 1) {
            if (*(level_array+p.x+1+p.y*(level_w+1))!=wall_c) {
                p.x=p.x+1;
                //change_pos(player_c, void_c, p.x-1+p.y*(level_w+1), p.x+p.y*(level_w+1));
            }
        }
        fflush(stdout);
        system("cls");
        printf("p_x: %d, p_y: %d \n", p.x, p.y);
        //printf(level_array);

        //game_render();

        sleep(0.7);
    }
}

int main() {
    //load_level1(level_array);
    load_level("level1.txt", level_array);
    //sleep(2);
    printf("%s", level_array);
    printf("\n\nThis is the level\n\n");
    sleep(2);

    game_loop();
    
    fflush(stdout);
    getch();
    printf("\nEND GAME\n");
    sleep(2);
    return 0;
}