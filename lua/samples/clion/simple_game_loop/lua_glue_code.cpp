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
    int y = d->getWidth();
    lua_pushnumber(l, y);
    return 1;
}

int lua_get_height(lua_State *l) {
    Data* d = *(Data**)lua_touserdata(l, 1);
    int y = d->getHeight();
    lua_pushnumber(l, y);
    return 1;
}

int lua_get_direction(lua_State *l) {
    Data* d = *(Data**)lua_touserdata(l, 1);
    int direction = d->getDirection();
    lua_pushnumber(l, direction);
    return 1;
}

int lua_update_data(lua_State *l) {
    Data* d = *(Data**)lua_touserdata(l, 1);
    int x = (int)lua_tonumber(l, -3);
    int y = (int)lua_tonumber(l, -2);
    int direction = (int)lua_tonumber(l, -1);

    d->update(x, y, direction);
    return 0;
}

void data_function_init(lua_State *l) {
    lua_newuserdata(l, sizeof(Data*));
    lua_newtable(l);
    lua_newtable(l);

    lua_pushcfunction(l, lua_update_data);
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

// TODO DataをLuaに渡すか、関数だけ登録するか統一する
int get_hit_data(lua_State *l) {
    Data* hit_data = NULL;
    for (int i = 0; i < drawing_data_list.size(); i++) {
        Data* d = drawing_data_list[i];
        if (d != target_data) {
            int x1 = target_data->getX();
            int w1 = target_data->getWidth();
            int x2 = d->getX();
            int w2 = d->getWidth();

            int y1 = target_data->getY();
            int h1 = target_data->getHeight();
            int y2 = d->getY();
            int h2 = d->getHeight();

            if ((x1 - w2 <= x2 && x2 <= x1 + w1) && (y1 - h2 <= y2 && y2 <= y1 + h1)) {
                // TODO 1つだけではなく、すべてのDataをLuaに伝える
                hit_data = d;
                break;
            } else {
                hit_data = NULL;
            }
        }
    }

    if (hit_data != NULL) {
        data_function_init(l);
        Data** d = (Data**)lua_touserdata(l, 1);
        *d = hit_data;
    } else {
        lua_pushnil(l);
    }
    return 1;
}