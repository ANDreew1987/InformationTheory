#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <cstdint>
#include <array>

using std::array;

using Matrix8x8ui8 = array<array<uint8_t, 8>, 8>;
using Matrix8x8i8 = array<array<int8_t, 8>, 8>;

Matrix8x8i8 BuildQuantizationMatrix(const float quality);
Matrix8x8i8 Encode(const Matrix8x8ui8 &inputMat, const Matrix8x8i8 &q);
Matrix8x8ui8 Decode(const Matrix8x8i8 &inputMat, const Matrix8x8i8 &q);

double PSNR(const Matrix8x8ui8 &original, const Matrix8x8ui8 &compressed);

#endif //MATRIX_HPP
