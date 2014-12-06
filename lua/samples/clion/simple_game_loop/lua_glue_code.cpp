Data* target_data;

int update_data(lua_State *l) {
    int x = (int)lua_tonumber(l, -3);
    int y = (int)lua_tonumber(l, -2);
    int direction = (int)lua_tonumber(l, -1);
    target_data->update(x, y, direction);
    return 0;
}

int set_size(lua_State *l) {
    int w = (int)lua_tonumber(l, -2);
    int h = (int)lua_tonumber(l, -1);
    target_data->setSize(w, h);
    return 0;
}

int get_x(lua_State *l) {
    int x = target_data->getX();
    lua_pushnumber(l, x);
    return 1;
}

int get_y(lua_State *l) {
    int y = target_data->getY();
    lua_pushnumber(l, y);
    return 1;
}

int get_width(lua_State *l) {
    int w = target_data->getWidth();
    lua_pushnumber(l, w);
    return 1;
}

int get_height(lua_State *l) {
    int h = target_data->getHeight();
    lua_pushnumber(l, h);
    return 1;
}

int get_direction(lua_State *l) {
    int direction = target_data->getDirection();
    lua_pushnumber(l, direction);
    return 1;
}

int delete_data(lua_State *l) {
    remove_data_from_lua(target_data);
    free(target_data);
    return 0;
}

int data_init(lua_State *l) {
    int x = (int)lua_tonumber(l, -5);
    int y = (int)lua_tonumber(l, -4);
    int w = (int)lua_tonumber(l, -3);
    int h = (int)lua_tonumber(l, -2);
    int direction = (int)lua_tonumber(l, -1);

    lua_newuserdata(l, sizeof(Data*));
    Data** d = (Data**)lua_touserdata(l, -1);
    *d = new Data(x, y, w, h, target_data->getColor(), direction);

    register_data_from_lua(*d);

    return 1;
}
