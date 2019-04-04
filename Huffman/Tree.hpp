#ifndef TREE_HPP
#define TREE_HPP

#include <iostream>
#include <iomanip>
#include <string>

#include "Node.hpp"

using std::cout;
using std::endl;
using std::setw;
using std::string;

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

	void PrintCode(const Node *node, string str) const {
	    if (!node){
	        return;
	    }

	    if (node->Data()){
	        cout << node->Data() << ": " << str << endl;
	    }

        PrintCode(node->Left(), str + "0");
        PrintCode(node->Right(), str + "1");
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

	void PrintCodes() const {
        PrintCode(m_Root, "");
	}
};

#endif //TREE_HPP