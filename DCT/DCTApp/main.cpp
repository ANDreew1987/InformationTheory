#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <stdexcept>

#include "Matrix.hpp"

using std::cout;
using std::endl;
using std::ifstream;
using std::runtime_error;
using std::fixed;

const char optionQuality[]      = "-q";
const char optionInputMatrix[]  = "-m";

const char errOpenInputFile[]   = "Could not open input file";

const char help[] = "Usage: DCT -q [value] -m [input file]\n";

void ReadMatrix(ifstream &in, Matrix8x8ui8 &mat)
{
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            int data;
            in >> data;
            mat[i][j] = static_cast<uint8_t>(data);
        }
    }
}

template<typename T>
void PrintMatrix(T &mat)
{
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            cout << int(mat[i][j]) << " ";
        }
        cout << endl;
    }
}

int main(int argc, char *argv[]) {

    int exitCode = EXIT_SUCCESS;

    ifstream inputMatrix;

    try{
        if (argc != 5) {
            // Неправильное количество аргументов
            throw runtime_error(help);
        }

        if (strcmp(argv[1], optionQuality) != 0) {
            // Неверный аргумент
            throw runtime_error(help);
        }

		uint8_t quality = atoi(argv[2]);

        if (strcmp(argv[3], optionInputMatrix) != 0) {
            // Неверный аргумент
            throw runtime_error(help);
        }

        inputMatrix.open(argv[4]);
        if (!inputMatrix.is_open()) {
            // Не удалось открыть входной файл
            throw runtime_error(errOpenInputFile);
        }

		cout << "Quality = " << int(quality) << endl;

        Matrix8x8ui8 matrix;
        ReadMatrix(inputMatrix, matrix);
		cout << "Input matrix:" << endl;
		PrintMatrix(matrix);
		Matrix8x8ui8 qMatrix = BuildQuantizationMatrix(quality);
		cout << "Quantization matrix:" << endl;
		PrintMatrix(qMatrix);
		Matrix8x8i8 encodedMat = Encode(matrix, qMatrix);
		cout << "Encoded matrix:" << endl;
		PrintMatrix(encodedMat);
		Matrix8x8ui8 decodedMatrix = Decode(encodedMat, qMatrix);
		cout << "Decoded matrix:" << endl;
		PrintMatrix(decodedMatrix);
		double psnr = PSNR(matrix, decodedMatrix);
		cout << "PSNR = " << fixed << psnr << " dB" << endl;

    } catch(runtime_error &e) {
        cout << e.what() << endl;
        exitCode = EXIT_FAILURE;
    }

    if (inputMatrix.is_open()) {
        inputMatrix.close();
    }

    return exitCode;
}