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

  free_block->size -= sizeof(alloc_block);
  free_block->size -= sizeof(free_block);
  free_block->prev = (free_block - 1);
  free_block->next = free_block + 1;
  free_block->size = sizeof(arena);
}

void *orig_malloc(size_t size) {
  alloc_block = alloc_block->next + 1;
  struct HeapBlock *block_head = free_block->next;
  block_head->size = sizeof(block_head);
  block_head->next = alloc_block;

  // 返す値は、alloc_blockではなくて、新しくallocしたブロック
  alloc_block->next = block_head + block_head->size;
  alloc_block->size += block_head->size + size;
  free_block->next = alloc_block->next + 1;
  free_block->size -= block_head->size + size;
  return alloc_block;
}

void orig_free(void *ptr) {
  struct HeapBlock *block_head = (HeapBlock *)(&ptr - 1);
  block_head->next->prev = block_head->prev;
  block_head->prev = free_block;
  block_head->size = 0;

  free_block->size += sizeof(ptr);
  alloc_block->size -= sizeof(ptr);

  if (block_head && alloc_block->next) {
    alloc_block->next = block_head->next;
  }
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

  printf("--- alloc again ---\n");
  printf("*two  %d\n", *two);
  printf("two %p\n", two);
  printf("*three %d\n", *three);
  printf("three %p\n", three);
  printf("--- end ---\n");

  return 0;
}
