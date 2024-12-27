#include "HCTree.hpp"
//#include "Helper.hpp"
#include <queue>
#include <stack>
using namespace std;

void deleteTree(HCNode* ptr) {
    if (ptr == nullptr) return;
    
    deleteTree(ptr->c0);
    deleteTree(ptr->c1);

    delete ptr;
}

HCTree::~HCTree() {
    //clear all HCNodes given root
    deleteTree(root);
    root = nullptr;
    leaves.clear();
}

/**
* Use the Huffman algorithm to build a Huffman coding tree.
* PRECONDITION: freqs is a vector of ints, such that freqs[i] is the frequency of occurrence of byte i in the input file.
* POSTCONDITION: root points to the root of the trie, and leaves[i] points to the leaf node containing byte i.
*/
void HCTree::build(const vector<int>& freqs) {
    //for each byte i , freqs[i] add new HCNode(freqs[i], make i a char) to HCTree.leaves
    //then sort them accordingly starting with smallest two and making new parent node each time
    /*if (freqs.empty()) {
        return;
    }*/
    priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> pq;
    for (long unsigned int i = 0; i < freqs.size(); i++) {
        if (freqs[i] == 0) 
            continue; 
        else {
            HCNode* n = new HCNode(freqs[i], (unsigned char)i);
            leaves[i] = n;
            pq.push(n);
        }
    }
    HCNode* ptr; 
    HCNode* ptr2;

    while (!pq.empty()) {
        if (pq.size() == 1) {
            root = pq.top();
            pq.pop();
            break;
        }
        ptr = pq.top();
        pq.pop();
        ptr2 = pq.top();
        pq.pop();
        HCNode* n = new HCNode(ptr->count + ptr2->count, ptr->symbol);//ptr symbol
        n->c0 = ptr;//figure out which ptr is 0 or 1
        n->c1 = ptr2;
        ptr->p = n;
        ptr2->p = n;
        pq.push(n);
    }
}

/**
* Write to the given FancyOutputStream the sequence of bits coding the given symbol.
* PRECONDITION: build() has been called, to create the coding tree, and initialize root pointer and leaves vector.
*/
void HCTree::encode(unsigned char symbol, FancyOutputStream & out) const {
    stack<int> s;
    HCNode* ptr = leaves[(int)symbol]; 
    HCNode* par;
    while (ptr!=nullptr && ptr!=root) {
        par = ptr->p;
        if (ptr == par->c0) {
            s.push(0);
        }
        else {
            s.push(1);
        }
        ptr = par;
    }

    while (!s.empty()) {
        out.write_bit(s.top());
        s.pop();
    }
}

/**
* Return symbol coded in the next sequence of bits from the stream.
* PRECONDITION: build() has been called, to create the coding tree, and initialize root pointer and leaves vector.\
*/
unsigned char HCTree::decode(FancyInputStream & in) const {
    /*if (root == nullptr) {
        return (unsigned char)256;
    }*/
    HCNode* ptr = root;
    unsigned char c;
    int bit;
    while (ptr != nullptr) {
        if (ptr->c1 == nullptr && ptr->c0 == nullptr) {
            c = ptr->symbol;
            break;
        }
        bit = in.read_bit();
        if (bit == 1) {
            ptr = ptr->c1;
        }
        else if (bit == 0) {
            ptr = ptr->c0;
        }
        else if(bit == -1) {
            return (unsigned char)256;
        }
    }
    return c;
}
