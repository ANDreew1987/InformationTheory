#include <iostream>
#include <fstream>
#include <cstring>

using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::hex;

enum ActionType {
    atUnknown = 0,
    atPack,
    atUnpack
};

const char optionPack[] = "-p";
const char optionUnpack[] = "-u";
const char optionInputFile[] = "-i";
const char optionOutputFile[] = "-o";

const uint8_t rleMagic = 0xC0;
const uint8_t maxCount = 0x3F;

void RLE_Pack(ifstream &in, ofstream &out) {
    // Последовательно проходим по входному файлу
    char prevData;
    in.read(&prevData, 1);
    char data;
    in.read(&data, 1);
    uint8_t count = 1;
    while(!in.eof()) {
        if (count == maxCount) {
            out << char(rleMagic | count) << prevData;
            count = 1;
            continue;
        }
        if (data == prevData){
            count++;
        } else {
            if (count > 1) {
                out << char(rleMagic | count);
            } else if (uint8_t(prevData) >= rleMagic) {
                out << char(rleMagic | 1);
            }
            out << prevData;
            prevData = data;
            count = 1;
        }
        in.read(&data, 1);
    }
    if (count > 1) {
        out << char(rleMagic | count);
    } else if (uint8_t(prevData) >= rleMagic) {
        out << char(rleMagic | 1);
    }
    out << prevData;
}

void RLE_Unpack(ifstream &in, ofstream &out) {
    // Последовательно проходим по входному файлу
    char dataRaw;
    in.read(&dataRaw, 1);
    while(!in.eof()) {
        // Если текущйи байт является счётчиком
        if (uint8_t(dataRaw) >= rleMagic){
            // Получаем значение счётчика
            uint8_t count = (dataRaw ^ rleMagic);
            // Считываем следующий байт (байт данных)
            char data;
            in.read(&data, 1);
            // Записываем байт данных необходимое количество раз
            for (int i = 0; i < count; i++) {
                out << data;
            }
        } else {
            // Текущий байт - байт данных
            // Просто запиисываем его
            out << dataRaw;
        }
        in.read(&dataRaw, 1);
    }
}

void printHelp() {
    cout << "Usage: RLE [option] -i [input file] -o [output file]" << endl;
    cout << "option:" << endl;
    cout << "  -p: pack" << endl;
    cout << "  -u: unpack" << endl;
}

int main(int argc, char *argv[]) {
    if (argc < 6){
        printHelp();
        return EXIT_FAILURE;
    }
    ActionType act = atUnknown;
    if (strcmp(argv[1], optionPack) == 0) {
        act = atPack;
    } else if (strcmp(argv[1], optionUnpack) == 0) {
        act = atUnpack;
    } else {
        printHelp();
        return EXIT_FAILURE;
    }
    if (strcmp(argv[2], optionInputFile) != 0) {
        printHelp();
        return EXIT_FAILURE;
    }
    ifstream inputFile(argv[3], std::ios::binary);
    if (!inputFile){
        cout << "Could not open input file" << endl;
        return EXIT_FAILURE;
    }
    if (strcmp(argv[4], optionOutputFile) != 0) {
        printHelp();
        return EXIT_FAILURE;
    }
    ofstream outputFile(argv[5], std::ios::binary);
    if (!outputFile){
        cout << "Could not open output file" << endl;
        return EXIT_FAILURE;
    }
    switch(act){
    case atPack:
        RLE_Pack(inputFile, outputFile);
        break;
    case atUnpack:
        RLE_Unpack(inputFile, outputFile);
        break;
    }
    inputFile.close();
    outputFile.close();
    return EXIT_SUCCESS;
}