#include <iostream>
#include "../lua-5.2.3/src/lua.hpp"

using namespace std;

int main() {
    cout << "Hello, World!" << endl;

    string path = "#{HOME_PATH}/src/github.com/hello_low_layer/lua/samples/sample_class/";
    string script = path + "Point.lua";

    lua_State *l = luaL_newstate();
    luaL_openlibs(l);

    if (luaL_dofile(l, script.c_str())) {
        printf("error: %s\n", lua_tostring(l, -1));
        return -1;
    }

    lua_close(l);
    return 0;
}