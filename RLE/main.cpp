#include <iostream>
#include <fstream>
#include <cstring>
#include <stdexcept>

using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::runtime_error;

const char optionPack[]         = "-p";
const char optionUnpack[]       = "-u";
const char optionInputFile[]    = "-i";
const char optionOutputFile[]   = "-o";

const char errOpenInputFile[]   = "Could not open input file";
const char errOpenOutputFile[]  = "Could not open output file";
const char errWrite[]           = "Could not write data to output file";
const char errRead[]            = "Could not read data from input file";
const char errBadData[]         = "Bad input data";

const char help[] =
    "Usage: RLE [option] -i [input file] -o [output file]\n"
    "option:\n"
    "  -p: pack\n"
    "  -u: unpack\n";

const char rleMagic2 = 0b11000000;
//const char rleMagic1 = 0b10000000;
const char rleMagic = rleMagic2;
const char maxCount = (~rleMagic);

typedef void (*PFNRLEFUNC) (ifstream &in, ofstream &out);

// Функция записывает символ в поток
// Если запись не удалась, генерируется исключение
inline void WriteChar(
    ofstream &out,
    const char ch) noexcept(false)
{
    if (!out.write(&ch, 1)) {
        throw runtime_error(errWrite);
    }
}

// Функция записывает байт-счётчик (если необходимо) и символ в поток
// Если запись не удалась, генерируется исключение
inline void WriteChar(
    ofstream &out,
    const char ch,
    const uint8_t count) noexcept(false)
{
    if ((ch & rleMagic) == rleMagic || count > 1) {
        WriteChar(out, (rleMagic | count));
    }
    WriteChar(out, ch);
}

// Функция кодирует данные из входного потока в выходной
void RLE_Encode(
    ifstream &in,
    ofstream &out) noexcept(false)
{
    char data, prevData;
    // Считываем байт
    if (!in.read(&data, 1)) {
        // Считать не удалось,
        // Либо входной файл пуст, либо системная ошибка
        throw runtime_error(errRead);
    }
    prevData = data;

    // Счётчик
    uint8_t count = 1;

    // Считываем байт из входного потока, пока это возможно
    while(in.read(&data, 1)) {
        // Если достигли максимального значения счётчика
        if (count == maxCount) {
            // Записываем данные
            WriteChar(out, prevData, count);
            // Сбрасываем счётчик
            count = 1;
            // Начинаем следующую итерацию кодирования
            continue;
        }
        // Если текущий байт равен предыдущему
        if (data == prevData) {
            // Увеличиваем значение счётчика
            count++;
        } else {
            // Записываем данные
            WriteChar(out, prevData, count);
            // Сохраняем текущий байт
            prevData = data;
            // Сбрасываем счётчик
            count = 1;
        }
    }

    // Проверяем наличие ошибки при чтении
    if (in.rdstate() == std::ios_base::failbit ||
        in.rdstate() == std::ios_base::badbit)
    {
        throw runtime_error(errRead);
    }

    // Успешно достигли конца входного файла
    // Записываем последний байт
    WriteChar(out, data, count);
}

// Функция декодирует данные из входного потока в выходной
void RLE_Decode(
    ifstream &in,
    ofstream &out) noexcept(false)
{
    char data;
    // Счётчик
    uint8_t count = 1;
    // Считываем байт из входного потока, пока это возможно
    while(in.read(&data, 1)) {
        // Проверяем, является ли текущий байт счётчиком
        if ((data & rleMagic) == rleMagic) {
            // Получаем значение счётчика
            count = (data ^ rleMagic);
            // Считываем байт, который нужно повторить
            if (!in.read(&data, 1)) {
                // Ошибка: байт-счётчик есть, а байта данных прочитать не удалось
                throw runtime_error(errBadData);
            }
        }
        // Циклически записываем байт данных согласно значению счётчика
        for (uint8_t i = 0; i < count; i++) {
            WriteChar(out, data);
        }
        // Сбрасываем счётчик
        count = 1;
    }

    // Проверяем наличие ошибки при чтении
    if (in.rdstate() == std::ios_base::failbit ||
        in.rdstate() == std::ios_base::badbit)
    {
        throw runtime_error(errRead);
    }
}

int main(int argc, char *argv[]) {

    int exitCode = EXIT_SUCCESS;

    ifstream inputFile;
    ofstream outputFile;

    try{
        if (argc != 6) {
            // Неправильное количество аргументов
            throw runtime_error(help);
        }

        // Указатель на функцию обработки
        PFNRLEFUNC rleFunc = nullptr;

        if (strcmp(argv[1], optionPack) == 0) {
            // Упаковываем данные
            rleFunc = RLE_Encode;
        } else if (strcmp(argv[1], optionUnpack) == 0) {
            // Распаковываем данные
            rleFunc = RLE_Decode;
        } else {
            // Неверный аргумент
            throw runtime_error(help);
        }

        if (strcmp(argv[2], optionInputFile) != 0) {
            // Неверный аргумент
            throw runtime_error(help);
        }

        inputFile.open(argv[3], std::ios::binary);
        if (!inputFile.is_open()) {
            // Не удалось открыть входной файл
            throw runtime_error(errOpenInputFile);
        }

        if (strcmp(argv[4], optionOutputFile) != 0) {
            // Неверный аргумент
            throw runtime_error(help);
        }
        ofstream outputFile(argv[5], std::ios::binary);
        if (!outputFile.is_open()) {
            // Не удалось открыть выходной файл
            throw runtime_error(errOpenOutputFile);
        }

        // Запускаем обработку
        rleFunc(inputFile, outputFile);

    } catch(runtime_error &e) {
        cout << e.what() << endl;
        exitCode = EXIT_FAILURE;
    }

    if (inputFile.is_open()) {
        inputFile.close();
    }

    if (outputFile.is_open()) {
        outputFile.close();
        // Если произошла ошибка при обработке
        // (системная ошибка либо неправильные данные)
        if (exitCode == EXIT_FAILURE) {
            // Удаляем выходной файл
            remove(argv[5]);
        }
    }

    return exitCode;
}