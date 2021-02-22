#include "prefix_dict.hh"

#include <cassert>
#include <map>
#include <string>
#include <vector>
#include <gtest/gtest.h>

static const std::vector<std::string> SAMPLE_DICT = {  //
    "a", "abcd",                                       //
    "bcd", "bcde", "bcdf", "bcdg",                     //
    "x", "xy", "xyz"};

TEST(PrefixDict, noDepth) {
  PrefixDict prefix_dict(SAMPLE_DICT, /*max_depth=*/0, 100);

  EXPECT_TRUE(prefix_dict.hasPrefix("a"));
  EXPECT_TRUE(prefix_dict.hasPrefix("ab"));
  EXPECT_TRUE(prefix_dict.hasPrefix("abc"));
  EXPECT_TRUE(prefix_dict.hasPrefix("abcd"));
  EXPECT_FALSE(prefix_dict.hasPrefix("abcde"));

  EXPECT_TRUE(prefix_dict.hasPrefix("b"));
  EXPECT_TRUE(prefix_dict.hasPrefix("bc"));
  EXPECT_FALSE(prefix_dict.hasPrefix("bce"));
  EXPECT_TRUE(prefix_dict.hasPrefix("bcd"));

  EXPECT_FALSE(prefix_dict.hasPrefix("c"));

  EXPECT_TRUE(prefix_dict.hasPrefix("x"));
  EXPECT_TRUE(prefix_dict.hasPrefix("xyz"));

  EXPECT_FALSE(prefix_dict.hasPrefix("z"));
}

// TODO - Turn this into a parameterized test, for different depth sizes.
// Depth = 4: This will build out PrefixDictImpl values for first 4 char's.
TEST(PrefixDict, depth4) {
  PrefixDict prefix_dict(SAMPLE_DICT, /*max_depth=*/4, 0);

  EXPECT_TRUE(prefix_dict.hasPrefix("a"));
  EXPECT_TRUE(prefix_dict.hasPrefix("ab"));
  EXPECT_TRUE(prefix_dict.hasPrefix("abc"));
  EXPECT_TRUE(prefix_dict.hasPrefix("abcd"));
  EXPECT_FALSE(prefix_dict.hasPrefix("abcde"));

  EXPECT_TRUE(prefix_dict.hasPrefix("b"));
  EXPECT_TRUE(prefix_dict.hasPrefix("bc"));
  EXPECT_FALSE(prefix_dict.hasPrefix("bce"));
  EXPECT_TRUE(prefix_dict.hasPrefix("bcd"));

  EXPECT_FALSE(prefix_dict.hasPrefix("c"));

  EXPECT_TRUE(prefix_dict.hasPrefix("x"));
  EXPECT_TRUE(prefix_dict.hasPrefix("xyz"));

  EXPECT_FALSE(prefix_dict.hasPrefix("z"));
}
