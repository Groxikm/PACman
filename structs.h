struct object {
    int x,y;
    int power, velocity;
    char character;
};

struct lvl_data{
    int w,h, number, ghosts_evaded, magic_stones;
    char *map;
};

const char wall_c = '#';
const char player_c = 'A';
const char void_c = ' ';
const char magic_stone_c = 'O';
const char ghost_c = '@';

const unsigned int File_name_limit = 30;