#include <iostream>
#include <unistd.h>
#include <vector>
#include "Data.cpp"
#include "libs/lua-5.2.3/include/lua.hpp"
#include "libs/lua-5.2.3/include/lualib.h"
#include "libs/SDL2-2.0.3/include/SDL.h"

using namespace std;

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
}

vector<Data*> data_list;

int registerData(lua_State* l) {
    int x = luaL_checkint(l, -4);
    int y = luaL_checkint(l, -3);
    int width = luaL_checkint(l, -2);
    int height = luaL_checkint(l, -1);

    Data *data = new Data(x, y, width, height);
    data_list.push_back(data);

    return 0; // 戻り値は無し
}

void Draw(lua_State *l) {
    // clear
    SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
    SDL_RenderClear(render);

    int color[4] = {100, 200, 120, 256};
    SDL_SetRenderDrawColor(render, color[0], color[1], color[2], color[3]);

    int data_size = data_list.size();
    SDL_Rect rects[data_size];
    for (int i = 0; i < data_size; i++) {
        SDL_Rect r;
        r.x = data_list[i]->getX();
        r.y = data_list[i]->getY();
        r.w = data_list[i]->getWidth();
        r.h = data_list[i]->getHeight();
        rects[i] = r;
    }
    SDL_RenderFillRects(render, rects, data_size);
    SDL_RenderPresent(render);
}

// REFER TO http://nyaocat.hatenablog.jp/entry/2014/01/27/153145
bool init(lua_State *l) {
    // TODO fix path
    string path = "/Users/sea_mountain/work/github/hello_low_layer/lua/samples/simple_game_loop/";

    string updateScript = path + "lua/update.lua";
    if (luaL_dofile(l, updateScript.c_str())) {
        printf("error 1: %s\n", lua_tostring(l, -1));
        return false;
    }

    string drawScript = path + "lua/update.lua";
    if (luaL_dofile(l, drawScript.c_str())) {
        printf("error 3: %s\n", lua_tostring(l, -1));
        return false;
    }

    lua_register(l, "registerData", registerData);

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

void call_lua(lua_State *l, int x, int y) {
    lua_getglobal(l, "add_data");
    lua_pushnumber(l, x);
    lua_pushnumber(l, y);

    if (lua_pcall(l, 2, 0, 0)) {
        printf("error: %s\n", lua_tostring(l, -1));
    }
}

bool isMoving = false;
bool pollingEvent(lua_State *l)
{
    SDL_Event ev;
    SDL_Keycode key;
    while ( SDL_PollEvent(&ev) )
    {
        switch(ev.type){
            case SDL_QUIT: // raise when exit event is occur
                return false;
            case SDL_KEYDOWN: // raise when key down
                key = ev.key.keysym.sym;
                // ESC
                if(key == SDLK_ESCAPE){
                    return false;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                isMoving = true;
                break;
            case SDL_MOUSEBUTTONUP:
                isMoving = false;
                break;
            case SDL_MOUSEMOTION:
                if (isMoving) {
                    call_lua(l, ev.button.x, ev.button.y);
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
        if (!pollingEvent(l)) break;

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
    lua_close(l);

    return 0;
}