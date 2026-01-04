#pragma once

#ifdef ASSERTS_ENABLED
#include "logger.h"
#define assert(condition)                                  \
  {                                                        \
    if (!(condition)) {                                    \
      logAssert("condition: [ %s ] is FALSE", #condition); \
      exit(0);                                             \
    }                                                      \
  }
#else
#define assert(expr)
#endif
