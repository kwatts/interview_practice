#ifndef INTERVIEW_PRACTICE_CACHE_MEMORY_H_
#define INTERVIEW_PRACTICE_CACHE_MEMORY_H_

#include <cstdlib>

// API for user controlled cache.

// This is an example of an API that allows low-level memory control on the CPU
// cache. Memory allocated with cache_malloc() is low-latency.

// Allocate bytes on the cache.
void* cache_malloc(std::size_t size);

// Free memory on cache. ptr must be the return value of previous call to
// cache_malloc.
void cache_free(void* ptr);

// Copy data to/from cache. src or dst must be on the cache.
void cache_memcpy(void* dst, void const* src, std::size_t size);

#endif  // INTERVIEW_PRACTICE_CACHE_MEMORY_H_
