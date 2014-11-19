#include <iostream>
#include "point.cpp"
#include "../lua-5.2.3/src/lua.hpp"

using namespace std;

int call_lua_class(lua_State *l, string script) {
    if (luaL_dofile(l, script.c_str())) {
        printf("error: %s\n", lua_tostring(l, -1));
        return -1;
    }
    return 0;
}

// ---- glue code start
int lua_point_delete(lua_State* l) {
    Point* p = *(Point**)lua_touserdata(l, 1);
    delete p;
    return 0;
}

int lua_point_set_value(lua_State* l) {
    Point* p = *(Point**)lua_touserdata(l, 1);
    p->set_value(lua_tonumber(l, -2), lua_tonumber(l, -1));
    return 0;
}

int lua_point_print_value(lua_State* l) {
    Point* p = *(Point**)lua_touserdata(l, 1);
    p->print_value();
    return 0;
}

int lua_Point(lua_State *l) {
    Point** p = (Point**)lua_newuserdata(l, sizeof(Point*));
    *p = new Point();

    lua_newtable(l);
    lua_newtable(l);

    lua_pushcfunction(l, lua_point_delete);
    lua_setfield(l, 3, "delete");
    lua_pushcfunction(l, lua_point_set_value);
    lua_setfield(l, 3, "set_value");
    lua_pushcfunction(l, lua_point_print_value);
    lua_setfield(l, 3, "print_value");

    lua_setfield(l, 2, "__index");
    lua_setmetatable(l, 1);

    return 1;
}
// ---- glue code end

int create_instance(lua_State *l) {
    lua_getglobal(l, "create_instance");

    if (lua_pcall(l, 0, 0, 0)) {
        printf("error: %s \n", lua_tostring(l, -1));
        return -2;
    }
    return 0;
}

int functions_lua_init(lua_State *l, string script) {
    if (luaL_dofile(l, script.c_str())) {
        printf("error: %s\n", lua_tostring(l, -1));
        return -1;
    }

    lua_register(l, "Point", lua_Point);
    return 0;
}

int main() {
    string home = getenv("HOME");
    string path = home + "/work/github/hello_low_layer/lua/samples/sample_class/";

    lua_State* l = luaL_newstate();
    luaL_openlibs(l);

    call_lua_class(l, path + "Point.lua");

    if (functions_lua_init(l, path + "Functions.lua") != 0) {
        return -1;
    }
    create_instance(l);

    lua_close(l);
    return 0;
}
