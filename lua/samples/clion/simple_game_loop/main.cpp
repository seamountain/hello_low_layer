#include <iostream>
#include <unistd.h>
#include <vector>
#include <map>
#include <SDL_surface.h>
#include "libs/SDL2-2.0.3/include/SDL.h"
#include "libs/lua-5.2.3/include/lua.hpp"
#include "Color.h"

using namespace std;

// TODO fix path
string path = "/Users/sea_mountain/work/github/hello_low_layer/lua/samples/clion/simple_game_loop/";

int TARGET_FPS = 60;
// milli seconds per frame
int TARGET_FRAME_TIME = 1000 / TARGET_FPS;
int fps;

lua_State* l;
SDL_Window* w;
SDL_Renderer* render;
SDL_GLContext context;
SDL_Texture *texture;
SDL_Texture *smile_texture;
SDL_Texture *painful_texture;
bool is_smile = true;

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

enum class Layout {
    BOTTOM_LEFT,
    BOTTOM_RIGHT
};

// REFER TO http://www5.big.or.jp/~high/VENIO/kuz/kuz_are_14.htm
void draw_num_label(int num, Layout layout) {
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
        drw.y = SCREEN_HEIGHT - (text_size + margin);
        switch (layout) {
            case Layout::BOTTOM_LEFT:
                drw.x = margin + i * text_size;
                break;
            case Layout::BOTTOM_RIGHT:
                drw.x = (int)((SCREEN_WIDTH - (label.length() * text_size + margin)) + (i * text_size));
                break;
        }

        SDL_RenderCopy(render, texture, &src, &drw); // Copy the texture into render

        i++;
    }
}

void draw_boss_texture(bool is_smile, SDL_Rect *r) {
    int face_img_size = 100;
    SDL_Rect src,drw;
    src.x = 0;
    src.y = 0;
    src.w = face_img_size;
    src.h = face_img_size;

    drw.x = r->x;
    drw.y = r->y;
    drw.w = r->w;
    drw.h = r->h;

    SDL_Texture** target_texture;
    if (is_smile) {
        target_texture = &smile_texture;
    } else {
        target_texture = &painful_texture;
    }
    SDL_RenderCopy(render, *target_texture, &src, &drw); // Copy the texture into render
}

int set_boss_texture_flag(lua_State *l) {
    is_smile = (bool)lua_toboolean(l, 1);
    return 0;
}

