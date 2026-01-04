#include "../lib/logger.h"
#include "../test/test.h"

FILE* logFile;
#define RUN_TESTS

int main(int argc, char* argv[]) {
  initLogger();
  logInfo("Started:");

#ifdef RUN_TESTS
  runTests();
#endif

  printf("hello");

  logInfo("Stopped!");
  return 0;
}
