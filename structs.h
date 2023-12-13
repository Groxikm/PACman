struct object {
    int x,y;
    int power;
};

struct wall {
    int x,y;
};

struct door {
    int x,y;
    //boolean is_open;
};

struct magic_stone {
    int x,y;
    int state;
};

struct magic_button {
    int x,y;
    //boolean is_on;
};

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