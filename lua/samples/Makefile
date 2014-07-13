sample_print: print run

sample_dostring: dostring run

sample_dofile: dofile run

sample_call_c_func: call_c_func run

# http://stackoverflow.com/questions/8024498/compile-embedded-lua-in-c
print:
	g++ print.cpp -o result.out -I./lua-5.2.3/include -L./lua-5.2.3/lib -llua

dostring: dostring.cpp
	g++ dostring.cpp -o result.out -I./lua-5.2.3/include -L./lua-5.2.3/lib -llua

dofile: dofile.cpp
	g++ dofile.cpp -o result.out -I./lua-5.2.3/include -L./lua-5.2.3/lib -llua

call_c_func: call_c_func.cpp
	g++ call_c_func.cpp -o result.out -I./lua-5.2.3/include -L./lua-5.2.3/lib -llua

run:
	./result.out

clean:
	rm *.out
