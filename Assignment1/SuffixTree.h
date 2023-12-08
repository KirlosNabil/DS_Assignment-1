#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>

using namespace std;

struct Node
{
    Node* headOfChildren;     // Start of children
    Node* next;               // To iterate over them
    int startIndexOfEdge;     // Starting index of the characters on the edge going to this node
    int startIndexOfSuffix;   // Starting index of the suffix ending at this node (-1 for non-leaf nodes)
};


class SuffixTree
{
public:
    Node* root;
public :
    void printSuffixTree(Node* t);
    SuffixTree(char* str);
    Node* createNewNode(int startIndexOfEdge, int startIndexOfSuffix);
    void insert(char* originalStr, int startIndexOfSuffix, int startIndexOfLastSuffix, int sizeOfOriginalStr);
    bool search(char* pattern);
};