void Update() {
    // http://www.c-lang.net/clock
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

void register_boss_data() {
    lua_getglobal(l, "register_boss_data");

    if (lua_pcall(l, 0, 0, 0)) {
        printf("call_lua error: %s\n", lua_tostring(l, -1));
    }
}

void init_num_texture() {
    string image_path = path + "images/font.bmp";
    SDL_Surface *font_bmp = SDL_LoadBMP(image_path.c_str());
  if (!font_bmp) {
    printf("bmp is null. Error: %s\n", SDL_GetError());
    return;
  }

  texture = SDL_CreateTextureFromSurface(render, font_bmp);

  SDL_FreeSurface(font_bmp);
  if (texture == nullptr){
    std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
  }
}

void init_boss_texture() {
    // Load smile texture
    string smile_image_path = path + "images/smile.bmp";
    SDL_Surface *smile_bmp = SDL_LoadBMP(smile_image_path.c_str());
    if (!smile_bmp) {
        printf("smile_bmp is null. Error: %s\n", SDL_GetError());
        return;
    }

    smile_texture = SDL_CreateTextureFromSurface(render, smile_bmp);

    SDL_FreeSurface(smile_bmp);
    if (smile_texture == nullptr){
        std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
    }

    // Load painful texture
    string painful_image_path = path + "images/painful.bmp";
    SDL_Surface *painful_bmp = SDL_LoadBMP(painful_image_path.c_str());
    if (!painful_bmp) {
        printf("painful_bmp is null. Error: %s\n", SDL_GetError());
        return;
    }

    painful_texture = SDL_CreateTextureFromSurface(render, painful_bmp);

    SDL_FreeSurface(painful_bmp);
    if (painful_texture == nullptr){
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

void draw_boss_params(SDL_Rect* boss_rect) {
    if (boss_rect->w <= 0 || boss_rect->h <= 0) {
        return;
    }

    int bar_width = 150;
    int bar_height = 20;
    int margin = 10;

    int boss_orig_size = 100;

    if (SDL_SetRenderDrawColor(render, 200, 200, 200, 1) < 0) {
        printf("SDL Error %s\n", SDL_GetError());
    }
    SDL_Rect *hp_full_bar = new SDL_Rect();
    hp_full_bar->x = SCREEN_WIDTH - bar_width - margin;
    hp_full_bar->y = margin;
    hp_full_bar->h = bar_height;
    hp_full_bar->w = bar_width;
    SDL_RenderFillRect(render, hp_full_bar);

    float boss_hp_rate = ((float)(boss_rect->w * boss_rect->h) / (boss_orig_size * boss_orig_size));
    SDL_Color *color = new SDL_Color();
    if (0.3 < boss_hp_rate) {
        color->r = 130;
        color->g = 230;
        color->b = 140;
    } else {
        color->r = 230;
        color->g = 130;
        color->b = 140;
    }

//    printf("w %i, h %i, rate %f\n", boss_rect->w, boss_rect->h, boss_hp_rate);

    if (SDL_SetRenderDrawColor(render, color->r, color->g, color->b, 1) < 0) {
        printf("SDL Error %s\n", SDL_GetError());
    }
    SDL_Rect *hp_bar = new SDL_Rect();
    hp_bar->x = SCREEN_WIDTH - bar_width - margin;
    hp_bar->y = margin;
    hp_bar->h = bar_height;
    int exist_hp_bar_width = (int)(bar_width * boss_hp_rate);
    int min_width = 1;
    hp_bar->w = (exist_hp_bar_width <= min_width) ? min_width : exist_hp_bar_width;
    SDL_RenderFillRect(render, hp_bar);
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

    int boss_color_id = -1;
    map<int, vector<SDL_Rect *>> data_list;
    SDL_Rect *boss_rect;
    if (data_num != 0) {
        lua_getglobal(l, "data_list");
        lua_pushnil(l);
        while (lua_next(l, -2) != 0) {
            int color_id;
            lua_pushnil(l);
            SDL_Rect *r = new SDL_Rect();
            while (lua_next(l, -2) != 0) {
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
                    color_id = (int) lua_tointeger(l, -1);
                }
                lua_pop(l, 1);
            }
            if (color_id == boss_color_id) {
                boss_rect = r;
            } else {
                data_list[color_id].push_back(r);
            }
            lua_pop(l, 1);
        }
    }
    lua_settop(l, 0);

    draw_boss_texture(is_smile, boss_rect);

    if (data_list.size() != 0) {
        for (int i = 0; i < palettes.size(); i++) {
            Color *color = new Color(*palettes[i]);
            SDL_SetRenderDrawColor(render, color->r, color->g, color->b, color->a);
            delete color;

            vector<SDL_Rect *> rects = data_list[i];
            if (rects.size() != 0) {
                SDL_Rect* rects_arr = new SDL_Rect[rects.size()];
                for (int j = 0; j < rects.size(); j++) {
                    rects_arr[j] = *rects[j];
                }

                if (SDL_RenderFillRects(render, rects_arr, rects.size()) < 0) {
                    printf("%s\n", SDL_GetError());
                }
                delete [] rects_arr;
            }
        }
    }

    draw_palette_button();
    draw_boss_params(boss_rect);
    draw_num_label(data_num, Layout::BOTTOM_LEFT);
    draw_num_label(fps, Layout::BOTTOM_RIGHT);

    SDL_RenderPresent(render);
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

    string bossScript = path + "lua/boss.lua";
    if (luaL_dofile(l, bossScript.c_str())) {
        printf("error 1: %s\n", lua_tostring(l, -1));
    }

    string dataManagerScript = path + "lua/data_manager.lua";
    if (luaL_dofile(l, dataManagerScript.c_str())) {
        printf("error 1: %s\n", lua_tostring(l, -1));
    }

    string DataLeafScript = path + "lua/data_leaf.lua";
    if (luaL_dofile(l, DataLeafScript.c_str())) {
        printf("error 1: %s\n", lua_tostring(l, -1));
    }

    string spacePertitionScript = path + "lua/space_pertition.lua";
    if (luaL_dofile(l, spacePertitionScript.c_str())) {
        printf("error 1: %s\n", lua_tostring(l, -1));
    }

    lua_getglobal(l, "set_screen_size");
    lua_pushnumber(l, SCREEN_WIDTH);
    lua_pushnumber(l, SCREEN_HEIGHT);
    if (lua_pcall(l, 2, 0, 0)) {
        printf("call_lua error: %s\n", lua_tostring(l, -1));
    }
    lua_pop(l, 1);

    // C functions
    lua_register(l, "set_boss_texture_flag", set_boss_texture_flag);
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
    init_boss_texture();

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

    float FPS_CALC_INTERVAL = 0.5;
    clock_t prev_time = clock();
    clock_t spent_time;

    register_boss_data();

    // main loop
    while (true) {
        // check event
        if (!pollingEvent()) break;

        clock_t begin = clock();

        // GameLoop
        Update();
        Draw();

        // wait
        clock_t end = clock();

        // calc fps
        // http://hakuhin.jp/as/fps.html#FPS_00
        double current_interval_time = (double)(end - prev_time) / CLOCKS_PER_SEC;
        if (FPS_CALC_INTERVAL <= current_interval_time) {
            spent_time = (clock() - prev_time);
            fps = (int)(frame_count / ((double)spent_time / CLOCKS_PER_SEC));
            prev_time = clock();
            frame_count = 0;
        }

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

        frame_count++;
    }

    dealloc();
    lua_close(l);

    return 0;
}
