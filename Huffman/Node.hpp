#ifndef NODE_HPP
#define NODE_HPP

class Node {
private:
	Node *m_Left;
	Node *m_Right;
	char m_Data;
	int m_Freq;
public:
	Node(const char data, const int freq):
		m_Left(nullptr),
		m_Right(nullptr),
		m_Data(data),
		m_Freq(freq) {}

	Node(Node *left, Node *right) :
		m_Left(left),
		m_Right(right),
		m_Data(0)
	{
		m_Freq = left->m_Freq + right->m_Freq;
	}

	Node* Left() const {
		return m_Left;
	}

	Node* Right() const {
		return m_Right;
	}

	char Data() const {
		return m_Data;
	}

	int Frequency() const {
		return m_Freq;
	}
};

struct NodeComparator{
    bool operator() (const Node *n1, const Node *n2) {
        return n1->Frequency() > n2->Frequency();
    }
};

#endif //NODE_HPP