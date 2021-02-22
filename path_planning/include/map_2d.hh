#ifndef __OBSTACLE_MAP_HH_
#define __OBSTACLE_MAP_HH_

#include <cassert>
#include <vector>

/** Obstacles and costs arrayed on 2D map. */
template <typename ValueT>
class Map2D {
 public:
  Map2D(int width, int height)
      : Map2D(width, height, std::vector<ValueT>(width * height)) {}

  Map2D(int width, int height, std::vector<ValueT> const &values) {
    width_ = width;
    height_ = height;
    values_ = values;

    assert((int)values.size() == width_ * height_);
    assert(width_ > 0);
    assert(height_ > 0);
  }

  void fill(ValueT value) { std::fill(values_.begin(), values_.end(), value); }

  int getWidth() const { return width_; }
  int getHeight() const { return height_; }

  /** Returns cost for row/col. Crashes for out-of-bounds values. */
  ValueT const &getCost(int row, int col) const {
    return values_[computeIndex(row, col)];
  }

  /** Sets cost for row/col. Crashes for out-of-bounds values. */
  ValueT &getCost(int row, int col) { return values_[computeIndex(row, col)]; }

 private:
  int computeIndex(int row, int col) const {
    assert(row >= 0 && row < height_);
    assert(col >= 0 && col < width_);

    return row * width_ + col;
  }

  int width_, height_;
  std::vector<ValueT> values_;
};

#endif  // __OBSTACLE_MAP_HH_
