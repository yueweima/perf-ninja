#include "solution.hpp"
#include <algorithm>

bool solution(MatrixOfDoubles &in, MatrixOfDoubles &out) {
  int size = in.size();
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      out[i][j] = in[j][i];
    }
  }
  return out[0][size - 1];
}

bool mySolution(MatrixOfDoubles &in, MatrixOfDoubles &out) {
  int size = in.size();
  int tileSize = 64; // Choose an appropriate tile size based on your system's cache size.

  for (int i = 0; i < size; i += tileSize) {
    for (int j = 0; j < size; j += tileSize) {
      // Compute the bounds of the current tile.
      int tileMaxI = std::min(i + tileSize, size);
      int tileMaxJ = std::min(j + tileSize, size);

      for (int ii = i; ii < tileMaxI; ii++) {
        for (int jj = j; jj < tileMaxJ; jj++) {
          out[ii][jj] = in[jj][ii];
        }
      }
    }
  }
  return out[0][size - 1];
}


bool facit(MatrixOfDoubles& in, MatrixOfDoubles& out) {
  static constexpr int TILE_SIZE = 16;
  int size = in.size();
  for (int ii = 0; ii < size; ii+= TILE_SIZE) {
    for (int jj = 0; jj < size; jj+= TILE_SIZE) {
      for (int i = ii; i < std::min(ii + TILE_SIZE, size); i++) {
        for (int j = jj; j < std::min(jj + TILE_SIZE, size); j++) {
          out[i][j] = in[j][i];
        }
      }
    }
  }

  return out[0][size - 1];
}