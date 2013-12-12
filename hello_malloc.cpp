#include <iostream>
#include <unistd.h>

typedef struct HeapBlock {
  struct HeapBlock *prev;
  struct HeapBlock *next;
  int size;
} HeapBlock;

void initialize_memory(int address, int size) {
  int head_address = sbrk();
}

void free_memory() {
}

int main() {
  struct HeapBlock free;
  struct HeapBlock alloc;
  free.size = 256; // This value is sample
  alloc.prev = &alloc;

  //initialize_memory(free.size);

  //free_memory();
}
