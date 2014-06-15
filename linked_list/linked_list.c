#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct Cell {
  struct Cell* prev;
  struct Cell* next;
  int value;
};

struct Cell* firstCell = NULL;

struct Cell* create(int value) {
  // 間違えてCell*にしてしまったので、複数回createを実行すると、
  // Cell同士で番地が被り、意図しない代入が行われてしまった
  // 別途注意: http://www.ss.cs.meiji.ac.jp/CCP055.html
  struct Cell* cell = (struct Cell*)malloc(sizeof(struct Cell));
  cell->prev = NULL;
  cell->next = NULL;
  cell->value = value;
  return cell;
}

void push(struct Cell* cell) {
  if (firstCell == NULL) {
    firstCell = cell;
  } else {
    struct Cell* lastCell = firstCell;
    while (lastCell->next != NULL) {
      lastCell = lastCell->next;
    }
    lastCell->next = cell;
    cell->prev = lastCell;
  }
}

struct Cell* pop() {
  if (firstCell == NULL) {
    return firstCell;
  }
  struct Cell* lastCell = firstCell;
  while (lastCell->next != NULL) {
    lastCell = lastCell->next;
  }

  if (lastCell->prev == NULL) {
    firstCell = NULL;
  } else {
    lastCell->prev->next = NULL;
  }
  return lastCell;
}

void removeCell(struct Cell* cell) {
  struct Cell* targetCell = firstCell;
  while (targetCell->next != NULL) {
    if (targetCell == cell) {
      break;
    }
    targetCell = targetCell->next;
  }

  targetCell->prev->next = targetCell->next;
  targetCell->next->prev = targetCell->prev;
}

struct Cell* find(int value) {
  if (firstCell == NULL) {
    return NULL;
  }
  struct Cell* targetCell = firstCell;
  while (targetCell != NULL) {
    if (targetCell->value == value) {
      break;
    }
    targetCell = targetCell->next;
  }
  return targetCell;
}

int main() {
  // TEST create()
  struct Cell* cell_0 = create(0);
  assert(cell_0->value == 0);

  struct Cell* cell_1 = create(1);
  assert(cell_0->value == 0);
  assert(cell_1->value == 1);

  struct Cell* cell_2 = create(2);
  assert(cell_0->value == 0);
  assert(cell_1->value == 1);
  assert(cell_2->value == 2);

  printf("CREATED\n");
  printf("cell_0->value: %d\n", cell_0->value);
  printf("cell_1->value: %d\n", cell_1->value);
  printf("cell_2->value: %d\n", cell_2->value);


  // TEST find()
  // 1. 一つもpushされていない時はNULL
  assert(find(0) == NULL);


  // TEST push()
  push(cell_0);
  push(cell_1);
  push(cell_2);


  // TEST find()
  // 2. 存在しない時はNULL
  assert(find(-1) == NULL);
  assert(find(3) == NULL);
  struct Cell* f_res_0 = find(0);
  struct Cell* f_res_1 = find(1);
  struct Cell* f_res_2 = find(2);
  // 3. 正しいアドレスが返る
  assert(f_res_0 == cell_0);
  assert(f_res_1 == cell_1);
  assert(f_res_2 == cell_2);
  // 4. 正しいvalueが返る
  assert(f_res_0->value == cell_0->value);
  assert(f_res_1->value == cell_1->value);
  assert(f_res_2->value == cell_2->value);


  // TEST Linked list
  // 正しい状態でリンクされている
  assert(f_res_0->prev == NULL);
  assert(f_res_0->next == f_res_1);
  assert(f_res_1->prev == f_res_0);
  assert(f_res_1->next == f_res_2);
  assert(f_res_2->next == NULL);


  // TEST pop()
  struct Cell* p_res_0 = pop();
  struct Cell* p_res_1 = pop();
  struct Cell* p_res_2 = pop();
  struct Cell* p_res_null = pop();
  // 正しい順番でpopされる
  assert(p_res_0 == cell_2);
  assert(p_res_1 == cell_1);
  assert(p_res_2 == cell_0);
  assert(p_res_null == NULL);
  // すべてpopされたあとはNULLが代入される
  assert(firstCell == NULL);

  printf("ALL CELLS ARE POPED!\n");


  // 初期状態と同じ状態になった、という想定での再テスト
  // TEST create() again
  struct Cell* cell_3 = create(3);
  assert(cell_3->value == 3);
  // TEST push() again
  push(cell_3);
  // TEST find() again
  // 存在しない時はNULL
  assert(find(-1) == NULL);
  struct Cell* f_res_3 = find(3);
  // 正しいアドレスが返る
  assert(f_res_3 == cell_3);
  // 正しいvalueが返る
  assert(f_res_3->value == cell_3->value);
  // TEST pop() again
  struct Cell* p_res_3 = pop();
  assert(p_res_3 == cell_3);
  // すべてpopされたあとはNULLが代入される
  assert(firstCell == NULL);
}
