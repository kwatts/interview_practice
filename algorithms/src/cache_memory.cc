// Implementation of user-controlled CPU cache.
//
// This implementation is just a stub: It just allocates the "cache" on main
// memory, and does some simple assert's to make sure memory accesses are within
// bounds.

#include "cache_memory.h"

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <list>

namespace {

using MemoryBlock = std::pair<void*, std::size_t>;

// Stores allocated memory on the cache.
// Not thread safe.
static std::list<MemoryBlock> kCacheBlocks_;

// Returns true if pointer belongs to allocated cache memory.
bool IsRegionInCache(void const* begin, std::size_t size) {
  const char* roi_begin = reinterpret_cast<const char*>(begin);
  const char* roi_end = roi_begin + size;

  for (const auto& block : kCacheBlocks_) {
    const char* block_begin = reinterpret_cast<const char*>(block.first);
    const char* block_end = block_begin + block.second;

    if (roi_begin >= block_begin && roi_end <= block_end) {
      return true;
    }
  }

  return false;
}

}  // namespace

// TODO(kevinwatts) Use "extern C" for the API functions.

void* cache_malloc(std::size_t size) {
  void* result = std::malloc(size);

  MemoryBlock block{result, size};
  kCacheBlocks_.push_back(block);

  return result;
}

void cache_free(void* ptr) {
  for (auto iter = kCacheBlocks_.begin(); iter != kCacheBlocks_.end(); ++iter) {
    if (iter->first == ptr) {
      std::free(iter->first);
      kCacheBlocks_.erase(iter);

      return;
    }
  }

  assert(false);
}

void cache_memcpy(void* dst, void const* src, std::size_t size) {
  bool dst_in_cache = IsRegionInCache(dst, size);
  bool src_in_cache = IsRegionInCache(src, size);
  assert(dst_in_cache || src_in_cache);

  std::memcpy(dst, src, size);
}
