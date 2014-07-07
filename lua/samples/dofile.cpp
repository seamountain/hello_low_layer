#include "lua-5.2.3/src/lua.hpp"

// REFER TO http://marupeke296.com/LUA_No2_Begin.html
void printStack(lua_State *L) {
    // スタック数を取得
    const int num = lua_gettop(L);
    if (num == 0) {
        printf("No stack.\n");
        return;
    }
    for (int i = num; i >= 1; i--) {
        printf("%03d(%04d): ", i, -num + i - 1);
        int type = lua_type(L, i);
        switch(type) {
        case LUA_TNIL:
            printf("NIL\n");
            break;
        case LUA_TBOOLEAN:
            printf("BOOLEAN %s\n", lua_toboolean(L, i) ? "true" : "false");
            break;
        case LUA_TLIGHTUSERDATA:
            printf("LIGHTUSERDATA\n");
            break;
        case LUA_TNUMBER:
            printf("NUMBER %f\n", lua_tonumber(L, i));
            break;
        case LUA_TSTRING:
            printf("STRING %s\n", lua_tostring(L, i));
            break;
        case LUA_TTABLE:
            printf("TABLE\n");
            break;
        case LUA_TFUNCTION:
            printf("FUNCTION\n");
            break;
        case LUA_TUSERDATA:
            printf("USERDATA\n");
            break;
        case LUA_TTHREAD:
            printf("THREAD\n");
            break;
        }
    }
    printf("-----------------------------\n\n");
}


// REFER TO http://ameblo.jp/coffeex/theme-10002528966.html
// SEE ALSO http://www.hakkaku.net/articles/20081126-288
// SEE ALSO https://github.com/torus/embedding-lua/wiki/API

int main(void)
{
  lua_State *l = luaL_newstate();
  if (luaL_dofile(l, "sum.lua")) {
    printf("error: %s\n", lua_tostring(l, -1));
    return -1;
  }
  printStack(l);

  lua_getglobal(l, "lua_sum");
  printStack(l);

  lua_pushnumber(l, 1);
  printStack(l);

  lua_pushnumber(l, 10);
  printStack(l);

  if (lua_pcall(l, 2, 1, 0)) {
      printf("error: %s\n", lua_tostring(l, -1));
      return -2;
  }
  printStack(l);

  double result = lua_tonumber(l, -1);
  printf("result: %f\n", result);

  return 0;
}
