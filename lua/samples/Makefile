sample_lua_call: lua_call run

sample_dostring: dostring run

# http://stackoverflow.com/questions/8024498/compile-embedded-lua-in-c
lua_call:
	g++ lua_call.cpp -o result.out -I./lua-5.2.3/include -L./lua-5.2.3/lib -llua

dostring: dostring.cpp
	g++ dostring.cpp -o result.out -I./lua-5.2.3/include -L./lua-5.2.3/lib -llua

run:
	./result.out

clean:
	rm *.out
