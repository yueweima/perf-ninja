#include "solution.hpp"
#include <immintrin.h> // Include header for SIMD intrinsics (e.g., AVX)
#define SOLUTION

#ifdef SOLUTION
uint16_t checksum(const Blob &blob) {
    uint16_t acc = 0;
    constexpr size_t chunkSize = 8; // Example chunk size (adjust as needed)
    for (size_t i = 0; i < blob.size(); i += chunkSize) {
        // Load chunk of data into SIMD register
        __m128i chunk = _mm_loadu_si128(reinterpret_cast<const __m128i*>(&blob[i]));
        
        // Perform SIMD addition
        __m128i sum = _mm_add_epi16(chunk, _mm_set1_epi16(acc));
        
        // Extract results from SIMD register
        alignas(16) uint16_t result[8];
        _mm_store_si128(reinterpret_cast<__m128i*>(result), sum);
        
        // Accumulate results
        for (size_t j = 0; j < chunkSize; ++j) {
            acc += result[j];
        }
    }
    return acc;
}
// uint16_t checksum(const Blob& blob) {
//   uint32_t acc = 0;
//   for (const auto value : blob) {
//     acc += value;
//   }

//   auto high = acc >> 16;
//   auto low = acc & 0xFFFFu;
//   acc = low + high;

//   high = acc >> 16;
//   low = acc & 0xFFFFu;
//   acc = low + high;

//   return static_cast<uint16_t>(acc);
// }
#else
uint16_t checksum(const Blob &blob) {
  uint16_t acc = 0;
  for (auto value : blob) {
    acc += value;
    acc += acc < value; // add carry
  }
  return acc;
}
#endif
