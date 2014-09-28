sample_print: print run

sample_dostring: dostring run

sample_dofile: dofile run

sample_call_c_func: call_c_func run

sample_input_calc: input_calc run

sample_game_loop: game_loop run

# http://stackoverflow.com/questions/8024498/compile-embedded-lua-in-c
print:
	g++ print.cpp -o result.out -I./lua-5.2.3/include -L./lua-5.2.3/lib -llua

dostring: dostring.cpp
	g++ dostring.cpp -o result.out -I./lua-5.2.3/include -L./lua-5.2.3/lib -llua

dofile: dofile.cpp
	g++ dofile.cpp -o result.out -I./lua-5.2.3/include -L./lua-5.2.3/lib -llua

call_c_func: call_c_func.cpp
	g++ call_c_func.cpp -o result.out -I./lua-5.2.3/include -L./lua-5.2.3/lib -llua

input_calc: input.cpp
	g++ input.cpp -o result.out -I./lua-5.2.3/include -L./lua-5.2.3/lib -llua

game_loop: game_loop.cpp
	g++ game_loop.cpp -o result.out -I./lua-5.2.3/include -L./lua-5.2.3/lib -llua -g `sdl2-config --cflags` `sdl2-config --libs` -framework GLUT -framework OpenGL

run:
	./result.out

clean:
	rm *.out
