#include "Matrix.hpp"
#include <cmath>

using Matrix8x8f = array<array<float, 8>, 8>;

const float rsqrt_8 = 1.0f / sqrtf(8.0f);
const float Pi = 3.141593f;

Matrix8x8f operator * (const Matrix8x8f &mat1, const Matrix8x8f &mat2)
{
	Matrix8x8f result;

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			result[i][j] = 0.0f;
			for (int k = 0; k < 8; k++) {
				result[i][j] += mat1[i][k] * mat2[k][j];
			}
		}
	}

	return result;
}

Matrix8x8f operator ^ (const Matrix8x8f &mat1, const Matrix8x8f &mat2)
{
	Matrix8x8f result;

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			result[i][j] = mat1[i][j] * mat2[i][j];
		}
	}

	return result;
}

Matrix8x8f operator / (const Matrix8x8f &mat1, const Matrix8x8f &mat2)
{
	Matrix8x8f result;

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			result[i][j] = mat1[i][j] / mat2[i][j];
		}
	}

	return result;
}

// Transpose
Matrix8x8f operator ~ (const Matrix8x8f &mat)
{
	Matrix8x8f result;

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			result[i][j] = mat[j][i];
		}
	}

	return result;
}

Matrix8x8f Prepare(const Matrix8x8ui8 &inputMat)
{
	Matrix8x8f result;

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			result[i][j] = inputMat[i][j] - 128;
		}
	}

	return result;
}

Matrix8x8ui8 Prepare(const Matrix8x8f &inputMat)
{
	Matrix8x8ui8 result;

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			result[i][j] = inputMat[i][j] + 128;
		}
	}

	return result;
}

Matrix8x8f BuildDCT8x8()
{
	Matrix8x8f result;

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			result[i][j] = (i == 0) ? rsqrt_8 : 0.5f * cosf((2 * j + 1) * i * Pi / 16);
		}
	}

	return result;
}

Matrix8x8i8 Round(const Matrix8x8f &mat)
{
	Matrix8x8i8 result;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			result[i][j] = static_cast<int8_t>(mat[i][j] + 0.5f);
		}
	}
	return result;
}

Matrix8x8f ConvertTo8x8f(const Matrix8x8i8 &mat)
{
	Matrix8x8f result;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			result[i][j] = mat[i][j];
		}
	}
	return result;
}

Matrix8x8i8 BuildQuantizationMatrix(const float quality)
{
	Matrix8x8i8 result;

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			result[i][j] = 1 + ((1 + i + j) * quality);
		}
	}

	return result;
}

Matrix8x8i8 Encode(const Matrix8x8ui8 &inputMat, const Matrix8x8i8 &q)
{
	Matrix8x8f mat = Prepare(inputMat);
	Matrix8x8f dctMat = BuildDCT8x8();
	mat = dctMat * mat * ~dctMat;
	Matrix8x8f qf = ConvertTo8x8f(q);
	mat = mat / qf;
	return Round(mat);
}

Matrix8x8ui8 Decode(const Matrix8x8i8 &inputMat, const Matrix8x8i8 &q)
{
	Matrix8x8f qf = ConvertTo8x8f(q);
	Matrix8x8f mat = ConvertTo8x8f(inputMat);
	mat = mat ^ qf;
	Matrix8x8f dctMat = BuildDCT8x8();
	mat = ~dctMat * mat * dctMat;
	return Prepare(mat);
}

// Mean square error
double MSE(const Matrix8x8ui8 &original, const Matrix8x8ui8 &compressed)
{
	double result = 0.0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			double diff = original[i][j] - compressed[i][j];
			result += diff * diff;
		}
	}
	result /= (8 * 8);
	return result;
}

double PSNR(const Matrix8x8ui8 &original, const Matrix8x8ui8 &compressed)
{
	return 10.0 * log10((255 * 255) / MSE(original, compressed));
}
