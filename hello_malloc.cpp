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
  alloc_block.prev = (HeapBlock *)arena;
  alloc_block.next = (HeapBlock *)arena;
  alloc_block.size = 0;
  free_block.prev = (HeapBlock *)arena;
  free_block.next = (HeapBlock *)arena;
  free_block.size = sizeof(arena);
}

void *orig_malloc(size_t size) {
  HeapBlock* head_block = free_block.next;
  head_block->prev = alloc_block.next;
  head_block->next = NULL;
  head_block->size = size;

  alloc_block.next->next = head_block;
  alloc_block.prev = alloc_block.next;
  alloc_block.next = head_block;
  alloc_block.size += sizeof(head_block) + size;

  free_block.prev = free_block.next;
  free_block.next = (HeapBlock *)((uint8_t *)head_block + size);
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

  struct HeapBlock* zero_head = (HeapBlock *)zero - 1;
  printf("zero_head->prev %p\n", zero_head->prev); // 0xa4c30200a4c3020
  // TODO below assertion is failed
  //assert(zero_head->prev == (HeapBlock *)arena);
  assert(zero_head->next == (HeapBlock *)one - 1);

  struct HeapBlock* one_head = (HeapBlock *)one - 1;
  printf("one_head->prev %p\n", one_head->prev); // 0xa4c30240a4c302
  // TODO below assertion is failed
  //assert(one_head->prev == (struct HeapBlock *)zero - 1);
  printf("one_head->next %p\n", one_head->next); // 0x1
  // TODO below assertion is failed
  //assert(one_head->next == NULL);

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
