#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <OpenGL/glu.h>
#include "lua-5.2.3/src/lua.hpp"
#include "game_loop/SDL2-2.0.3/include/SDL.h"

int TARGET_FPS = 60;
// milli seconds per frame
int TARGET_FRAME_TIME = 1000 / TARGET_FPS;

SDL_Window* w;
SDL_Renderer* render;
SDL_GLContext context;
int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 640;

int frame_count = 0;
int aindex = 0;
int pos[1000];
int size;

void Update(lua_State *l) {
  if (frame_count == TARGET_FPS) {
    // http://www.c-lang.net/clock
    frame_count = 0;
  } else {
    frame_count++;
  }

  lua_getglobal(l, "add_data");
  lua_pushnumber(l, pos[aindex]);

  if (lua_pcall(l, 1, 1, 0)) {
    printf("error 2: %s\n", lua_tostring(l, -1));
  }
  pos[aindex] = lua_tonumber(l, -1);
  aindex++;

  if (size < aindex) {
    aindex = 0;
  }
}

int x1 = 100;
//int y1 = 10;
int x2 = 400;
int y2 = 400;

void Draw(lua_State *l) {
  // call lua func
  //printf("%d \n", frame_count);

  SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
  SDL_RenderClear(render);

  SDL_SetRenderDrawColor(render, 100, 155, 80, 255);
  for (int i = 0; i < size; i++) {
    SDL_RenderDrawLine(render, x1, pos[i], x2, y2);
  }
  SDL_RenderPresent(render);
}

// REFER TO http://nyaocat.hatenablog.jp/entry/2014/01/27/153145
bool init(lua_State *l) {
  if (luaL_dofile(l, "./game_loop/update.lua")) {
    printf("error 1: %s\n", lua_tostring(l, -1));
  }

  if (luaL_dofile(l, "./game_loop/draw.lua")) {
    printf("error 3: %s\n", lua_tostring(l, -1));
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

  init(l);

  size = sizeof(pos) / sizeof(pos[0]);

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
