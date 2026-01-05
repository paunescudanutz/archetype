#include "main.h"

#include "../test/test.h"

FILE* logFile;
struct termios origTermios;

#define RUN_TESTS

void initApp(App* app, Arena* arena) {
  initArena(arena, MB(1));

  *app = (App){
      .masterArena = arena,
  };
}

int main(int argc, char* argv[]) {
  initLogger();
  logInfo("Started:");

#ifdef RUN_TESTS
  runTests();
#endif

  Arena arena = {0};
  App app = {0};
  initApp(&app, &arena);

  // your code here
  printf("hello");

  logInfo("Stopped!");
  return 0;
}
