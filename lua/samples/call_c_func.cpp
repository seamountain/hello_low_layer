#include "lua-5.2.3/src/lua.hpp"

// REFER TO http://marupeke296.com/LUA_No5_CallFuncOfCFromLua.html

// REFER TO http://gamedevgeek.com/tutorials/calling-c-functions-from-lua/
int CFunc(lua_State *l) {
  printf("This is CFunc \n");
  char c_str[] = "String from C";
  lua_pushstring(l, c_str);
  return 1; /* return the number of results */
};

int call_dostring(lua_State *l) {
  if (luaL_dostring(l, "CFunc()")) {
    printf("error: %s\n", lua_tostring(l, -1));
    return -1;
  }
  return 0;
}

int call_lua_file(lua_State *l) {
  if (luaL_dofile(l, "call_c_func.lua")) {
    printf("error: %s\n", lua_tostring(l, -1));
    return -1;
  }

  lua_getglobal(l, "call_c");

  if (lua_pcall(l, 0, 1, 0)) {
      printf("error: %s\n", lua_tostring(l, -1));
      return -2;
  }

  return 0;
}

int main(void)
{
  lua_State *l = luaL_newstate();
  lua_register(l, "CFunc", &CFunc);
  luaL_openlibs(l);

  call_dostring(l);
  call_lua_file(l);

  lua_close(l);

  return 0;
}
