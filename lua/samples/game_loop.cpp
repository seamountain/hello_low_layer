#include <stdio.h>
#include <unistd.h>
#include <time.h>

int fps = 30;
int frame_count = fps;

int TARGET_FPS = 60;
// milli seconds per frame
int TARGET_FRAME_TIME = 1000 / TARGET_FPS;


void Update() {
  if (frame_count == TARGET_FPS) {
    // http://www.c-lang.net/clock
    frame_count = 0;
  } else {
    frame_count++;
  }
}

void Draw() {
  // call lua func
  printf("%d \n", frame_count);
}

int main(int argc, char* argv[])
{
  while (true) {
    clock_t begin = clock();

    // GameLoop
    Update();
    Draw();

    // wait
    clock_t end = clock();

    // spent milli seconds
    double diff = (double)(end - begin) / CLOCKS_PER_SEC * 1000;

    // wait
    usleep(TARGET_FRAME_TIME - diff - 1);
    // advanced: wait strictly by switching
    while (TARGET_FRAME_TIME >= ((double)(clock() - begin) / CLOCKS_PER_SEC * 1000)) {
      usleep(0);
    }
  }
  return 0;
}
