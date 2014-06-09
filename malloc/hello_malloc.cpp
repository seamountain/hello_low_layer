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
  printf("&arena %p \n", &arena);
  printf("&arena[array_size] %p \n", &arena[array_size]);
  alloc_block.prev = NULL;
  alloc_block.next = NULL;
  alloc_block.size = 0;
  free_block.prev = NULL;
  free_block.next = (HeapBlock *)arena;
  free_block.size = sizeof(arena);
}

void *orig_malloc(size_t size) {
  HeapBlock* head_block = (HeapBlock *)free_block.next;
  head_block->next = NULL;
  head_block->prev = NULL;
  head_block->size = 0;
  head_block->prev = &alloc_block;
  head_block->next = alloc_block.next;

  if (alloc_block.next != NULL) {
    alloc_block.next->prev = head_block;
  }

  alloc_block.next = head_block;
  head_block->size = size;

  alloc_block.size += sizeof(head_block) + size;

  //free_block.next = (HeapBlock *)((uint8_t *)(head_block + 1) + size);
  if (free_block.next != NULL) {
    if (free_block.next->next != NULL) {
      free_block.next = (HeapBlock *)free_block.next->next;
    } else {
      free_block.next = (HeapBlock *)((uint8_t *)(head_block + 1) + size);
    }
  } else {
    free_block.next = (HeapBlock *)((uint8_t *)(head_block + 1) + size);
  }
  free_block.size -= sizeof(HeapBlock *) + size;

  return head_block + 1;
}

void orig_free(void *ptr) {
  HeapBlock* block_head = (HeapBlock *)(ptr) - 1;

  // alloc_block
  block_head->prev->next = block_head->next;
  if (block_head->next != NULL) {
    block_head->next->prev = block_head->prev;
  }
  alloc_block.size -= sizeof(HeapBlock *) + block_head->size;

  // free_block
  block_head->next = free_block.next;
  block_head->prev = &free_block;
  if (free_block.next != NULL) {
    free_block.next->prev = block_head;
  }
  free_block.next = block_head;

  free_block.size += sizeof(HeapBlock *) + block_head->size;
}

int main() {
  initialize_block();

  printf("--- FIRST TEST START ---\n");

  // TEST initialize_block()
  // 正しい値で初期化されていること
  assert(free_block.size == sizeof(arena));
  assert(alloc_block.prev == NULL);
  assert(alloc_block.next == NULL);
  assert(alloc_block.size == 0);
  assert(free_block.next == (HeapBlock *)&arena);
  assert(free_block.prev == NULL);
  assert(free_block.size == sizeof(arena));

  int* zero = (int *)orig_malloc(sizeof(int));
  *zero = 0;

  // TEST orig_malloc()
  // 正しいサイズが使用されていること
  assert(alloc_block.size == (sizeof(HeapBlock *) + sizeof(*zero)));
  assert(free_block.size == sizeof(arena) - (sizeof(HeapBlock *) + sizeof(*zero)));
  // 正しい位置が返ってくること
  assert(zero == (int *)((HeapBlock *)&arena + 1));

  int* one = (int *)orig_malloc(sizeof(int));
  *one = 1;

  // 正しいサイズが使用されていること
  assert(alloc_block.size == (sizeof(HeapBlock *) * 2 + sizeof(*zero) + sizeof(*one)));
  assert(free_block.size == sizeof(arena) - (sizeof(HeapBlock *) * 2 + sizeof(*zero + sizeof(*one))));

  struct HeapBlock* zero_head = (HeapBlock *)zero - 1;
  struct HeapBlock* one_head = (HeapBlock *)one - 1;

  // 正しい位置が返ってくること
  assert(one == (int *)((int8_t *)((HeapBlock *)arena + 2) + zero_head->size));

  // TEST doubly links
  assert(zero_head->prev == one_head);
  assert(zero_head->next == NULL);
  assert(one_head->prev == &alloc_block);
  assert(one_head->next == zero_head);

  printf("zero %p\n", zero);
  printf("one %p\n", one);

  orig_free(zero);

  // TEST after free
  assert(alloc_block.next == (HeapBlock *)one - 1);
  assert(free_block.next == (HeapBlock *)zero - 1);
  assert(((HeapBlock *)one - 1)->prev == &alloc_block);
  assert(((HeapBlock *)one - 1)->next == NULL);
  assert(alloc_block.size == sizeof(HeapBlock *) + sizeof(int));
  assert(free_block.size == sizeof(arena) - (sizeof(HeapBlock *) + sizeof(int)));

  orig_free(one);

  // TEST after free
  assert(alloc_block.next == NULL);
  assert(free_block.next == (HeapBlock *)one - 1);
  assert(free_block.next->next == (HeapBlock *)zero - 1);
  assert(alloc_block.size == 0);
  assert(free_block.size == sizeof(arena));

  printf("--- FIRST TEST FINISHED ---\n");


  printf("--- SECOND TEST START ---\n");
  // TEST AGAIN
  int* two = (int *)orig_malloc(sizeof(int));
  *two = 2;
  int* three = (int *)orig_malloc(sizeof(int));
  *three = 3;
  int* four = (int *)orig_malloc(sizeof(int));
  *three = 4;

  HeapBlock* two_head = (HeapBlock *)(two - 1);
  HeapBlock* three_head = (HeapBlock *)(three - 1);

  printf("two %p\n", two);
  printf("three %p\n", three);

  // 一度freeされた要素から使用される
  assert(two == (int *)one);
  assert(three == (int *)zero);

  printf("--- SECOND TEST FINISHED ---\n");

  return 0;
}
