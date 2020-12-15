#include "algorithm.h"

#include <cstdlib>

#include "testing_util.h"

using namespace algorithm;

void testReverseInts() {
  constexpr int kArray[] = {0, 1, 2, 3, 4, 5};
  constexpr int kNumValues = sizeof(kArray) / sizeof(kArray[0]);
  int array_cp[kNumValues];
  copy(&kArray[0], &kArray[kNumValues], &array_cp[0]);

  reverse(&array_cp[0], &array_cp[0] + kNumValues);

  for (int i = 0; i < kNumValues; ++i) {
    int j = kNumValues - i - 1;
    CHECK(array_cp[i] == kArray[j], "Value mismatch!");
  }
}

int main(int argc, char **argv) {
  testReverseInts();

  return 0;
}
