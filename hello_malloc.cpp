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
  alloc_block.next = &alloc_block;
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
  free_block.next = (HeapBlock *)((uint8_t *)(head_block + 1) + size);
  free_block.size -= sizeof(HeapBlock *) + size;

  return head_block + 1;
}

void orig_free(void *ptr) {
  HeapBlock* block_head = (HeapBlock *)(ptr) - 1;

  // TODO fix prev pointer at malloc
  block_head->next->prev = block_head->prev;
  block_head->prev->next = block_head->next;

  if (alloc_block.prev == ptr) {
    alloc_block.prev = block_head->prev;
  } else if (alloc_block.next == ptr) {
    alloc_block.next = block_head->prev;
    alloc_block.prev = block_head->prev->prev;
  }

  if (free_block.prev == ptr) {
    HeapBlock* lastBlock = alloc_block.next;
    while(lastBlock != NULL) {
      lastBlock = lastBlock->next;
    }
    free_block.prev = lastBlock;
    free_block.next = (HeapBlock *)((int8_t *)lastBlock + lastBlock->size);
  }

  alloc_block.size -= sizeof(HeapBlock *) + sizeof(ptr);
  free_block.size += sizeof(HeapBlock *) + sizeof(ptr);
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
  assert(zero_head->prev == &alloc_block);
  assert(zero_head->next == (HeapBlock *)one - 1);

  struct HeapBlock* one_head = (HeapBlock *)one - 1;
  assert(one_head->prev == (struct HeapBlock *)zero - 1);
  assert(one_head->next == NULL);


  orig_free(zero);
  assert(((HeapBlock *)one - 1)->prev == &alloc_block);
  assert(((HeapBlock *)one - 1)->next == NULL);
  printf("free_block.size  %d\n", free_block.size);
  printf("alloc_block.size  %d\n", alloc_block.size);
  assert(alloc_block.size == sizeof(HeapBlock *) + sizeof(one));
  assert(free_block.size == sizeof(arena) - (sizeof(HeapBlock *) + sizeof(one)));
  orig_free(one);

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
