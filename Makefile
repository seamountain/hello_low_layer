default: hello run

hello: hello_malloc2.cpp
	g++ hello_malloc2.cpp -o malloc.out

run:
	./malloc.out

clean:
	rm *.out
