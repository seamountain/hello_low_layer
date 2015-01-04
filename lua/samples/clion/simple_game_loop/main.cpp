#include <iostream>
#include <unistd.h>
#include <vector>
#include <deque>
#include "libs/SDL2-2.0.3/include/SDL.h"
#include "libs/lua-5.2.3/include/lua.hpp"
#include "Color.h"

using namespace std;

// TODO fix path
string path = "/Users/sea_mountain/work/github/hello_low_layer/lua/samples/clion/simple_game_loop/";

int TARGET_FPS = 60;
// milli seconds per frame
int TARGET_FRAME_TIME = 1000 / TARGET_FPS;

lua_State* l;
SDL_Window* w;
SDL_Renderer* render;
SDL_GLContext context;
SDL_Texture *texture;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 640;

const int ASCII_CODE_ZERO = 48;

int frame_count = 0;

Color* red = new Color(Palette::Red);
Color* green = new Color(Palette::Green);
Color* blue = new Color(Palette::Blue);
Color* black = new Color(Palette::Black);
Color* white = new Color(Palette::White);
vector<Color*> palettes = {red, green, blue, black, white};
Palette current_palette = Palette::Black;

vector<SDL_Rect*> palette_buttons_rect;

void Update() {
    // http://www.c-lang.net/clock
    if (frame_count == TARGET_FPS) {
        frame_count = 0;
    } else {
        frame_count++;
    }

    lua_getglobal(l, "move_data");

    if (lua_pcall(l, 0, 0, 0)) {
        printf("call_lua error: %s\n", lua_tostring(l, -1));
    }

    // TODO Add debug mode
//    printf("Update gettop %i\n", lua_gettop(l));
}

void register_drawing_data(int x, int y) {
    lua_getglobal(l, "register_data");
    lua_pushnumber(l, x);
    lua_pushnumber(l, y);
    lua_pushnumber(l, (int)current_palette);

    if (lua_pcall(l, 3, 0, 0)) {
        printf("call_lua error: %s\n", lua_tostring(l, -1));
    }
}

void init_num_texture() {
    string image_path = path + "images/font.bmp";
    SDL_Surface *bmp = SDL_LoadBMP(image_path.c_str());
  if (!bmp) {
    printf("bmp is null. Error: %s\n", SDL_GetError());
    return;
  }

  texture = SDL_CreateTextureFromSurface(render, bmp);

  SDL_FreeSurface(bmp);
  if (texture == nullptr){
    std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
  }
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

// REFER TO http://www5.big.or.jp/~high/VENIO/kuz/kuz_are_14.htm
void draw_num_label(int num) {
  string label = to_string(num);
  SDL_Rect src,drw;
  int text_size = 32;
  int margin = 10;

  src.y = 0;
  src.w = text_size;
  src.h = text_size;
  drw.w = text_size;
  drw.h = text_size;
  int i = 0;
  while (i < label.length()) {
    src.x = (label[i] - ASCII_CODE_ZERO) * text_size; // set num texture position
    drw.x = margin + i * text_size;
    drw.y = SCREEN_HEIGHT - text_size;

    SDL_RenderCopy(render, texture, &src, &drw); // Copy the texture into render

    i++;
  }
}

void Draw() {
    // clear
    SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
    SDL_RenderClear(render);

    lua_getglobal(l, "data_num");
    if (lua_pcall(l, 0, 1, 0)) {
        printf("call_lua error: %s\n", lua_tostring(l, -1));
    }
    int data_num = (int) lua_tonumber(l, -1);

    if (data_num != 0) {
        lua_getglobal(l, "data_list");
        lua_pushnil(l);
        while (lua_next(l, -2) != 0) {
            lua_pushnil(l);
            SDL_Rect *r = new SDL_Rect();
            while (lua_next(l, -2) != 0) {
//            printf("%s %s\n", lua_tostring(l, -2), lua_tostring(l, -1));
                string key = lua_tostring(l, -2);
                if (key == "x") {
                    r->x = (int) lua_tointeger(l, -1);
                } else if (key == "y") {
                    r->y = (int) lua_tointeger(l, -1);
                } else if (key == "w") {
                    r->w = (int) lua_tointeger(l, -1);
                } else if (key == "h") {
                    r->h = (int) lua_tointeger(l, -1);
                } else if (key == "color_id") {
                    int colorId = (int) lua_tointeger(l, -1);
                    Color *color = new Color((Palette) colorId);
                    SDL_SetRenderDrawColor(render, color->r, color->g, color->b, color->a);
                }
                lua_pop(l, 1);
            }
            // TODO Use RenderFillRects
            SDL_RenderFillRect(render, r);
            lua_pop(l, 1);
        }
    }
    lua_settop(l, 0);

    draw_palette_button();
    draw_num_label(data_num);

    SDL_RenderPresent(render);

//    printf("Render gettop %i\n", lua_gettop(l));
}

void lua_init() {
    string updateScript = path + "lua/update.lua";
    if (luaL_dofile(l, updateScript.c_str())) {
        printf("error 1: %s\n", lua_tostring(l, -1));
    }

    string dataScript = path + "lua/data.lua";
    if (luaL_dofile(l, dataScript.c_str())) {
        printf("error 1: %s\n", lua_tostring(l, -1));
    }

    string dataManagerScript = path + "lua/data_manager.lua";
    if (luaL_dofile(l, dataManagerScript.c_str())) {
        printf("error 1: %s\n", lua_tostring(l, -1));
    }

    lua_getglobal(l, "set_screen_size");
    lua_pushnumber(l, SCREEN_WIDTH);
    lua_pushnumber(l, SCREEN_HEIGHT);
    if (lua_pcall(l, 2, 0, 0)) {
        printf("call_lua error: %s\n", lua_tostring(l, -1));
    }
    lua_pop(l, 1);
}

// REFER TO http://nyaocat.hatenablog.jp/entry/2014/01/27/153145
bool init() {
    lua_init();

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

    init_num_texture();

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
    l = luaL_newstate();
    luaL_openlibs(l);

    if (!init()) {
        return 1;
    };

    // initialize rand
    srand((unsigned)time(NULL));

    while (true) {
        // check event
        if (!pollingEvent()) break;

        clock_t begin = clock();

        // GameLoop
        Update();
        Draw();

        // wait
        clock_t end = clock();

        // TODO Display fps

        // spent milli seconds
        double diff = (double)(end - begin) / CLOCKS_PER_SEC * 1000;
        int wait_time = (TARGET_FRAME_TIME - diff - 1);

        // wait
        if (0 < wait_time) {
            usleep((useconds_t)wait_time);
            // advanced: wait strictly by switching
            while (TARGET_FRAME_TIME >= ((double)(clock() - begin) / CLOCKS_PER_SEC * 1000)) {
                usleep(0);
            }
        }
    }

    dealloc();
    lua_close(l);

    return 0;
}
