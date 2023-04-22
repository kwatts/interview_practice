// LeetCode 1610
// https://leetcode.com/problems/maximum-number-of-visible-points/

#include <gtest/gtest.h>

#include <cmath>
#include <deque>
#include <valarray>
#include <vector>

using namespace std;

class Solution {
 public:
  int visiblePoints(vector<vector<int>> const& points, int angle,
                    vector<int> const& location) {
    double fov = angle * M_PI / 180;  // radians

    int coincident_points = 0;

    // Compute the angle relative to the start location.
    vector<double> angles;
    for (const auto& pt : points) {
      // Special case for coincident points (visible with any angle)
      if (pt[0] == location[0] && pt[1] == location[1]) {
        coincident_points++;
      } else {
        angles.push_back(atan2(pt[1] - location[1], pt[0] - location[0]));
      }
    }
    sort(angles.begin(), angles.end());

    // Simple trick to solve singularity above PI: Extend point angles around
    // the M_PI singularity
    const int num_angles = angles.size();  // Unmodified array size.
    for (int i = 0; i < num_angles; ++i) {
      if (angles[i] < fov - M_PI) {
        angles.push_back(angles[i] + 2 * M_PI);
      }
    }

    // Iterate through the point angles, count max visible points.
    int max_visible = 0;
    std::deque<double> visible_angles;
    for (const auto& angle : angles) {
      visible_angles.push_back(angle);
      while (!visible_angles.empty() && visible_angles.front() < angle - fov) {
        visible_angles.pop_front();
      }

      max_visible = max(max_visible, static_cast<int>(visible_angles.size()));
    }

    return max_visible + coincident_points;
  }
};

TEST(SolutionTest, lcTests) {
  EXPECT_EQ(
      3, Solution().visiblePoints(vector<vector<int>>{{2, 1}, {2, 2}, {3, 3}},
                                  90, vector<int>{1, 1}));
  EXPECT_EQ(4, Solution().visiblePoints(
                   vector<vector<int>>{{2, 1}, {2, 2}, {3, 4}, {1, 1}}, 90,
                   vector<int>{1, 1}));
  EXPECT_EQ(1, Solution().visiblePoints(vector<vector<int>>{{1, 0}, {2, 1}}, 13,
                                        vector<int>{1, 1}));
}

// Test near singularity of +M_PI
TEST(SolutionTest, singularityTests) {
  EXPECT_EQ(3, Solution().visiblePoints(
                   vector<vector<int>>{{-2, 1}, {-2, -1}, {-2, 0}}, 90,
                   vector<int>{0, 0}));

  EXPECT_EQ(2, Solution().visiblePoints(
                   vector<vector<int>>{{-2, 1}, {-2, -1}, {1, 0}}, 90,
                   vector<int>{0, 0}));
}

TEST(SolutionTest, lcFailureTests) {
  // LeetCode checks that original solution failed on.
  EXPECT_EQ(4, Solution().visiblePoints(
                   vector<vector<int>>{
                       {1, 1}, {2, 2}, {3, 3}, {4, 4}, {1, 2}, {2, 1}},
                   0, vector<int>{1, 1}));

  EXPECT_EQ(
      3, Solution().visiblePoints(vector<vector<int>>{{1, 1}, {1, 1}, {1, 1}},
                                  1, vector<int>{1, 1}));
}
