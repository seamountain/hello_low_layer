#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lua-5.2.3/src/lua.hpp"

int parse_input(int* is_valid) {
  char input[10];

  fgets(input, sizeof(input), stdin);

  if (input[0] == '\n') {
    *is_valid = 0;
  }

  return atoi(input);
}

int call_lua(lua_State *l, int x, int y) {
  if (luaL_dofile(l, "sum.lua")) {
    printf("error: %s\n", lua_tostring(l, -1));
    return -1;
  }

  lua_getglobal(l, "lua_sum");
  lua_pushnumber(l, x);
  lua_pushnumber(l, y);

  if (lua_pcall(l, 2, 1, 0)) {
      printf("error: %s\n", lua_tostring(l, -1));
      return -2;
  }

  int result = lua_tonumber(l, -1);
  printf("%d + %d = %d\n", x, y, result);

  return 0;
}

int main(int argc, char* argv[])
{
  int is_valid = 1;

  lua_State *l = luaL_newstate();
  int count = 0;
  int input[2];

  while (is_valid) {
    input[count] = parse_input(&is_valid);
    count++;
    if (is_valid) {
      if (count == 2) {
        call_lua(l, input[0], input[1]);
        count = 0;
      }
    }
  }

  printf("You didn't input a number.\n");

  return 0;
}
