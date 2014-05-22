#include <iostream>
#include "cassert"

struct HeapBlock {
  struct HeapBlock *prev;
  struct HeapBlock *next;
  int size;
};

const int array_size = 10000;
int arena[array_size];
struct HeapBlock alloc_block;
/*same with
struct HeapBlock alloc_block2;
struct HeapBlock* alloc_block = &alloc_block2;
*/
struct HeapBlock free_block;

void initialize_block() {
  alloc_block.next = &alloc_block;
  alloc_block.size = 0;
  free_block.prev = (HeapBlock *)arena;
  free_block.next = (HeapBlock *)&arena[0];
  free_block.size = sizeof(arena);
}

void *orig_malloc(size_t size) {
  //struct HeapBlock* head_block = free_block.next;
  void* head_block = free_block.next;
  ((HeapBlock *)head_block)->prev = alloc_block.next;
  alloc_block.next->prev = (HeapBlock *)head_block;
  ((HeapBlock *)head_block)->next = alloc_block.prev;
  ((HeapBlock *)head_block)->size = size;
  alloc_block.next = (HeapBlock *)head_block;
  alloc_block.size += sizeof(HeapBlock *) + size;

  // size(バイト数)分だけ次の番地を取得したい
  //free_block.next = (struct HeapBlock *)((size_t)(head_block + 1) + size);
  free_block.next = (HeapBlock *)((char *)head_block + size);
  free_block.prev = alloc_block.next;
  free_block.size -= sizeof(HeapBlock *) + size;

  printf("free_block.next %p\n", free_block.next);

  return (HeapBlock *)head_block + 1;
}

void orig_free(void *ptr) {
  struct HeapBlock* block_head = (HeapBlock *)(ptr) - sizeof(alloc_block);

  if (block_head && alloc_block.next) {
    alloc_block.next = block_head->next;
  }

  if (block_head && free_block.next) {
    free_block.next = (HeapBlock *)&arena + sizeof(alloc_block);
  }

  free_block.size += sizeof(ptr) + block_head->size;
  alloc_block.size -= sizeof(ptr) + block_head->size;

  block_head->prev = &free_block;
  if (block_head->next) {
    block_head->next->prev = block_head->prev;
  }
  block_head->size = 0;
}

int main() {
  initialize_block();

  assert(alloc_block.size == 0);
  assert(free_block.size == sizeof(arena));

  int* zero = (int *)orig_malloc(sizeof(int));
  *zero = 0;

  assert(alloc_block.size == (sizeof(HeapBlock *) + sizeof(*zero)));
  assert(free_block.size == sizeof(arena) - (sizeof(HeapBlock *) + sizeof(*zero)));

  assert(zero == &arena[0] + (sizeof(HeapBlock) / sizeof(int)));

  int* one = (int *)orig_malloc(sizeof(int));
  *one = 1;

  assert(alloc_block.size == (sizeof(HeapBlock *) * 2 + sizeof(*zero) + sizeof(*one)));
  assert(free_block.size == sizeof(arena) - (sizeof(HeapBlock *) * 2 + sizeof(*zero + sizeof(*one))));

  struct HeapBlock* block = (struct HeapBlock *)zero - 1;
  printf("(struct HeapBlock *)one - 1 %p\n", (struct HeapBlock *)one - 1);
  printf("block->next %p\n", block->next);
  assert((struct HeapBlock *)one - 1 == block->next);
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

  int* two = (int *)orig_malloc(sizeof(int));
  *two = 2;

  int* three = (int *)orig_malloc(sizeof(int));
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
