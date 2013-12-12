#include <stdio.h>
#include <stdlib.h>

typedef struct Hello {
	int a, b, c, d;
	int array[100];
} Hello;

int arena[1024];

int main() {
	/*ore_mem_init(arena, sizeof (arena));
	/*ore_mem_init(address, size); そのアドレスとサイズしか使わなくなるから、ここで初期化する*/
	int* i = malloc(sizeof (int));
	Hello* s = malloc(sizeof (Hello));
	*i = 100;
	printf("Hello world! %p %i %ld %ld\n", i, *i, sizeof(i), sizeof(*i));
	printf("Hello world! %p %ld %ld\n", s ,sizeof(s), sizeof(*s));
	printf("Hello world! %p %i %ld %ld\n", i + 1, *i, sizeof(i), sizeof(*i));
	printf("Hello world! %p %ld %ld\n", s + 1 ,sizeof(s), sizeof(*s));
	free(i);
	/*ore_mem_exit();*/
	return 0;
}
