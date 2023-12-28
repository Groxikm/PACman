struct object {
    int x,y;
    int power, velocity;
    char character;
};

const char wall_c = '#';
const char player_c = 'A';
const char void_c = ' ';
const char magic_stone_c = 'O';
const char ghost_c = '@';

#define File_name_limit 30
#define ghost_limit 8
#define step 1

struct lvl_data{
    int w,h, number, ghosts_evaded, magic_stones;
    char *map;
    struct object ghost_array[ghost_limit];
};