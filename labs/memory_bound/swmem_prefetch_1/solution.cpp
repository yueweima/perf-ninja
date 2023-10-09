#include "solution.hpp"

static int getSumOfDigits(int n) {
  int sum = 0;
  while (n != 0) {
    sum = sum + n % 10;
    n = n / 10;
  }
  return sum;
}

int solution(const hash_map_t *hash_map, const std::vector<int> &lookups) {
  int result = 0;

  for (int val : lookups) {
    if (hash_map->find(val))
      result += getSumOfDigits(val);
  }

  return result;
}

int mySolution(const hash_map_t *hash_map, const std::vector<int> &lookups) {
  int result = 0;

  for (int val : lookups) {
    if (hash_map->find(val)){
      result += getSumOfDigits(val);
      // Prefetch the next value in the 'lookups' vector
      if (val + 16 < lookups.size()) {
        // __builtin_prefetch(&lookups[val + 16], 0, 1);
        hash_map->prefetchForVal(lookups[val + 16]);
      }
    }
  }

  return result;
}

constexpr int look_ahead = 16;

int facit(const hash_map_t *hash_map, const std::vector<int> &lookups) {
  int result = 0;

  for (size_t i = 0; i < lookups.size() - look_ahead; i++) {
    int val = lookups[i];
    if (hash_map->find(val)){
      result += getSumOfDigits(val);
    }
    hash_map->prefetchForVal(lookups[i + look_ahead]);
  }

  for (size_t i = lookups.size() - look_ahead; i < lookups.size(); i++) {
    int val = lookups[i];
    if (hash_map->find(val)){
      result += getSumOfDigits(val);
    }
  }
  
  return result;
}