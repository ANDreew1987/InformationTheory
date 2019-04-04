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
    "Usage: Huffman -i [input file]";

// Частоты встречаемости символов
using FrequencyTable = map<char, int>;
using HuffmanQueue = priority_queue<Node*, vector<Node*>, NodeComparator>;

FrequencyTable BuildFrequencyTable(ifstream &in)
{
	FrequencyTable result;
	char data;
	while (in.read(&data, 1)) {
		try {
			result.at(data)++;
		} catch(out_of_range) {
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

Tree* BuildHuffmatTree(FrequencyTable &tbl) {
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

int main(int argc, char *argv[]) {

    int exitCode = EXIT_SUCCESS;

    ifstream inputFile;

    try{
        if (argc != 2) {
            // Неправильное количество аргументов
            throw runtime_error(help);
        }

        inputFile.open(argv[1], std::ios::binary);
        if (!inputFile.is_open()) {
            // Не удалось открыть входной файл
            throw runtime_error(errOpenInputFile);
        }

		FrequencyTable ft = BuildFrequencyTable(inputFile);
        inputFile.close();
        cout << "Frequency table:" << endl;
		PrintFrequencyTable(ft);
		cout << endl;

		unique_ptr<Tree> huffmanTree(BuildHuffmatTree(ft));

        cout << "Huffman tree:" << endl;
		huffmanTree->PrintTree();
        cout << endl;

        cout << "Huffman table:" << endl;
		huffmanTree->PrintCodes();
        cout << endl;

    } catch(runtime_error &e) {
        cout << e.what() << endl;
        exitCode = EXIT_FAILURE;
    }

    if (inputFile.is_open()) {
        inputFile.close();
    }

    return exitCode;
}