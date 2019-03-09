#include <iostream>
#include <fstream>
#include <cstring>
#include <stdexcept>
#include <cmath>

#include "Matrix.hpp"

using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::runtime_error;

const char optionQuality[]      = "-q";
const char optionInputMatrix[]  = "-m";
const char optionEncodedData[]  = "-e";
const char optionDecodedData[]  = "-d";

const char errOpenInputFile[]   = "Could not open input file";
const char errOpenOutputFile[]  = "Could not open output file";
const char errWrite[]           = "Could not write data to output file";
const char errRead[]            = "Could not read data from input file";

const char help[] =
        "Usage: DCT -q [value] -m [input file] -e [output file] -d [output file]\n";

void ReadMatrix(
    ifstream &in,
    Matrix8x8ui8 &mat) noexcept(false)
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
void SaveMatrix(
    ofstream &out,
    T &mat) noexcept(false)
{
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            out << std::to_string(mat[i][j]) << " ";
        }
        out << endl;
    }
}

template<typename T>
void PrintMatrix(
        T &mat) noexcept(false)
{
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            cout << std::to_string(mat[i][j]) << " ";
        }
        cout << endl;
    }
    cout << endl;
}

double eqm(const Matrix8x8ui8 &original, const Matrix8x8ui8 &compressed) {
    double result = 0.0;
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            double diff = original[i][j] - compressed[i][j];
            result += diff * diff;
        }
    }
    result /= (8 * 8);
    return result;
}

double psnr(const Matrix8x8ui8 &original, const Matrix8x8ui8 &compressed) {
    return 10.0 * log10((255 * 255) / eqm(original, compressed));
}

int main(int argc, char *argv[]) {

    int exitCode = EXIT_SUCCESS;

    ifstream inputMatrix;
    ofstream encodedFile;
    ofstream decodedFile;

    try{
        if (argc != 9) {
            // Неправильное количество аргументов
            throw runtime_error(help);
        }

        if (strcmp(argv[1], optionQuality) != 0) {
            // Неверный аргумент
            throw runtime_error(help);
        }

        float quality = atoff(argv[2]);

        if (strcmp(argv[3], optionInputMatrix) != 0) {
            // Неверный аргумент
            throw runtime_error(help);
        }

        inputMatrix.open(argv[4]);
        if (!inputMatrix.is_open()) {
            // Не удалось открыть входной файл
            throw runtime_error(errOpenInputFile);
        }

        if (strcmp(argv[5], optionEncodedData) != 0) {
            // Неверный аргумент
            throw runtime_error(help);
        }

        encodedFile.open(argv[6]);
        if (!encodedFile.is_open()) {
            // Не удалось открыть выходной файл
            throw runtime_error(errOpenOutputFile);
        }

        if (strcmp(argv[7], optionDecodedData) != 0) {
            // Неверный аргумент
            throw runtime_error(help);
        }

        decodedFile.open(argv[8]);
        if (!decodedFile.is_open()) {
            // Не удалось открыть выходной файл
            throw runtime_error(errOpenOutputFile);
        }

        Matrix8x8ui8 matrix;
        ReadMatrix(inputMatrix, matrix);
        PrintMatrix(matrix);
        Matrix8x8f p = matrix - 128;
        PrintMatrix(p);
        Matrix8x8f dct = BuildDCT();
        PrintMatrix(dct);
        Matrix8x8f pdct = dct * p * ~dct;
        PrintMatrix(pdct);
        Matrix8x8f q = BuildQuantizationMatrix(quality);
        PrintMatrix(q);
        pdct = pdct / q;
        PrintMatrix(pdct);
        Matrix8x8i8 encoded = Round(pdct);
        PrintMatrix(encoded);
        //SaveMatrix(encodedFile, encoded);

        Matrix8x8f dec = encoded ^ q;
        /*Matrix8x8f dec;
        for(int i = 0; i < 8; i++){
            for(int j = 0; j < 8; j++){
                dec[i][j] = encoded[i][j] * q[i][j];
            }
        }*/
        PrintMatrix(dec);
        dec = ~dct * dec * dct;
        PrintMatrix(dec);
        Matrix8x8ui8 decoded = dec + 128;
        PrintMatrix(decoded);
        //SaveMatrix(decodedFile, decoded);
        cout << "PSNR: " << std::to_string(psnr(matrix, decoded)) << endl;

    } catch(runtime_error &e) {
        cout << e.what() << endl;
        exitCode = EXIT_FAILURE;
    }

    if (inputMatrix.is_open()) {
        inputMatrix.close();
    }

    if (encodedFile.is_open()) {
        encodedFile.close();
        // Если произошла ошибка при обработке
        // (системная ошибка либо неправильные данные)
        if (exitCode == EXIT_FAILURE) {
            // Удаляем выходной файл
            remove(argv[5]);
        }
    }

    if (decodedFile.is_open()) {
        decodedFile.close();
        // Если произошла ошибка при обработке
        // (системная ошибка либо неправильные данные)
        if (exitCode == EXIT_FAILURE) {
            // Удаляем выходной файл
            remove(argv[7]);
        }
    }

    return exitCode;
}