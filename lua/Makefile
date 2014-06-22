default: test run

sample: build_sample run

# http://stackoverflow.com/questions/8024498/compile-embedded-lua-in-c
test:
	g++ test.cpp -o result.out -llua

build_sample:
	g++ sample.cpp -o result.out -llua

run:
	./result.out

clean:
	rm *.out
