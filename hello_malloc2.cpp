#include <iostream>

typedef struct HeapBlock {
  struct HeapBlock *prev;
  struct HeapBlock *next;
  int size;
} HeapBlock;

const int array_size = 10000;
int arena[array_size];
struct HeapBlock alloc_block;
struct HeapBlock free_block;

void initialize_block() {
  alloc_block.next = &alloc_block;
  alloc_block.size = sizeof(arena);
  free_block.size = sizeof(arena);
}

void *orig_malloc(size_t size) {
  struct HeapBlock block_head;
  struct HeapBlock current_block = *alloc_block.next;

  // TODO: Rest size check
  // TODO: Adding test script

  printf("&alloc_block %p\n", &alloc_block);
  printf("current_block %p\n", &current_block);
  printf("current_block->next %p\n", current_block.next);

  while(current_block.next != &alloc_block) {
    current_block = *current_block.next;
  }

  printf("current_block %p\n", current_block.next);
  printf("block_head %p\n", &block_head);
  printf("block_head->next %p\n", block_head.next);

  block_head.prev = current_block.next;
  block_head.next = &alloc_block;
  block_head.size = size;
  current_block.next = &block_head;
  free_block.next = &block_head;
  return (&free_block.next + 1);
}

int main() {
  initialize_block();
  int zero = (intptr_t)orig_malloc(sizeof(int));
  zero = 0;
  printf("zero %d\n", zero);
  printf("&zero %p\n", &zero);

  int one = (intptr_t)orig_malloc(sizeof(int));
  one = 1;
  printf("one %d\n", one);
  printf("&one %p\n", &one);

  return 0;
}
