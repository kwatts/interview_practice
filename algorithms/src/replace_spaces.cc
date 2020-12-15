#include <assert.h>
#include <iostream>
#include <string>

// Replaces all spaces in a string with "%20". String has
// enough room at end for additional characters.
void replaceSpaces(char *begin, char *end, int len) {
  char *rbegin = end - 1;
  char *src = begin + len - 1;
  char *const rend = begin - 1;

  while (src != rend) {
    if (*src != ' ') {
      std::swap(*src--, *rbegin--);
    } else {
      src--;
      *rbegin-- = '0';
      *rbegin-- = '2';
      *rbegin-- = '%';
    }
  }

  // Verify the input string has the correct number of spaces at end.
  assert(rbegin == src);
}

bool testReplaceSpaces(const std::string &in, const std::string &expected) {
  std::string modified = in;
  modified.resize(expected.size(), ' ');

  replaceSpaces(&modified[0], &modified[modified.size()], in.size());
  bool result = modified == expected;

  if (result) {
    std::cerr << "PASS: \"" << in << "\"\n";
  } else {
    std::cerr << "FAIL!\n";
    std::cerr << "Input: \"" << in << "\"\n";
    std::cerr << "Output: \"" << modified << "\"\n";
    std::cerr << "Expected: \"" << expected << "\"\n";
    std::cerr << "\n";
  }

  return result;
}

int main(int argc, char **argv) {
  bool result = true;

  result = testReplaceSpaces("foobar", "foobar") && result;
  result = testReplaceSpaces(" ", "%20") && result;
  result = testReplaceSpaces("  ", "%20%20") && result;
  result = testReplaceSpaces("1 ", "1%20") && result;
  result = testReplaceSpaces(" 1", "%201") && result;
  result = testReplaceSpaces(" abc ", "%20abc%20") && result;

  if (result) {
    return 0;
  }

  std::cerr << "Tests failed!\n";
  return 1;
}
