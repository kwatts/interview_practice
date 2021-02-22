#ifndef __PREFIX_DICT_HH__
#define __PREFIX_DICT_HH__

#include <string>
#include <string_view>
#include <vector>

class PrefixDictImpl;

/**
 * Holds dictionary of words starting with a given prefix.
 *
 * Values in PrefixDictImpl are stored with the prefix removed.
 */
class PrefixDictImpl {
 public:
  // Default constructor indicates empty bucket.
  PrefixDictImpl();

  /** Construct PrefixDictImpl using word list (sorted).
   *
   * All words must start with the given prefix.
   * An empty word indicates there is an exact word starting with the prefix.
   * (ex: "i" for the bucket[char(i)])
   *
   * max_depth - Maximum **remaining** depth of the PrefixDictImpl tree.
   * num_words_for_buckets - Don't use buckts for small number of words.
   */
  PrefixDictImpl(std::vector<std::string_view> word_suffix, int max_depth,
                 int num_words_for_buckets);

  bool hasPrefix(std::string_view prefix) const;

 private:
  static const int NUM_BUCKETS = 26;

  /** Returns index from [0, NUM_BUCKETS). */
  static int bucketIndex(char c);

  bool has_word_;

  // Only one of these will be filled.
  std::vector<PrefixDictImpl> buckets_;
  std::vector<std::string> remaining_words_;  // sorted
};

class PrefixDict {
 public:
  PrefixDict(std::vector<std::string> const &words, int max_depth,
             int num_words_for_buckets);

  bool hasPrefix(std::string_view prefix) const;

 private:
  PrefixDictImpl impl_;
};

#endif  // __PREFIX_DICT_HH__
