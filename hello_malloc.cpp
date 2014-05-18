#include <iostream>
#include "cassert"

struct HeapBlock {
  struct HeapBlock *prev;
  struct HeapBlock *next;
  int size;
};

const int array_size = 10000;
int arena[array_size];
struct HeapBlock *alloc_block = new HeapBlock();
struct HeapBlock *free_block = new HeapBlock();

void initialize_block() {
  alloc_block->next = alloc_block;
  alloc_block->size = 0;
  free_block->prev = (HeapBlock *)arena;
  free_block->next = (HeapBlock *)&arena[0];
  free_block->size = sizeof(arena);
}

void *orig_malloc(size_t size) {
  struct HeapBlock *new_alloc_block = free_block->next;
  new_alloc_block->prev = alloc_block->next;
  alloc_block->next->prev = new_alloc_block;
  new_alloc_block->next = alloc_block->prev;
  new_alloc_block->size = size;
  alloc_block->next = new_alloc_block;
  alloc_block->size += sizeof(new_alloc_block) + size;

  free_block->next = (struct HeapBlock*)((size_t)(new_alloc_block + 1) + size);
  free_block->prev = alloc_block->next;
  free_block->size -= sizeof(new_alloc_block) + size;

  return new_alloc_block + 1;
}

void orig_free(void *ptr) {
  struct HeapBlock *block_head = (HeapBlock *)(ptr) - sizeof(alloc_block);

  if (block_head && alloc_block->next) {
    alloc_block->next = block_head->next;
  }

  if (block_head && free_block->next) {
    free_block->next = (HeapBlock *)&arena + sizeof(alloc_block);
  }

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

  assert(alloc_block->size == 0);
  assert(free_block->size == sizeof(arena));

  int *zero = (int *)orig_malloc(sizeof(int));
  *zero = 0;

  assert(alloc_block->size == (sizeof(HeapBlock *) + sizeof(*zero)));
  assert(free_block->size == sizeof(arena) - (sizeof(HeapBlock *) + sizeof(*zero)));

  assert(zero == &arena[0] + (sizeof(HeapBlock) / sizeof(int)));

  int *one = (int *)orig_malloc(sizeof(int));
  *one = 1;

  assert(alloc_block->size == (sizeof(HeapBlock *) * 2 + sizeof(*zero) + sizeof(*one)));
  assert(free_block->size == sizeof(arena) - (sizeof(HeapBlock *) * 2 + sizeof(*zero + sizeof(*one))));

  struct HeapBlock* block = (struct HeapBlock*)zero - 1;
  assert((struct HeapBlock*)one - 1 == block->next);
  assert(one == zero + 1 + (sizeof(HeapBlock) / (sizeof(zero) + 1)));

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
  *two = 2;

  int *three = (int *)orig_malloc(sizeof(int));
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
