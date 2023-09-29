
#include "solution.h"
#include <memory>
#include <immintrin.h> // Include the AVX2 header
#include <iostream>

void imageSmoothing(const InputVector &input, uint8_t radius,
                    OutputVector &output) {
  int pos = 0;
  int currentSum = 0;
  int size = static_cast<int>(input.size());

  // 1. left border - time spend in this loop can be ignored, no need to
  // optimize it
  for (int i = 0; i < std::min<int>(size, radius); ++i) {
    currentSum += input[i];
  }

  int limit = std::min(radius + 1, size - radius);
  for (pos = 0; pos < limit; ++pos) {
    currentSum += input[pos + radius];
    output[pos] = currentSum;
  }

  // 2. main loop. During optimization, focus mainly on this part
  limit = size - radius;

  for (; pos < limit; ++pos) {
    currentSum -= input[pos - radius - 1];
    currentSum += input[pos + radius];
    output[pos] = currentSum;
  }

  // 3. special case, executed only if size <= 2*radius + 1
  limit = std::min(radius + 1, size);
  for (; pos < limit; pos++) {
    output[pos] = currentSum;
  }

  // 4. right border - time spend in this loop can be ignored, no need to
  // optimize it
  for (; pos < size; ++pos) {
    currentSum -= input[pos - radius - 1];
    output[pos] = currentSum;
  }
}


void mySolution(const InputVector &input, uint8_t radius,
                    OutputVector &output) {
  int pos = 0;
  int currentSum = 0;
  int size = static_cast<int>(input.size());

  // 1. left border - time spend in this loop can be ignored, no need to
  // optimize it
  for (int i = 0; i < std::min<int>(size, radius); ++i) {
    currentSum += input[i];
  }

  int limit = std::min(radius + 1, size - radius);
  for (pos = 0; pos < limit; ++pos) {
    currentSum += input[pos + radius];
    output[pos] = currentSum;
  }

  // 2. main loop.
  limit = size - radius;
  __m256i currentSumV = _mm256_set1_epi32(currentSum); // Broadcast currentSum to a 256-bit vector
  for (; pos < limit; ++pos) {
    // Load 8 integer values from input into a 256-bit vector
    __m256i subtractValue = _mm256_set1_epi32(input[pos - radius - 1]);

    currentSumV = _mm256_sub_epi32(currentSumV, subtractValue);

    // Add the new value to the sliding window
    __m256i addValue = _mm256_set1_epi32(input[pos + radius]);
    currentSumV = _mm256_add_epi32(currentSumV, addValue);

    // Extract the lowest 32-bit integer from the result vector and store it in currentSum
    currentSum = _mm256_extract_epi32(currentSumV, 0);

    // Store the result into the output vector
    output[pos] = currentSum;
  }

  // 3. special case, executed only if size <= 2*radius + 1
  limit = std::min(radius + 1, size);
  for (; pos < limit; pos++) {
    output[pos] = currentSum;
  }

  // 4. right border - time spend in this loop can be ignored, no need to
  // optimize it
  for (; pos < size; ++pos) {
    currentSum -= input[pos - radius - 1];
    output[pos] = currentSum;
  }
}

