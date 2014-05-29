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
  struct Cell* cell = (struct Cell*)malloc(sizeof(struct Cell*));
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
    return firstCell;
  }
  struct Cell* targetCell = firstCell;
  while (targetCell->next != NULL) {
    if (targetCell->value == value) {
      break;
    }
    targetCell = targetCell->next;
  }
  return targetCell;
}

int main() {
  struct Cell* cell_0 = create(0);
  struct Cell* cell_1 = create(1);
  struct Cell* cell_2 = create(2);

  push(cell_0);
  push(cell_1);
  push(cell_2);

  assert(pop() == cell_2);
  assert(pop() == cell_1);
  assert(pop() == cell_0);
  assert(pop() == NULL);
}
