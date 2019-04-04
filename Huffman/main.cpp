#include <iostream>
#include <fstream>
#include <cstring>
#include <stdexcept>
#include <map>
#include <vector>
#include <queue>
#include <memory>

#include "Tree.hpp"

using std::cout;
using std::endl;
using std::ifstream;
using std::runtime_error;
using std::out_of_range;
using std::map;
using std::vector;
using std::priority_queue;
using std::unique_ptr;

const char errOpenInputFile[]   = "Could not open input file";
const char errRead[]            = "Could not read data from input file";
const char errBadData[]         = "Bad input data";

const char help[] =
    "Usage: Huffman [input file]";

// Частоты встречаемости символов
using FrequencyTable = map<char, int>;
using HuffmanQueue = priority_queue<Node*, vector<Node*>, NodeComparator>;

const FrequencyTable myVariantTable = {
        { 'a', 52 }, { 'b',  7 }, { 'c', 18 }, { 'd',  14 }, { 'e', 56 },
        { 'f', 12 }, { 'g', 18 }, { 'h', 33 }, { 'i',  28 }, { 'j',  1 },
        { 'k',  5 }, { 'l', 43 }, { 'm', 10 }, { 'n',  35 }, { 'o', 28 },
        { 'p',  7 }, { 'r', 34 }, { 's', 30 }, { 't',  62 }, { 'u', 10 },
        { 'v',  8 }, { 'w',  8 }, { 'y',  9 }, { '_', 135 }, { ',',  1 }
};

FrequencyTable BuildFrequencyTable(ifstream &in)
{
	FrequencyTable result;
	char data;
	while (in.read(&data, 1)) {
		try {
			result.at(data)++;
		} catch(out_of_range &ex) {
			result[data] = 1;
		}
	}
	return result;
}

void PrintFrequencyTable(const FrequencyTable &tbl) {
	for (auto t : tbl) {
		cout << "[" << t.first << "] - [" << t.second << "]" << endl;
	}
}

Tree* BuildHuffmatTree(const FrequencyTable &tbl) {
	HuffmanQueue q;
	for (auto fr : tbl) {
		q.push(new Node(fr.first, fr.second));
	}
	while (q.size() > 1) {
		Node* node1 = q.top();
		q.pop();
		Node* node2 = q.top();
		q.pop();

		q.push(new Node(node1, node2));
	}
	return new Tree(q.top());
}

void PrintCodeTable(const HuffmanCodeTable &tbl) {
    for(auto i: tbl){
        cout << i.first << ": " << i.second << endl;
    }
}

float GetCompressionCoef(
    const FrequencyTable &inputTbl,
    const HuffmanCodeTable &hufTbl)
{
    int inputBitsCount = 0;
    for(auto i: inputTbl) {
        inputBitsCount += i.second * 8;
    }
    int huffmanBitsCount = 0;
    for(auto i: hufTbl) {
        int freq = inputTbl.at(i.first);
        huffmanBitsCount += freq * i.second.size();
    }
    return float(inputBitsCount) / float(huffmanBitsCount);
}

int main(int argc, char *argv[]) {

    int exitCode = EXIT_SUCCESS;

    ifstream inputFile;

    try {
        FrequencyTable workTable;
        if (argc == 1) {
            workTable = myVariantTable;
            cout << "Use default frequency table" << endl;
        } else if (argc == 2) {
            inputFile.open(argv[1], std::ios::binary);
            workTable = BuildFrequencyTable(inputFile);
            inputFile.close();
        } else {
            // Неправильное количество аргументов
            throw runtime_error(help);
        }

        cout << "Frequency table:" << endl;
		PrintFrequencyTable(workTable);
		cout << endl;

		unique_ptr<Tree> huffmanTree(BuildHuffmatTree(workTable));

        cout << "Huffman tree:" << endl;
		huffmanTree->PrintTree();
        cout << endl;

        HuffmanCodeTable codeTable = huffmanTree->GetCodeTable();
        cout << "Huffman table:" << endl;
        PrintCodeTable(codeTable);
        cout << endl;

        cout << "Compression: " <<  GetCompressionCoef(workTable, codeTable) << endl;

    } catch(runtime_error &e) {
        cout << e.what() << endl;
        exitCode = EXIT_FAILURE;
    }

    if (inputFile.is_open()) {
        inputFile.close();
    }

    return exitCode;
}