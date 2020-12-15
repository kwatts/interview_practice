#include "convolution.h"

#include <cassert>
#include <cstdlib>
#include <cstring>

void convolve2D(const float* input, int width, int height, const float* kernel,
                int k_size, int k_padding, float* output) {
  // k_padding must be within range of kernel size.
  assert(k_padding >= 0 && k_padding < k_size);
  
  // Allocate cache rows with proper padding. This avoids out-of-bounds checks
  // in the inner loop.
  const int row_stride = width + (k_size - 1);

  // For kx=[0, k_size)
  // int src_x = out_x + kx - k_padding;
  
  // Depending on k_padding, the input data will be loaded as:
  // [ DATA _ _ ] or [ _ _ DATA ] or [ _ DATA _ ]
  // in_x = out_x - x_offset + kx;
  const int x_offset = k_padding;

  // Allocate K image rows in the cache.
  float* input_cache =
      static_cast<float*>(cache_malloc(row_stride * k_size * sizeof(float)));
  std::memset(input_cache, 0, row_stride * k_size * sizeof(float));

  // Copy input data into the cache. Use kernel_padding to determine initial
  // number of rows.
  for (int out_y = 0; out_y < k_size; ++out_y) {
    const int src_y = out_y - k_padding;
    if (src_y < 0) { continue; }
    
    cache_memcpy(input_cache + out_y * row_stride + x_offset,
                 input + src_y * width, width * sizeof(float));
  }

  float* kernel_cache =
      static_cast<float*>(cache_malloc(k_size * k_size * sizeof(float)));
  cache_memcpy(kernel_cache, kernel, k_size * k_size * sizeof(float));

  float* output_cache =
      static_cast<float*>(cache_malloc(width * sizeof(float)));

  for (int y = 0; y < height; ++y) {
    std::memset(output_cache, 0, width * sizeof(float));

    for (int x = 0; x < width; ++x) {
      for (int ky = 0; ky < k_size; ++ky) {
        for (int kx = 0; kx < k_size; ++kx) {
          output_cache[x] += kernel_cache[kx + ky * k_size] *
                             input_cache[x + kx + ky * row_stride];
        }
      }
    }

    cache_memcpy(output + y * width, output_cache, width * sizeof(float));

    // Rotate K-1 rows of input data in cache to front of cache. (stole this
    // from a candidate)
    // Note: This uses (potentially expensive) memcpy instead of rotating the
    // pointers.
    cache_memcpy(input_cache, input_cache + row_stride,
                 row_stride * (k_size - 1) * sizeof(float));

    // Input row for next input line.
    const int src_y = y + k_size - k_padding;
    if (src_y < height) {
      cache_memcpy(input_cache + (k_size - 1) * row_stride + x_offset,
                   input + src_y * width, width * sizeof(float));
    } else {
      // Fill cache with zeros.
      std::memset(input_cache + (k_size - 1) * row_stride, 0,
                  row_stride * sizeof(float));
    }
  }

  cache_free(output_cache);
  cache_free(kernel_cache);
  cache_free(input_cache);
}

void convolve2D_slow(const float* input, int width, int height,
                     const float* kernel, int k_size, int k_padding,
                     float* output) {
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      output[x + y * width] = 0;
      for (int ky = 0; ky < k_size; ++ky) {
        for (int kx = 0; kx < k_size; ++kx) {
          int src_x = x + kx - k_padding;
          int src_y = y + ky - k_padding;

          if (src_x < 0 || src_x >= width || src_y < 0 || src_y >= height) {
            continue;
          }

          output[x + y * width] +=
              input[src_x + src_y * width] * kernel[kx + ky * k_size];
        }
      }
    }
  }
}
