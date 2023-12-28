#define File_name_limit 30
#define ghost_limit 8
#define step 1
#define max_level_number 3

const char wall_c = '#';
const char player_c = 'A';
const char void_c = ' ';
const char magic_stone_c = 'O';
const char ghost_c = '@';

struct object {
    int x,y;
    int power, velocity;
    char character;
};

struct lvl_data{
    int w,h, number, ghosts_amount, magic_stones;
    char *map;
    struct object ghost_array[ghost_limit];
};

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