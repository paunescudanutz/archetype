#include "str.h"

#include <stddef.h>
#include <string.h>

#include "allocators.h"
#include "assert.h"
#include "logger.h"

// TODO: introduce a joiner param here for flexibility
size_t strArrayJoin(StrArray array, char *result) {
  size_t size = 0;

  for (int i = 0; i < array.size; i++) {
    Str str = array.list[i];
    memcpy(result + size, str.str, str.size);
    size += str.size;
  }
  return size;
}

void pushStrArray(StrArray *array, Str str) {
  assert(array->size + 1 <= array->capacity);

  array->list[array->size] = str;
  array->size++;
}

void pushTokenArray(TokenArray *array, Token token) {
  assert(array->size + 1 <= array->capacity);

  array->list[array->size] = token;
  array->size++;
}

int strArrayContains(StrArray strArray, Str str) {
  for (int i = 0; i < strArray.size; i++) {
    Str ignored = strArray.list[i];

    if (strEq(str, ignored)) {
      return i;
    }
  }

  return -1;
}

StrArray wrapStrArray(Str *stackBuffer, int capacity) {
  return (StrArray){
      .capacity = capacity,
      .size = 0,
      .list = stackBuffer,
  };
}
StrArray strArrayInit(Arena *arena, size_t capacity) {
  return (StrArray){
      .capacity = capacity,
      .size = 0,
      .list = arenaAlloc(arena, sizeof(Str) * capacity),
  };
}

TokenArray createTokenArray(Arena *arena, size_t capacity) {
  return (TokenArray){
      .capacity = capacity,
      .size = 0,
      .list = arenaAlloc(arena, sizeof(Token) * capacity),
  };
}

int getNextRightToken(TokenArray *tokens, int position) {
  assert(tokens->size > 0);

  if (tokens->size == 1) {
    Token token = tokens->list[0];

    if (position != token.pos.end) {
      return 0;
    }
  }

  for (int i = 0; i < tokens->size - 1; i++) {
    Token first = tokens->list[i];
    if (position < first.pos.end) {
      return i;
    }

    Token second = tokens->list[i + 1];
    if (position < second.pos.end) {
      return i + 1;
    }
  }

  return NO_TOKEN_FOUND;
}

int getNextLeftToken(TokenArray *tokens, int position) {
  assert(tokens->size > 0);

  if (tokens->size == 1) {
    Token token = tokens->list[0];

    if (position != token.pos.start) {
      return 0;
    }
  }

  // TODO: maybe try to iterate like in the right one but check the bounds like in this one to see if the two methods could be merged somehow
  for (int i = tokens->size - 1; i >= 1; i--) {
    Token second = tokens->list[i];
    if (position > second.pos.start) {
      return i;
    }

    Token first = tokens->list[i - 1];
    if (position > first.pos.start) {
      return i - 1;
    }
  }

  return NO_TOKEN_FOUND;
}

Token createToken(Str str, int start, int end) {
  return (Token){
      .pos = (Vec2){.start = start, .end = end},
      .value = sliceStr(str, start, end + 1),
  };
}

// TODO: add an additional output parameter that will ectract a array of bitflags that indicate what kind of information a token has
//  this param should be optional and only computed if the output value pointer is provided
void strTokens(TokenArray *result, Str str, char delimiter, bool tokenizePunctuatuion) {
  assert(result != NULL);
  assert(result->capacity > 0);
  assert(result->size == 0);
  assert(str.size > 0);

  int i = 0;
  int start = 0;
  int end = 0;

  while (true) {
    if (i == str.size) {
      break;
    }

    char c = str.str[i];

    if (tokenizePunctuatuion && (c == '/' || c == '*' || c == '&' || c == '#' || c == '!' || c == '+' || c == '-' || c == '>' || c == '<' || c == ')' || c == '(' || c == ']' || c == '[' || c == '}' || c == '{' || c == ',' || c == '.' || c == '=' || c == ';' || c == '(' || c == ')')) {
      if (start < end) {
        pushTokenArray(result, createToken(str, start, end - 1));

        start = i + 1;
        end = start;
      }

      pushTokenArray(result, createToken(str, i, i));

      start = i + 1;
      end = start;
    } else if (c == delimiter) {
      if (start < end) {
        pushTokenArray(result, createToken(str, start, end - 1));
      }

      start = i + 1;
      end = start;
    } else if (i == str.size - 1) {
      pushTokenArray(result, createToken(str, start, end));
    } else {
      end++;
    }

    i++;
  }
}

void strFill(Str str, char c) {
  memset(str.str, c, str.size);
}

int strSeekFirstNonBlank(Str str) {
  size_t size = str.size;

  if (size == 0) {
    return 0;
  }

  for (int i = 0; i < size; i++) {
    if (str.str[i] != ' ') {
      return i;
    }
  }

  return -1;
}

size_t cStringSize(char *str) {
  size_t size = 0;
  while (str[size] != '\0') {
    size++;
  };

  return size--;
}

bool strEqCString(Str str, char *cStr) {
  if (str.str == cStr) {
    return true;
  }

  int i = 0;
  for (; i < str.size; i++) {
    if (str.str[i] != cStr[i]) {
      return false;
    }
  }

  if (cStr[i] != '\0') {
    return false;
  }

  return true;
}

bool strEq(Str t1, Str t2) {
  if (t1.size != t2.size) {
    return false;
  }

  if (t1.str == t2.str) {
    return true;
  }

  int i = 0;
  for (; i < t1.size; i++) {
    if (t1.str[i] != t2.str[i]) {
      return false;
    }
  }

  return true;
}

Str wrapStr(char *cStr) {
  Str str;
  str.size = cStringSize(cStr);
  str.str = cStr;
  return str;
}

Str wrapStrN(char *cStr, size_t size) {
  Str str;
  str.size = size;
  str.str = cStr;
  return str;
}

Str copyStr(Arena *arena, Str original) {
  size_t size = original.size;

  Str str = {
      .size = size,
      .str = arenaAlloc(arena, size),
  };

  memcpy(str.str, original.str, size);

  return str;
}

Str newStr(Arena *arena, char *cStr) {
  size_t size = cStringSize(cStr);

  Str str = {
      .size = size,
      .str = arenaAlloc(arena, size),
  };

  memcpy(str.str, cStr, size);

  return str;
}

// Think about how to solve this;
Str allocStr(Arena *arena, size_t size) {
  Str str = {
      .size = size,
      .str = arenaAlloc(arena, size),
  };

  return str;
}

bool isBlank(Str str) {
  return str.size == 0 || str.str == NULL;
}

int charIndex(Str src, char c, int startIndex) {
  assert(startIndex < src.size);

  for (int i = startIndex; i < src.size; i++) {
    if (src.str[i] == c) {
      return i;
    }
  }

  return -1;
}

Str sliceStr(Str src, int a, int b) {
  assert(b >= a);

  int size = b - a;
  if (size == 0) {
    size = 1;
  }

  return (Str){
      .size = size,
      .str = &src.str[a],
  };
}

void toCString(Str str, char *cStr) {
  memcpy(cStr, str.str, str.size);
  cStr[str.size] = '\0';
}
