#ifndef INTERVIEW_PRACTICE_TESTING_UTIL_H_
#define INTERVIEW_PRACTICE_TESTING_UTIL_H_

#include <cstdio>
#include <cstdlib>

#define LIKELY(x) __builtin_expect(!!(x), 1)
#define UNLIKELY(x) __builtin_expect(!!(x), 0)

#define CHECK(_val, _msg)                                          \
  do {                                                             \
    if (UNLIKELY(!(_val))) {                                       \
      fprintf(stderr, "Assertion failed!\n");                      \
      fprintf(stderr, "Expression: " #_val "\n");                  \
      fprintf(stderr, "File: %s, line: %d\n", __FILE__, __LINE__); \
      fprintf(stderr, "Message: %s\n", _msg);                      \
      abort();                                                     \
    }                                                              \
  } while (0)

#define CHECK_EQ(_val1, _val2, _msg)                                   \
  do {                                                                 \
    if (UNLIKELY((_val1) != (_val2))) {                                \
      fprintf(stderr, "Assertion failed!\n");                          \
      fprintf(stderr, "Expression: " #_val1 " != " #_val2 "\n");       \
      fprintf(stderr, "Value (left): %d\n", static_cast<int>(_val1));  \
      fprintf(stderr, "Value (right): %d\n", static_cast<int>(_val2)); \
      fprintf(stderr, "File: %s, line: %d\n", __FILE__, __LINE__);     \
      fprintf(stderr, "Message: %s\n", _msg);                          \
      abort();                                                         \
    }                                                                  \
  } while (0)

#endif  // INTERVIEW_PRACTICE_TESTING_UTIL_H_
