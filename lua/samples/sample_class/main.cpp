#include <iostream>
#include "point.cpp"
#include "../lua-5.2.3/src/lua.hpp"

using namespace std;

lua_State* l;

int run_class_sample(lua_State *l, string script) {
    if (luaL_dofile(l, script.c_str())) {
        printf("error: %s\n", lua_tostring(l, -1));
        return -1;
    }
    return 0;
}

// ---- glue code
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
// ----

int create_instance(lua_State *l, string script) {
    if (luaL_dofile(l, script.c_str())) {
        printf("error: %s\n", lua_tostring(l, -1));
        return -1;
    }

    lua_register(l, "Point", lua_Point);

    lua_getglobal(l, "create_instance");
    if (lua_pcall(l, 0, 0, 0)) {
        printf("error: %s \n", lua_tostring(l, -1));
        return -2;
    }
    return 0;
}

int main() {
    string home = getenv("HOME");
    string path = home + "/src/github.com/hello_low_layer/lua/samples/sample_class/";

    l = luaL_newstate();
    luaL_openlibs(l);

    run_class_sample(l, path + "Point.lua");
    create_instance(l, path + "Functions.lua");

    lua_close(l);
    return 0;
}
