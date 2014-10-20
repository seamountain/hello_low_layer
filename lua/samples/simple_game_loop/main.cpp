#include <iostream>
#include <unistd.h>
#include <vector>
#include "Color.cpp"
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

Palette current_palette = Palette::Black;

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

    Color* color = new Color(current_palette);
    Data* data = new Data(x, y, width, height, color);
    data_list.push_back(data);

    return 0; // 戻り値は無し
}

void setup_palette_button() {
    Color* red = new Color(Palette::Red);
    Color* green = new Color(Palette::Green);
    Color* blue = new Color(Palette::Blue);
    Color* black = new Color(Palette::Black);
    Color* white = new Color(Palette::White);
    vector<Color*> palettes = {red, green, blue, black, white};

    int margin = 10;
    int width = 50;
    int height = 50;
    for (int i = 0; i < (int)Palette::SIZE; i++) {
        SDL_Rect r;
        r.x = margin;
        r.y = height * i + margin * (i + 1);
        r.w = width;
        r.h = height;

//        printf("r %i, g %i, b %i, a %i\n", palettes[i]->r, palettes[i]->g, palettes[i]->b, palettes[i]->a);
        if (SDL_SetRenderDrawColor(render, palettes[i]->r, palettes[i]->g, palettes[i]->b, palettes[i]->a) < 0) {
            printf("SDL Error %s\n", SDL_GetError());
        }
        SDL_RenderFillRect(render, &r);
    }
}

void Draw(lua_State *l) {
    // clear
    SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
    SDL_RenderClear(render);

    int data_size = data_list.size();
    for (int i = 0; i < data_size; i++) {
        SDL_Rect r;
        r.x = data_list[i]->getX();
        r.y = data_list[i]->getY();
        r.w = data_list[i]->getWidth();
        r.h = data_list[i]->getHeight();

        Color* color = data_list[i]->getColor();
        SDL_SetRenderDrawColor(render, color->r, color->g, color->b, color->a);
        SDL_RenderFillRect(render, &r);
    }

    setup_palette_button();

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

    setup_palette_button();

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

void select_palette(int x, int y) {

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
                } else {
                    select_palette(ev.button.x, ev.button.y);
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