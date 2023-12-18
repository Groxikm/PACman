struct object {
    int x,y;
    int power, velocity;
    char character;
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
const char ghost_c = '@';

struct lvl_data{
    int w,h, number, ghosts_evaded, magic_stones;
    char *map;
};

const unsigned int File_name_limit = 30;