struct player {
    int x,y;
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

struct render_matrix {
    char* matrix;
    const int w;
    const int h;
};