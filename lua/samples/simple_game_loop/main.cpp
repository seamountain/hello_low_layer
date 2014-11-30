#include <iostream>
#include <unistd.h>
#include <vector>
#include "libs/SDL2-2.0.3/include/SDL.h"
#include "libs/lua-5.2.3/include/lua.hpp"
#include "Color.cpp"
#include "Data.cpp"

using namespace std;

int TARGET_FPS = 60;
// milli seconds per frame
int TARGET_FRAME_TIME = 1000 / TARGET_FPS;

SDL_Window* w;
SDL_Renderer* render;
SDL_GLContext context;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 640;

int frame_count = 0;

// TODO ヘッダファイル作成
#include "lua_glue_code.cpp"

Color* red = new Color(Palette::Red);
Color* green = new Color(Palette::Green);
Color* blue = new Color(Palette::Blue);
Color* black = new Color(Palette::Black);
Color* white = new Color(Palette::White);
vector<Color*> palettes = {red, green, blue, black, white};
Palette current_palette = Palette::Black;

vector<SDL_Rect*> palette_buttons_rect;

vector<Data*> drawing_data_list;

void call_lua(lua_State *l, Data* data) {
    target_data = data;

    lua_getglobal(l, "move_data_pos");
    lua_pushnumber(l, SCREEN_WIDTH);
    lua_pushnumber(l, SCREEN_HEIGHT);

    if (lua_pcall(l, 2, 0, 0)) {
        printf("call_lua error: %s\n", lua_tostring(l, -1));
    }
}

void Update(lua_State *l) {
    // http://www.c-lang.net/clock
    if (frame_count == TARGET_FPS) {
        frame_count = 0;
    } else {
        frame_count++;
    }

    for (int i = 0; i < drawing_data_list.size(); i++) {
        // TODO オブジェクト数だけ毎フレームにLuaを読んでいるが直したほうがいいのでは
        call_lua(l, drawing_data_list[i]);
    }
}

void register_drawing_data(int x, int y) {
    int brash_size = 10;
    Color* color = new Color(current_palette);
    Data* data = new Data(x, y, brash_size, brash_size, color);
    drawing_data_list.push_back(data);
}

void init_palette_button() {
    int margin = 10;
    int width = 50;
    int height = 50;
    for (int i = 0; i < (int)Palette::SIZE; i++) {
        SDL_Rect* r = new SDL_Rect();
        r->x = margin;
        r->y = height * i + margin * (i + 1);
        r->w = width;
        r->h = height;
        palette_buttons_rect.push_back(r);
    }
}

void draw_palette_button() {
    for (int i = 0; i < (int)Palette::SIZE; i++) {
        if (SDL_SetRenderDrawColor(render, palettes[i]->r, palettes[i]->g, palettes[i]->b, palettes[i]->a) < 0) {
            printf("SDL Error %s\n", SDL_GetError());
        }

        SDL_RenderFillRect(render, palette_buttons_rect[i]);
    }
}

void Draw() {
    // clear
    SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
    SDL_RenderClear(render);

    int data_size = drawing_data_list.size();
    for (int i = 0; i < data_size; i++) {
        SDL_Rect* r = new SDL_Rect();
        r->x = drawing_data_list[i]->getX();
        r->y = drawing_data_list[i]->getY();
        r->w = drawing_data_list[i]->getWidth();
        r->h = drawing_data_list[i]->getHeight();

//        printf("i: %i,  x %i, y %i, w %i, h %i\n", i, r->x, r->y, r->w, r->h);

        Color* color = drawing_data_list[i]->getColor();
        SDL_SetRenderDrawColor(render, color->r, color->g, color->b, color->a);
        // TODO use RenderFillRects with same color rects
        SDL_RenderFillRect(render, r);
    }

    draw_palette_button();

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

    init_palette_button();

    lua_register(l, "get_target_data", get_target_data);

    return true;
}

void change_selected_color(int palette_index) {
    current_palette = (Palette)palette_index;
}

bool select_palette(int x, int y) {
    SDL_Point p = {x, y};
    for (int i = 0; i < (int)Palette::SIZE; i++) {
        SDL_Rect* palette_rect = palette_buttons_rect[i];
        // 当たり判定
        if ((palette_rect->x <= p.x && p.x <= palette_rect->x + palette_rect->w) &&
                (palette_rect->y <= p.y && p.y <= palette_rect->y + palette_rect-> h))
        {
            change_selected_color(i);
            return true;
        }
    }
    return false;
}

bool pollingEvent()
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
                if (!select_palette(ev.button.x, ev.button.y)) {
                    register_drawing_data(ev.button.x, ev.button.y);
                }
                break;
            default:
                break;
        }
    }
    return true;
}

bool dealloc() {
    // finalize SDL
    SDL_Quit();

    // Delete palettes color
    delete red;
    delete green;
    delete blue;
    delete black;
    delete white;

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
        Draw();

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
