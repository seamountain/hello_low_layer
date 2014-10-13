#include <iostream>

using namespace std;

//int main() {
//    cout << "Hello, World!" << endl;
//    return 0;
//}

#include <unistd.h>
#include "libs/lua-5.2.3/include/lua.hpp"
#include "libs/SDL2-2.0.3/include/SDL.h"

int TARGET_FPS = 60;
// milli seconds per frame
int TARGET_FRAME_TIME = 1000 / TARGET_FPS;

SDL_Window* w;
SDL_Renderer* render;
SDL_GLContext context;
int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 640;

int frame_count = 0;

void Update(lua_State *l) {
    if (frame_count == TARGET_FPS) {
        // http://www.c-lang.net/clock
        frame_count = 0;
    } else {
        frame_count++;
    }

    lua_getglobal(l, "add_data");

    if (lua_pcall(l, 0, 1, 0)) {
        printf("error 2: %s\n", lua_tostring(l, -1));
    }
}

void parse_pushed_data(SDL_Rect* rect, const char* data) {
// Data example "{ x: 0, y: 0, w: 100, h: 80 }"
// Actually -> "0 0 100 80"

    //printf("data %s\n", data);
    char char_data[strlen(data)];
    strcpy(char_data, data);

    char *tp = strtok(char_data, " ");
    int target = 0;
    int splited_data[4];
    // split data from Lua
    while (tp != NULL) {
        splited_data[target] = atoi(tp);
        tp = strtok(NULL, " ");
        target++;
    }

    rect->x = splited_data[0];
    rect->y = splited_data[1];
    rect->w = splited_data[2];
    rect->h = splited_data[3];
}

void get_pushed_data(lua_State *l, SDL_Rect* rects, int size) {
    for (int i = 0; i < size; i++) {
        // get most bottm data
        int index = 1;
        const char* data = lua_tostring(l, index);
        lua_remove(l, index);

        SDL_Rect rect;
        parse_pushed_data(&rect, data);
        rects[i] = rect;
    }
}

void get_random_color(int color[]) {
    int size = 4;
    int MAX_COLOR = 256;

    // http://wisdom.sakura.ne.jp/programming/c/c61.html
    for (int i = 0; i < size; i++) {
        color[i] = (i < size - 1) ? (rand() % (MAX_COLOR + 1)) : 256;
    }
}

void Draw(lua_State *l) {
    // clear
    SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
    SDL_RenderClear(render);

    // this program don't draw with no data
    int stack_size = lua_gettop(l);
    //printf("stack_size %i\n", stack_size);
    if (stack_size < 1) {
        return;
    }

    SDL_Rect rects[stack_size];
    get_pushed_data(l, rects, stack_size);

    int color[4];
    get_random_color(color);
    SDL_SetRenderDrawColor(render, color[0], color[1], color[2], color[3]);

    SDL_RenderFillRects(render, rects, stack_size);
    SDL_RenderPresent(render);
}

// REFER TO http://nyaocat.hatenablog.jp/entry/2014/01/27/153145
bool init(lua_State *l) {
    if (luaL_dofile(l, "./lua/update.lua")) {
        printf("error 1: %s\n", lua_tostring(l, -1));
        return false;
    }

    if (luaL_dofile(l, "./lua/draw.lua")) {
        printf("error 3: %s\n", lua_tostring(l, -1));
        return false;
    }

    // initialize SDL
    if( SDL_Init(SDL_INIT_VIDEO) < 0 ) return false;

    // initialize window
    w = SDL_CreateWindow(
            "SDL window test",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH, SCREEN_HEIGHT,
            SDL_WINDOW_OPENGL
    );
    context = SDL_GL_CreateContext(w);

    render = SDL_CreateRenderer(w, -1, 0);

    return true;
}

bool pollingEvent()
{
    SDL_Event ev;
    SDL_Keycode key;
    while ( SDL_PollEvent(&ev) )
    {
        switch(ev.type){
            case SDL_QUIT:
                // raise when exit event is occur
                return false;
                break;
            case SDL_KEYDOWN:
                // raise when key down
            {
                key = ev.key.keysym.sym;
                // ESC
                if(key == SDLK_ESCAPE){
                    return false;
                }
            }
                break;
        }
    }
    return true;
}

bool dealloc() {
    // finalize SDL
    SDL_Quit();

    return true;
}

int main(int argc, char* argv[])
{
    lua_State *l = luaL_newstate();
    luaL_openlibs(l);

    if (!init(l)) {
        return 1;
    };

    // initialize rand
    srand((unsigned)time(NULL));

    while (true) {
        // check event
        if (!pollingEvent()) break;

        clock_t begin = clock();

        // GameLoop
        Update(l);
        Draw(l);

        // wait
        clock_t end = clock();

        // spent milli seconds
        double diff = (double)(end - begin) / CLOCKS_PER_SEC * 1000;

        // wait
        usleep(TARGET_FRAME_TIME - diff - 1);
        // advanced: wait strictly by switching
        while (TARGET_FRAME_TIME >= ((double)(clock() - begin) / CLOCKS_PER_SEC * 1000)) {
            usleep(0);
        }
    }

    dealloc();

    return 0;
}
