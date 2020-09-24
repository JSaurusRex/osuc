#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "raylib.h"

#define MAX_TEXTURES 1000
#define MAX_ANIMATIONS 100

#define MAX_ENEMYS 20
char * load_file (char * file);
int lengthFile = 0;


//put all the global structs in here

int screenx, screeny; //screenresolutions

//0 game, 1 overworld
int game_state = 1;
bool paused = false;

//0 up, 1 down, 2 left, 3 right, 4 jump, 5 run, 6 pause
bool keys[5];

float delta;
int millitotal, millitimer;

typedef struct{ //vector
    float x, y;

} vec;

vec vec_create (float x, float y)
{
    vec new_vector = {.x = x, .y = y};
    return new_vector;
}

vec vec_add (vec one, vec two)
{
    one.x += two.x;
    one.y += two.y;
    return one;
}

vec vec_addf (vec one, float two)
{
    one.x += two;
    one.y += two;
    return one;
}

vec vec_min (vec one, vec two)
{
    one.x -= two.x;
    one.y -= two.y;
    return one;
}

vec vec_multi (vec one, vec two)
{
    one.x *= two.x;
    one.y *= two.y;
    return one;
}
vec vec_multif (vec one, float two)
{
    one.x *= two;
    one.y *= two;
    return one;
}

vec vec_dev (vec one, vec two)
{
    one.x /= two.x;
    one.y /= two.y;
    return one;
}

vec vec_devf (vec one, float two)
{
    one.x /= two;
    one.y /= two;
    return one;
}

typedef struct{ //vector 4 (colors)
    union{
        struct { float x, y, z, w};
        struct { float r, b, g, a};
        float v[4];
    };
}vec4;

vec4 vec4_create (float r, float g, float b, float a)
{
    vec4 new_vector4 = {.r = r, .g = g, .b = b, .a = a};
    return new_vector4;
}

typedef struct //notes
{
    int position, timing;
    int longTime;
}Note;
