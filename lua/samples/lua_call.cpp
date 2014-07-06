#include <lua.hpp>

// Lua ver 5.1
// Refer to http://snipplr.com/view/10169/
void print_msg(lua_State* s, char* msg) {
  luaL_openlibs(s);
  lua_getfield(s, LUA_GLOBALSINDEX, "print");
  lua_pushstring(s, msg);
  lua_call(s,1,0);
}

// Refer to
// http://www.acamara.es/blog/2012/08/running-a-lua-5-2-script-from-c/

int main(int argc, char* argv[])
{
  // create new Lua state
  lua_State *lua_state = luaL_newstate();

  char msg[] = "hoge";
  print_msg(lua_state, msg);

  // close the Lua state
  lua_close(lua_state);

  return 0;
}
