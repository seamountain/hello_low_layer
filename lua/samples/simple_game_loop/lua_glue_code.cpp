Data* target_data;

int update_target_data(lua_State *l) {
    Data* d = *(Data**)lua_touserdata(l, 1);
    int x = (int)lua_tonumber(l, -3);
    int y = (int)lua_tonumber(l, -2);
    int direction = (int)lua_tonumber(l, -1);
    d->update(x, y, direction);
    return 0;
}

int lua_get_x(lua_State *l) {
    Data* d = *(Data**)lua_touserdata(l, 1);
    int x = d->getX();
    lua_pushnumber(l, x);
    return 1;
}

int lua_get_y(lua_State *l) {
    Data* d = *(Data**)lua_touserdata(l, 1);
    int y = d->getY();
    lua_pushnumber(l, y);
    return 1;
}

int lua_get_width(lua_State *l) {
    Data* d = *(Data**)lua_touserdata(l, 1);
    int w = d->getWidth();
    lua_pushnumber(l, w);
    return 1;
}

int lua_get_height(lua_State *l) {
    Data* d = *(Data**)lua_touserdata(l, 1);
    int h = d->getHeight();
    lua_pushnumber(l, h);
    return 1;
}

int lua_get_direction(lua_State *l) {
    Data* d = *(Data**)lua_touserdata(l, 1);
    int direction = d->getDirection();
    lua_pushnumber(l, direction);
    return 1;
}

// TODO 関数の登録はインスタンスごとに毎回必要？
void data_function_init(lua_State *l) {
    lua_newuserdata(l, sizeof(Data*));

    lua_newtable(l);
    lua_newtable(l);

    lua_pushcfunction(l, update_target_data);
    lua_setfield(l, 3, "update");
    lua_pushcfunction(l, lua_get_x);
    lua_setfield(l, 3, "get_x");
    lua_pushcfunction(l, lua_get_y);
    lua_setfield(l, 3, "get_y");
    lua_pushcfunction(l, lua_get_width);
    lua_setfield(l, 3, "get_width");
    lua_pushcfunction(l, lua_get_height);
    lua_setfield(l, 3, "get_height");
    lua_pushcfunction(l, lua_get_direction);
    lua_setfield(l, 3, "get_direction");

    lua_setfield(l, 2, "__index");
    lua_setmetatable(l, 1);
}

int get_target_data(lua_State *l) {
    data_function_init(l);

    Data** d = (Data**)lua_touserdata(l, 1);
    *d = target_data;

    return 1;
}
