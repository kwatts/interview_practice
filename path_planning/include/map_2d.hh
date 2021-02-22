#ifndef __MAP_2D_HH_
#define __MAP_2D_HH_

#include <cassert>
#include <vector>

// Row/col coordinates of cell.
struct Cell {
  int row;
  int col;

  Cell(int r, int c) : row(r), col(c) {}

  Cell(Cell const &other) = default;
  Cell &operator=(Cell const &other) = default;

  bool operator==(Cell const &other) const {
    return row == other.row && col == other.col;
  }

  bool operator!=(Cell const &other) const { return !(*this == other); }
};

/** Obstacles and costs arrayed on 2D map. */
template <typename ValueT>
class Map2D {
 public:
  Map2D(int width, int height)
      : Map2D(width, height, std::vector<ValueT>(width * height)) {}

  Map2D(int width, int height, std::vector<ValueT> const &values)
      : width_(width), height_(height), values_(values) {
    assert((int)values.size() == width_ * height_);
    assert(width_ > 0);
    assert(height_ > 0);
  }

  void fill(ValueT value) { std::fill(values_.begin(), values_.end(), value); }

  int getWidth() const { return width_; }
  int getHeight() const { return height_; }

  /** Returns cost for cell. Crashes for out-of-bounds values. */
  ValueT const &getCost(Cell cell) const { return values_[computeIndex(cell)]; }

  /** Sets cost for cell. Crashes for out-of-bounds values. */
  ValueT &getCost(Cell cell) { return values_[computeIndex(cell)]; }

 private:
  int computeIndex(Cell cell) const {
    assert(cell.row >= 0 && cell.row < height_);
    assert(cell.col >= 0 && cell.col < width_);

    return cell.row * width_ + cell.col;
  }

  const int width_, height_;
  std::vector<ValueT> values_;
};

#endif  // __MAP_2D_HH_
