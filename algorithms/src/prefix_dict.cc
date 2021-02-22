#include "prefix_dict.hh"

#include <algorithm>
#include <cassert>
#include <locale>
#include <string>
#include <string_view>
#include <vector>

namespace {

bool startsWith(std::string_view s, std::string_view prefix) {
  if (static_cast<int>(s.size()) < static_cast<int>(prefix.size())) {
    return false;
  }

  for (int i = 0; i < static_cast<int>(prefix.size()); ++i) {
    if (s.at(i) != prefix.at(i)) {
      return false;
    }
  }

  return true;
}

std::string_view subStringOrEmpty(std::string_view s, size_t pos) {
  if (pos >= s.size()) {
    return std::string_view("");
  }

  return s.substr(pos);
}

}  // namespace

PrefixDictImpl::PrefixDictImpl() : has_word_(false) {}

PrefixDictImpl::PrefixDictImpl(std::vector<std::string_view> word_suffix,
                               int max_depth, int num_words_for_buckets) {
  // Check if we have empty word in input list.
  auto word_cbegin = word_suffix.cbegin();
  if (word_cbegin->empty()) {
    has_word_ = 1;
    ++word_cbegin;
  }

  // Input is too small for buckets, use remaining_words_.
  if (std::distance(word_cbegin, word_suffix.cend()) <= num_words_for_buckets ||
      max_depth == 0) {
    while (word_cbegin != word_suffix.cend()) {
      remaining_words_.push_back(std::string(*word_cbegin));

      ++word_cbegin;
    }

    return;
  }

  assert(word_cbegin != word_suffix.cend());  // Can't be empty at this point.

  // Fill remaining buckets using sorted words.
  buckets_.resize(NUM_BUCKETS);

  auto word_char_end = word_cbegin;
  do {
    // Find last value that starts with current char.
    // Note: Could be done using std::lower_bound in O(log(n)) time.
    while (word_char_end != word_suffix.cend() &&
           word_char_end->at(0) == word_cbegin->at(0)) {
      word_char_end++;
    }

    // post condition: word_char_end points to end of range that starts with
    // same char as "word_cbegin".

    char prefix_char = word_cbegin->at(0);
    int bucket_index = bucketIndex(prefix_char);

    // Repack into string_view's with first character removed.
    std::vector<std::string_view> words_with_char_prefix;
    while (word_cbegin != word_char_end) {
      words_with_char_prefix.push_back(subStringOrEmpty(*word_cbegin, 1));
      ++word_cbegin;
    }

    buckets_[bucket_index] = PrefixDictImpl(
        words_with_char_prefix, max_depth - 1, num_words_for_buckets);
  } while (word_char_end != word_suffix.cend());
}

bool PrefixDictImpl::hasPrefix(std::string_view prefix) const {
  // Base case: We have a word matching this prefix.
  if (prefix.empty()) {
    return has_word_;
  }

  if (buckets_.empty()) {
    const auto iter = std::lower_bound(remaining_words_.begin(),
                                       remaining_words_.end(), prefix);

    if (iter == remaining_words_.end()) {
      return false;
    }

    // iter points to a string that does not compare less than prefix.
    // Based on string sorting rules, this will be either:
    // - prefix + suffix (suffix can be empty)
    // - another string, comparing greater than prefix.
    return startsWith(*iter, prefix);
  }

  const char next_c = prefix[0];
  return buckets_[bucketIndex(next_c)].hasPrefix(subStringOrEmpty(prefix, 1));
}

int PrefixDictImpl::bucketIndex(char c) {
  std::locale loc;
  int index = std::tolower(c, loc) - 'a';

  assert(index >= 0);
  assert(index < NUM_BUCKETS);
  return index;
}

PrefixDict::PrefixDict(std::vector<std::string> const& words, int max_depth,
                       int num_words_for_buckets) {
  // TODO - Verify that all words have values in [a-z].
  assert(std::is_sorted(words.begin(), words.end()));
  assert(max_depth >= 0);
  assert(num_words_for_buckets >= 0);

  std::vector<std::string_view> word_view;
  for (const std::string& w : words) {
    word_view.push_back(w);
  }
  impl_ = PrefixDictImpl(word_view, max_depth, num_words_for_buckets);
}

bool PrefixDict::hasPrefix(std::string_view prefix) const {
  return impl_.hasPrefix(prefix);
}
