#include <iostream>

typedef struct HeapBlock {
  struct HeapBlock *prev;
  struct HeapBlock *next;
  int size;
} HeapBlock;

const int array_size = 10000;
int arena[array_size];
struct HeapBlock *alloc_block;
struct HeapBlock *free_block;

void initialize_block() {
  alloc_block = (HeapBlock *)&arena;
  free_block = (HeapBlock *)&arena + sizeof(alloc_block);

  alloc_block->next = alloc_block;
  alloc_block->size = sizeof(arena);

  printf("sizeof(alloc_block): %lu\n", sizeof(alloc_block));

  free_block->prev = (free_block - 1);
  free_block->next = free_block + 1;
  free_block->size = sizeof(arena);

  alloc_block->size += sizeof(alloc_block);
  free_block->size -= sizeof(free_block);

  printf("free_block->size: %d\n", free_block->size);
  printf("alloc_block->size: %d\n", alloc_block->size);
}

void *orig_malloc(size_t size) {
  struct HeapBlock *new_alloc_block = free_block->next;
  struct HeapBlock *block_head = free_block->next;
  block_head->size = size;
  block_head->prev = alloc_block->next;
  HeapBlock *next_block = alloc_block->next;
  if (next_block && !next_block->next) {
    next_block->next = (HeapBlock *)&arena;
  }
  alloc_block->next = block_head;

  alloc_block->next = block_head;
  alloc_block->size += sizeof(new_alloc_block) + size;
  free_block->next = alloc_block->next + sizeof(block_head)+ block_head->size + 1;
  free_block->size -= sizeof(new_alloc_block) + size;

  return new_alloc_block + size;
}

void orig_free(void *ptr) {
  printf("&ptr: %p\n", &ptr);
  printf("ptr: %p\n", ptr);
  printf("sizeof(alloc_block): %lu\n", sizeof(alloc_block));

  struct HeapBlock *block_head = (HeapBlock *)(ptr) - sizeof(alloc_block);

  if (block_head && alloc_block->next) {
    alloc_block->next = block_head->next;
  }

  if (block_head && free_block->next) {
    free_block->next = (HeapBlock *)&arena + sizeof(alloc_block);
  }

  printf("size: %d\n", (int)((HeapBlock *)&ptr)->size);
  printf("sizeof(ptr): %lu\n", sizeof(ptr));
  printf("((HeapBlock *)&ptr): %lu\n", ((HeapBlock *)&ptr));

  free_block->size += sizeof(ptr) + block_head->size;
  alloc_block->size -= sizeof(ptr) + block_head->size;

  block_head->prev = free_block;
  if (block_head->next) {
    block_head->next->prev = block_head->prev;
  }
  block_head->size = 0;
}

int main() {
  initialize_block();
  int *zero = (int *)orig_malloc(sizeof(int));
  if (zero == NULL) exit(0);
  *zero = 0;

  int *one = (int *)orig_malloc(sizeof(int));
  if (one == NULL) exit(0);
  *one = 1;

  printf("--- result ---\n");
  printf("*zero %d\n", *zero);
  printf("zero %p\n", zero);
  printf("*one %d\n", *one);
  printf("one %p\n", one);
  printf("--- end ---\n");

  orig_free(zero);
  orig_free(one);

  printf("--- after free ---\n");
  printf("*zero %d\n", *zero);
  printf("zero %p\n", zero);
  printf("*one %d\n", *one);
  printf("one %p\n", one);
  printf("--- end ---\n");

  int *two = (int *)orig_malloc(sizeof(int));
  if (two == NULL) exit(0);
  *two = 2;

  int *three = (int *)orig_malloc(sizeof(int));
  if (three == NULL) exit(0);
  *three = 3;

  // TODO Check new alloc addres
  printf("--- alloc again ---\n");
  printf("*two  %d\n", *two);
  printf("two %p\n", two);
  printf("*three %d\n", *three);
  printf("three %p\n", three);
  printf("--- end ---\n");

  return 0;
}
