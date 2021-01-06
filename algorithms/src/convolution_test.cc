#include "convolution.h"

#include <cassert>
#include <map>
#include <string>
#include <vector>

#include <gtest/gtest.h>

class SimpleImage {
 public:
  SimpleImage(int width, int height) : width_(width), height_(height) {
    data_.resize(width * height, 0);
  }

  SimpleImage(SimpleImage const& other) = delete;
  SimpleImage& operator=(SimpleImage const& other) = delete;

  SimpleImage(SimpleImage&& other) = default;
  SimpleImage& operator=(SimpleImage&& other) = default;

  int width() const { return width_; }
  int height() const { return height_; }

  float& operator()(int r, int c) { return data_[r * width_ + c]; }
  const float& operator()(int r, int c) const { return data_[r * width_ + c]; }

  const float* data() const { return data_.data(); }
  float* data() { return data_.data(); }

 private:
  int width_, height_;
  std::vector<float> data_;
};

void ExpectImagesEqual(const SimpleImage& img1, const SimpleImage& img2) {
  ASSERT_EQ(img1.width(), img2.width());
  ASSERT_EQ(img1.height(), img2.height());

  for (int row = 0; row < img1.height(); ++row) {
    for (int col = 0; col < img1.width(); ++col) {
      EXPECT_EQ(img1(row, col), img2(row, col)) << "Mismatch at row=" << row
                                                << ", col=" << col;
    }
  }
}

TEST(Convolution, conv_1x1) {
  SimpleImage kernel(1, 1);
  kernel.data()[0] = 5;

  SimpleImage input_image(64, 64);
  for (int row = 0; row < input_image.height(); ++row) {
    for (int col = 0; col < input_image.width(); ++col) {
      input_image(row, col) = col + row * input_image.width() * 1000;
    }
  }

  SimpleImage output_image(input_image.width(), input_image.height());
  convolve2D_slow(input_image.data(), input_image.width(), input_image.height(),
                  kernel.data(), kernel.width(), /* padding */ 0,
                  output_image.data());

  // Expected output for all pixels.
  for (int row = 0; row < input_image.height(); ++row) {
    for (int col = 0; col < input_image.width(); ++col) {
      EXPECT_EQ(output_image(row, col), input_image(row, col) * 5);
    }
  }

  // Compare slow vs fast implementation.
  SimpleImage fast_output(input_image.width(), input_image.height());
  convolve2D(input_image.data(), input_image.width(), input_image.height(),
             kernel.data(), kernel.width(), /* padding */ 0,
             fast_output.data());

  ExpectImagesEqual(output_image, fast_output);
}

TEST(ConvSlow, conv_3x3_single_pt) {
  // Create simple kernel k(1, 1) = 1, and 0 elsewhere.
  SimpleImage kernel(3, 3);
  kernel(1, 1) = 1.0;

  SimpleImage input_image(64, 64);
  for (int row = 0; row < input_image.height(); ++row) {
    for (int col = 0; col < input_image.width(); ++col) {
      input_image(row, col) = col + row * input_image.width() * 1000;
    }
  }

  const int padding = kernel.width() / 2;

  SimpleImage output_image(input_image.width(), input_image.height());
  convolve2D_slow(input_image.data(), input_image.width(), input_image.height(),
                  kernel.data(), kernel.width(), padding, output_image.data());

  ExpectImagesEqual(input_image, output_image);
}

class ConvPadding : public ::testing::TestWithParam<int> {};

TEST_P(ConvPadding, conv_3x3_pad) {
  // Create simple kernel k(1, 1) = 1, and 0 elsewhere.
  SimpleImage kernel(3, 3);
  kernel(1, 1) = 1.0;

  SimpleImage input_image(64, 64);
  for (int row = 0; row < input_image.height(); ++row) {
    for (int col = 0; col < input_image.width(); ++col) {
      input_image(row, col) = col + row * input_image.width() * 1000;
    }
  }

  const int padding = GetParam();

  SimpleImage output_image(input_image.width(), input_image.height());
  convolve2D_slow(input_image.data(), input_image.width(), input_image.height(),
                  kernel.data(), kernel.width(), padding, output_image.data());

  SimpleImage fast_output(input_image.width(), input_image.height());
  convolve2D(input_image.data(), input_image.width(), input_image.height(),
             kernel.data(), kernel.width(), padding, fast_output.data());

  ExpectImagesEqual(output_image, fast_output);
}

INSTANTIATE_TEST_SUITE_P(Conv3x3Padding, ConvPadding,
                         ::testing::Range<int>(0, 3));

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
