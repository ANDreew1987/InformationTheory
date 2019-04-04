#ifndef TREE_HPP
#define TREE_HPP

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

#include "Node.hpp"

using std::cout;
using std::endl;
using std::setw;
using std::string;
using std::vector;
using std::pair;

using std::make_pair;

using HuffmanCodeTable = vector<pair<char,string>>;

class Tree {
private:
	Node *m_Root;

	void PrintNode(const Node *node, const int level) const {
		if (node) {
            PrintNode(node->Right(), level + 1);

            if (level > 0) {
                cout << setw(level) << " ";
            }

			cout << "[" << node->Frequency() << "]";
			if (node->Data() != 0) {
				cout << "[" << node->Data() << "]";
			}
			cout << endl;

            PrintNode(node->Left(), level + 1);
		}
	}

	void BuildCodeTable(const Node *node, HuffmanCodeTable &table, string code) const {
	    if (!node){
	        return;
	    }

	    if (node->Data()){
            table.push_back(make_pair(node->Data(), code));
	    }

        BuildCodeTable(node->Left(), table, code + "0");
        BuildCodeTable(node->Right(), table, code + "1");
	}

	void Delete(Node *node) {
		if (node) {
			Delete(node->Left());
			Delete(node->Right());
			delete node;
		}
	}
public:
	Tree(Node *root) :
		m_Root(root) {}

	~Tree() {
		Delete(m_Root);
	}

	void PrintTree() const {
		PrintNode(m_Root, 0);
	}

    HuffmanCodeTable GetCodeTable() const {
        HuffmanCodeTable result;
        BuildCodeTable(m_Root, result, "");
        return result;
	}
};

#endif //TREE_HPP