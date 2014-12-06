.PHONY: print dostring dofile call_c_func lua_cpp_bridge game_loop clean

# http://stackoverflow.com/questions/8024498/compile-embedded-lua-in-c
print:
	g++ lua_print/print.cpp -o print.out -I./libs/lua-5.2.3/include -L./libs/lua-5.2.3/lib -llua
	./print.out

dostring:
	g++ dostring/dostring.cpp -o dostring.out -I./libs/lua-5.2.3/include -L./libs/lua-5.2.3/lib -llua
	./dostring.out

dofile:
	g++ dofile/dofile.cpp -o dofile.out -I./libs/lua-5.2.3/include -L./libs/lua-5.2.3/lib -llua
	./dofile.out

call_c_func:
	g++ call_c_func/call_c_func.cpp -o call_c_func.out -I./libs/lua-5.2.3/include -L./libs/lua-5.2.3/lib -llua
	./call_c_func.out

lua_cpp_bridge:
	g++ lua_cpp_bridge/input.cpp -o input.out -I./libs/lua-5.2.3/include -L./libs/lua-5.2.3/lib -llua
	./input.out

game_loop:
	g++ game_loop/game_loop.cpp -o game_loop.out -I./libs/lua-5.2.3/include -L./libs/lua-5.2.3/lib -llua -g `sdl2-config --cflags --libs`
	./game_loop.out

clean:
	rm -r *.dSYM
	rm *.out
