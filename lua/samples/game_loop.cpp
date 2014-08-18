#include <stdio.h>
#include <unistd.h>
#include <time.h>

clock_t start_time;
int fps = 30;
int frame_count = fps;

void Update() {
  if (frame_count == fps) {
    // http://www.c-lang.net/clock
    start_time = clock();
    frame_count = 0;
  } else {
    frame_count++;
  }
}

void Draw() {
  printf("%d \n", frame_count);
  // call lua func
}

// http://dixq.net/g/03_14.html
void Wait() {
  clock_t took_time = clock() - start_time;
  // TODO fix
  float wait_time = ((frame_count + 1) / fps) - took_time;
  if (wait_time > 0) {
    sleep(wait_time);
  }
}

int main(int argc, char* argv[])
{
  start_time = clock();
  while (1) {
    //printf("start_time %f \n", (double)start_time);
    Update();
    Draw();
    Wait();
    //printf("end_time %f \n", (double)clock());
  }
  return 0;
}
