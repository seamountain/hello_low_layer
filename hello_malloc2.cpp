#include <iostream>
//#include <unsistd.h>

typedef struct HeapBlock {
  struct HeapBlock *prev;
  struct HeapBlock *next;
  int size;
} HeapBlock;

int arena[10000];
struct HeapBlock alloc_block;
struct HeapBlock free_block;

void initialize_block() {
  //alloc_block.prev = &arena;
  alloc_block.next = &alloc_block;
  alloc_block.size = sizeof(arena);
  //free_block.prev = &arena;
  //free_block.next = &arena;
  free_block.size = sizeof(arena);
}

void *orig_malloc(size_t size) {
  struct HeapBlock *block_head;
  struct HeapBlock *current_block = alloc_block.next;
  while(current_block->next != &alloc_block) {
    current_block = current_block->next;
    printf("Here is inside while.");
  }
  block_head->prev = current_block->next;
  block_head->next = &alloc_block;
  block_head->size = size;
  current_block->next = block_head;
  free_block.next = block_head;
  return (&free_block.next + 1);
}

int main() {
  initialize_block();
  int zero = (intptr_t)orig_malloc(sizeof(int));
  return 0;
}
