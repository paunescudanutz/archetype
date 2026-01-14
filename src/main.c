#include "main.h"

#include "../test/test.h"

FILE* logFile;
struct termios origTermios;

#define RUN_TESTS

void initApp(App* app) {
  initLogger();
  logInfo("Started:");

#ifdef RUN_TESTS
  runTests();
#endif

  Arena* arena = arenaCreate(MB(1));

  *app = (App){
      .masterArena = arena,
  };
}

void releaseResources(App* app) {
  fclose(logFile);
}

int main(int argc, char* argv[]) {
  App app = {0};
  initApp(&app);

  // your code here
  printf("hello");

  logInfo("Stopped!");
  releaseResources(&app);
  return 0;
}
