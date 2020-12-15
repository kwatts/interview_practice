#include "cache_memory.h"

// Compute N*N convolution over an input image.
//
// Input image is width*height, stored row-major.
// Kernel is k_size*k_size, stored row-major.
// Output image size = input image size.
//
// Kernel "padding" parameter controls offset of kernel origin.
// out_x = in_x + kx - k_padding
// kx ranges from [0, k_size).
//
// Out-of-bounds reads are clamped to zero.
//
// Output array must be initialized to the correct size.
void convolve2D(const float* input, int width, int height, const float* kernel,
                int k_size, int k_padding, float* output);

// Naive version of convolve2D, unoptimized.
void convolve2D_slow(const float* input, int width, int height,
                     const float* kernel, int k_size, int k_padding,
                     float* output);
