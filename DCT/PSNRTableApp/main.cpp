#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <stdexcept>

#include "Matrix.hpp"

using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::runtime_error;
using std::fixed;

const char optionInputMatrix[]  = "-m";
const char optionOutputFile[]	= "-o";

const char errOpenInputFile[]   = "Could not open input file";
const char errOpenOutputFile[]	= "Could not open output file";

const char help[] = "Usage: PSNRTable -m [input file] -o [output file]\n";

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

int main(int argc, char *argv[]) {

    int exitCode = EXIT_SUCCESS;

    ifstream inputMatrix;
	ofstream outputFile;

    try{
        if (argc != 5) {
            throw runtime_error(help);
        }

        if (strcmp(argv[1], optionInputMatrix) != 0) {
            throw runtime_error(help);
        }

        inputMatrix.open(argv[2]);
        if (!inputMatrix.is_open()) {
            throw runtime_error(errOpenInputFile);
        }

		if (strcmp(argv[3], optionOutputFile) != 0) {
			throw runtime_error(help);
		}

		outputFile.open(argv[4]);
		if (!outputFile.is_open()) {
			throw runtime_error(errOpenOutputFile);
		}

		outputFile << "Quality;PSNR (dB)" << endl;
		cout << "Quality;PSNR (dB)" << endl;

		Matrix8x8ui8 matrix;
		ReadMatrix(inputMatrix, matrix);

		for (uint8_t q = 0; q < 255; q++) {
			Matrix8x8ui8 qMatrix = BuildQuantizationMatrix(q);
			Matrix8x8i8 encodedMat = Encode(matrix, qMatrix);
			Matrix8x8ui8 decodedMatrix = Decode(encodedMat, qMatrix);
			double psnr = PSNR(matrix, decodedMatrix);
			outputFile << int(q) << ";" << fixed << psnr << endl;
			cout << int(q) << ";" << fixed << psnr << endl;
		}

    } catch(runtime_error &e) {
        cout << e.what() << endl;
        exitCode = EXIT_FAILURE;
    }

    if (inputMatrix.is_open()) {
        inputMatrix.close();
    }

	if (outputFile.is_open()) {
		outputFile.close();
		if (exitCode == EXIT_FAILURE) {
			remove(argv[5]);
		}
	}

    return exitCode;
}