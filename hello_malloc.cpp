#include <iostream>
#include <unistd.h>

typedef struct HeapBlock {
  struct HeapBlock *prev;
  struct HeapBlock *next;
  int size;
} HeapBlock;

struct HeapBlock alloc_block;
struct HeapBlock free_block;
int arena[10000];
//struct HeapBlock memory_list[];

void initialize_memory(int arena[], int size) {
  alloc_block.next = &alloc_block;
  free_block.size = size;
}

void free_memory() {
}

// Refer to man malloc. "The malloc() function allocates size bytes of memory and returns a pointer to the allocated memory."
//void* orig_malloc(int size) {
int* orig_malloc(int size) {
  struct HeapBlock *current_block = &alloc_block;
  struct HeapBlock *last_block;
  while(current_block->next != NULL) {
    last_block = current_block->next;
    current_block = last_block;
  }
  //last_block + 1; // このアドレスの領域が使える
  //initialize_memory(last_block + 1, size);
  struct HeapBlock* new_block_position = (last_block + 1);
  //int initialize_value = 0;
  //for(int i = 0; i <= size; i++) {
    //last_block + (i + 1) = initialize_value;
  //}
  struct HeapBlock new_block = *new_block_position;
  new_block.prev = last_block;
  new_block.size = size;
  last_block->next = &new_block;
  return &new_block;
}

int main() {
  initialize_memory(arena, sizeof(arena));

  //void* test_value = orig_malloc(sizeof(int));
  int* test_value = orig_malloc(sizeof(int));
  printf("Hello malloc! int sample_value: %i size: %ld", *test_value, sizeof(test_value));

  free_memory();
  return 0;
}
