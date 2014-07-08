#include "lua-5.2.3/src/lua.hpp"

// Lua ver 5.2
int main(void)
{
  lua_State *l = luaL_newstate();
  luaL_openlibs(l);
  int result = luaL_dostring(l, "print(\"Hello from Lua! This Lua version is \" .. _VERSION)");
  lua_close(l);

  return result;
}