void mySolutionA(const InputVector &input, uint8_t radius,
                    OutputVector &output) {
  int pos = 0;
  int currentSum = 0;
  int size = static_cast<int>(input.size());

  // 1. left border - time spend in this loop can be ignored, no need to
  // optimize it
  for (int i = 0; i < std::min<int>(size, radius); ++i) {
    currentSum += input[i];
  }

  int limit = std::min(radius + 1, size - radius);
  for (pos = 0; pos < limit; ++pos) {
    currentSum += input[pos + radius];
    output[pos] = currentSum;
    // std::cout <<"pos:" <<  pos << " output:" << output[pos] << std::endl;
  }
  
  limit = size - radius;
  const uint8_t* subtractPtr = input.data() + pos - radius - 1;
  const uint8_t* addPtr = input.data() + pos + radius;
  uint16_t* outputPtr = const_cast<uint16_t*>(output.data()) + pos; // Change to mutable pointer
  __m128i current = _mm_set1_epi16(currentSum);
  
  // std::cout <<"currentSum :" << currentSum << std::endl;
  // std::cout <<"pos:" <<  pos << " output:" << output[pos] << std::endl;

  int i = 0;
  // loop processes 16 elements per iteration
  for (; i + 15 < limit - pos; i += 16) {
      // 1. Calculate vector differences for the first 8 elements
      __m128i sub_u8_0 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(subtractPtr + i));
      __m128i sub_0 = _mm_cvtepu8_epi16(sub_u8_0);
      __m128i add_u8_0 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(addPtr + i));
      __m128i add_0 = _mm_cvtepu8_epi16(add_u8_0);

      __m128i diff_0 = _mm_sub_epi16(add_0, sub_0);

      // 1. Calculate vector differences for the next 8 elements
      __m128i sub_u8_1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(subtractPtr + i + 8));
      __m128i sub_1 = _mm_cvtepu8_epi16(sub_u8_1);
      __m128i add_u8_1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(addPtr + i + 8));
      __m128i add_1 = _mm_cvtepu8_epi16(add_u8_1);

      __m128i diff_1 = _mm_sub_epi16(add_1, sub_1);

      // 2. Calculate vector prefix sums for the first 8 elements
      __m128i s_0 = _mm_add_epi16(diff_0, _mm_slli_si128(diff_0, 2));
      s_0 = _mm_add_epi16(s_0, _mm_slli_si128(s_0, 4));
      s_0 = _mm_add_epi16(s_0, _mm_slli_si128(s_0, 8));

      // 2. Calculate vector prefix sums for the next 8 elements
      __m128i s_1 = _mm_add_epi16(diff_1, _mm_slli_si128(diff_1, 2));
      s_1 = _mm_add_epi16(s_1, _mm_slli_si128(s_1, 4));
      s_1 = _mm_add_epi16(s_1, _mm_slli_si128(s_1, 8));

      // 3. Store the results for the first 8 elements
      __m128i result_0 = _mm_add_epi16(s_0, current);
      _mm_storeu_si128(reinterpret_cast<__m128i*>(outputPtr + i), result_0);

      // 3. Store the results for the next 8 elements
      __m128i result_1 = _mm_add_epi16(s_1, current);
      _mm_storeu_si128(reinterpret_cast<__m128i*>(outputPtr + i + 8), result_1);

      // 4. Broadcast currentSum for the next iteration
      currentSum = (uint16_t)_mm_extract_epi16(result_1, 7); // Extract the highest 16-bit value
      current = _mm_set1_epi16(currentSum);
      std::cout <<"pos:" <<  pos << " output:" << output[pos] << std::endl;
      std::cout <<"i:" <<  i << std::endl;
  }
  
  pos += i;

  std::cout <<"pos:" <<  pos << std::endl;

  // keep the sequential loop to process the reminder
  for (; pos < limit; ++pos) {
    currentSum -= input[pos-radius -1];
    currentSum += input[pos+radius];
    output[pos] = currentSum;
    std::cout <<"pos:" <<  pos << " output:" << output[pos] << std::endl;
  }

  // 3. special case, executed only if size <= 2*radius + 1
  limit = std::min(radius + 1, size);
  for (; pos < limit; pos++) {
    output[pos] = currentSum;
  }

  // 4. right border - time spend in this loop can be ignored, no need to
  // optimize it
  for (; pos < size; ++pos) {
    currentSum -= input[pos - radius - 1];
    output[pos] = currentSum;
  }
}

void facit(const InputVector &input, uint8_t radius,
                    OutputVector &output) {
  int pos = 0;
  int currentSum = 0;
  int size = static_cast<int>(input.size());

  // 1. left border - time spend in this loop can be ignored, no need to
  // optimize it
  for (int i = 0; i < std::min<int>(size, radius); ++i) {
    currentSum += input[i];
  }

  int limit = std::min(radius + 1, size - radius);
  for (pos = 0; pos < limit; ++pos) {
    currentSum += input[pos + radius];
    output[pos] = currentSum;
  }
    
  limit = size - radius;
  const uint8_t* subtractPtr = input.data() + pos - radius - 1;
  const uint8_t* addPtr = input.data() + pos + radius;
  const uint16_t* outputPtr = output.data() + pos;
  __m128i current = _mm_set1_epi16(currentSum);

  int i = 0;
  // loop processes 8 elements per iteration
  for (; i + 7 < limit - pos; i += 8) {
    // 1. Calculate vector diff: input[i + radius] - input[i-radius -1]
    __m128i sub_u8 = _mm_loadu_si64(subtractPtr + i);
    __m128i sub = _mm_cvtepu8_epi16(sub_u8);
    __m128i add_u8 = _mm_loadu_si64(addPtr + i);
    __m128i add = _mm_cvtepu8_epi16(add_u8);

    __m128i diff = _mm_sub_epi16(add, sub);

    // 2. calcualte vector prefix sub for 8 elements
    __m128i s = _mm_add_epi16(diff, _mm_slli_si128(diff, 2));
    s = _mm_add_epi16(s, _mm_slli_si128(s, 4));
    s = _mm_add_epi16(s, _mm_slli_si128(s, 8));

    // 3. store the result
    __m128i result = _mm_add_epi16(s, current);
    _mm_storeu_si128((__m128i*)(outputPtr + i), result);

    // 4. brodcast currentSum for the next iteration
    currentSum = (uint16_t)_mm_extract_epi16(result, 7);
    current = _mm_set1_epi16(currentSum);
  }
  pos += i;

  // keep the sequential loop to process the reminder
  for (; pos < limit; ++pos) {
    currentSum -= input[pos-radius -1];
    currentSum += input[pos+radius];
    output[pos] = currentSum;
  }

  // 3. special case, executed only if size <= 2*radius + 1
  limit = std::min(radius + 1, size);
  for (; pos < limit; pos++) {
    output[pos] = currentSum;
  }

  // 4. right border - time spend in this loop can be ignored, no need to
  // optimize it
  for (; pos < size; ++pos) {
    currentSum -= input[pos - radius - 1];
    output[pos] = currentSum;
  }
}