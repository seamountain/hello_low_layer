sample1: clean lua_call run

sample2: clean dostring run

# http://stackoverflow.com/questions/8024498/compile-embedded-lua-in-c
lua_call:
	g++ test.cpp -o result.out -llua

dostring: sample.cpp
	g++ sample.cpp -o result.out -I./lua-5.2.3/include -L./lua-5.2.3/lib -llua -lm -ldl

run:
	./result.out

clean:
	rm *.out
