#include "solution.hpp"


// static std::size_t mapToBucket(std::size_t v) {    // diff
//   if (v >= 0 && v < 13)        return 0; // 13
//   else if (v >= 13 && v < 29)  return 1; // 16
//   else if (v >= 29 && v < 41)  return 2; // 12
//   else if (v >= 41 && v < 53)  return 3; // 12
//   else if (v >= 53 && v < 71)  return 4; // 18
//   else if (v >= 71 && v < 83)  return 5; // 12
//   else if (v >= 83 && v < 100) return 6; // 17
//   return -1;  // let it crash
// }

// int buckets[100] = {
// // 1  2  3  4  5  6  7  8  9  0  1  2  3  4  5  6  7  8  9  0
//    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
//    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
//    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
//    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
//    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6
// };

// static size_t mapToBucket(size_t v) {
//   if (v < (sizeof (buckets) / sizeof (int)))
//     return buckets[v];
//   return -1; // let it crash
// }

// std::array<size_t, NUM_BUCKETS> histogram(const std::vector<int> &values) {
//   std::array<size_t, NUM_BUCKETS> retBuckets{0};
//   for (auto v : values) {
//     retBuckets[mapToBucket(v)]++;
//   }
//   return retBuckets;
// }

// // Define an array to map values to buckets
// int bucketMap[100] {
// // 1  2  3  4  5  6  7  8  9  0  1  2  3  4  5  6  7  8  9  0
//   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//   2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
//   3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
//   4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
//   5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
//   6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6
// };

const size_t bucketMap[100] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6};

static size_t mapToBucket(size_t v) {
    // Check if the value is within the bounds of the array
    if (v >= 0 && v < sizeof(bucketMap) / sizeof(bucketMap[0])) {
        return bucketMap[v];
    } else {
        return -1;  // Let it crash if v is out of bounds
    }
}

std::array<std::size_t, NUM_BUCKETS> histogram(const std::vector<int> &values) {
  std::array<std::size_t, NUM_BUCKETS> retBuckets{0};
  for (auto v : values) {
    retBuckets[mapToBucket(v)]++;
  }
  return retBuckets;
}
