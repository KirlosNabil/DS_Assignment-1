#include "SuffixTree.h"

// Creates new node
Node* SuffixTree::createNewNode(int startIndexOfEdge, int startIndexOfSuffix)
{
    // Intializing new node
    Node* newNode = new Node;
    // Intializing the head of children of the node with nullptr
    newNode->headOfChildren = nullptr;
    // Intializing the next with nullptr
    newNode->next = nullptr;
    // Assigning node values
    newNode->startIndexOfEdge = startIndexOfEdge;
    newNode->startIndexOfSuffix = startIndexOfSuffix;
    return newNode;
}

// Searches for pattern in the tree
bool SuffixTree::search(char* pattern)
{

}

// Prints the nodes of the suffix tree
void SuffixTree::printSuffixTree(Node* parent)
{
    cout << parent -> startIndexOfEdge << " " << parent -> startIndexOfSuffix << endl;
    Node* traverse = parent -> headOfChildren;
    if(traverse != nullptr)
    {
        printSuffixTree(traverse);
    }
    else
    {
        return;
    }

    while(traverse -> next != nullptr)
    {
        traverse = traverse -> next;
        printSuffixTree(traverse);
    }
}

// Constructor of the suffix tree
SuffixTree::SuffixTree(char *str)
{
    // Intializing the tree with new root
    this->root = createNewNode(-1, -1);
    int n = strlen(str);

    // Getting all suffixes of string and adds it to the tree
    for(int i = 0; i < n; i++)
    {
        // Inserting this suffix in the tree
        insert(str, i, i, n);
    }
}

// Gets the edge length by finding least child index and subtracts there indices (if the node is leaf it gets its length by subtracting from original string length)
int findEdgeLength(int sizeOfOriginalStr, Node* edge)
{
    // Getting the start index of edge
    int startOfEdge = edge -> startIndexOfEdge;
    // Intializing the minimum index of children with the size of the original string
    // (Because if there is no children then the length of the edge will be directly length of string - edge starting index)
    int minimumIndex = sizeOfOriginalStr;
    Node* children = edge -> headOfChildren;
    // Getting minimum index of children
    while(children != nullptr)
    {
        minimumIndex = min(children->startIndexOfEdge, minimumIndex);
        children = children -> next;
    }
    return minimumIndex - startOfEdge;
}

// Checks if this child node matches with current suffix
bool checkIfChildIsValid(char* originalStr, Node* child, int indexOfSuffixSoFar)
{
    // If they have same first character then they match
    return originalStr[child -> startIndexOfEdge] == originalStr[indexOfSuffixSoFar];
}

// Adds child to node
void addChild(Node* child, Node* parent)
{
    Node* traverse = parent;
    // If the node has no children then assign head with this child
    if(traverse -> headOfChildren == nullptr)
    {
        traverse -> headOfChildren = child;
        return;
    }
    // Traversing till getting last child and assigning its next with this child
    traverse = traverse -> headOfChildren;
    if(traverse -> next == nullptr)
    {
        traverse -> next = child;
        return;
    }
    while(traverse -> next != nullptr)
    {
        traverse = traverse -> next;
    }
    traverse -> next = child;
    return;
}

// Inserts the suffixes to the tree
void SuffixTree::insert(char* originalStr, int startIndexOfSuffix, int startIndexOfLastSuffix, int sizeOfOriginalStr)
{
     // if the root has no children then we will intialize it with this suffix
     if(root -> headOfChildren == nullptr)
     {
         Node* newSuffix = createNewNode(startIndexOfSuffix, startIndexOfLastSuffix);
         root -> headOfChildren = newSuffix;
         return;
     }

     Node* temp = root -> headOfChildren;
     // If there is no matching character from nodes then it will stay false and add this suffix as new node to the root
     bool suffixExists = false;

     // Searching if there is matching suffix
     while(true)
     {
         // If the first character from this node matches the first character of the suffix
         if(originalStr[temp -> startIndexOfEdge] == originalStr[startIndexOfSuffix])
         {
             // We found a match so we assign this boolean to true
             suffixExists = true;
             // We will start finding all characters match of this suffix till it this variable becomes equal to original string length
             int startOfSuffix = startIndexOfSuffix;
             while(startOfSuffix != sizeOfOriginalStr)
             {
                 // First finding this edge length
                 int lengthOfEdge = findEdgeLength(sizeOfOriginalStr,temp);
                 int startOfEdge = 0;
                 // We will continue comparing till whether the edge fully match the suffix or there is a difference in some character
                 while(startOfSuffix < sizeOfOriginalStr && startOfEdge < lengthOfEdge)
                 {
                     if(originalStr[startOfEdge + temp -> startIndexOfEdge] == originalStr[startOfSuffix])
                     {
                         startOfEdge++;
                         startOfSuffix++;
                     }
                     else
                     {
                         break;
                     }
                 }
                 // Edge is fully matched -> go to one of children
                 if(startOfEdge == lengthOfEdge)
                 {
                     // Searching if there is matching child of this edge with the suffix
                     Node* getAppropriateChild = temp -> headOfChildren;
                     if(checkIfChildIsValid(originalStr,getAppropriateChild,startOfSuffix))
                     {
                         temp = temp -> headOfChildren;
                     }
                     else
                     {
                         bool found = false;
                         while(getAppropriateChild != nullptr)
                         {
                             // If this child matches with the suffix
                             if(checkIfChildIsValid(originalStr,getAppropriateChild,startOfSuffix))
                             {
                                 found = true;
                                 temp = getAppropriateChild;
                                 break;
                             }
                             getAppropriateChild = getAppropriateChild -> next;
                         }
                         // If there is no matching child then we will add the remaining part of the suffix as a new node to this edge
                         if(!found)
                         {
                             Node* newChildOfRemainingSuffix = createNewNode(startOfSuffix,startIndexOfSuffix);
                             addChild(newChildOfRemainingSuffix,temp);
                             break;
                         }
                     }
                 }
                 // Edge didn't fully match with suffix -> we will split the edge
                 else
                 {
                     // Split the edge to two new nodes first one the remaining suffix and second one the remaining of the original edge
                     Node* newSuffix = createNewNode(startOfSuffix, startIndexOfSuffix);
                     Node* originalSuffix = createNewNode(startOfEdge + temp -> startIndexOfEdge, temp -> startIndexOfSuffix);
                     // If the edge has no children
                     if(temp -> headOfChildren == nullptr)
                     {
                         temp -> headOfChildren = originalSuffix;
                         temp -> headOfChildren -> next = newSuffix;
                     }
                     else
                     {
                         // Search for last child and assign its next with the new children
                         Node* getLastChild = temp;
                         getLastChild = getLastChild -> headOfChildren;
                         while(getLastChild -> next != nullptr)
                         {
                             getLastChild = getLastChild -> next;
                         }
                         getLastChild -> next = originalSuffix;
                         getLastChild -> next -> next = newSuffix;
                     }
                     temp -> startIndexOfSuffix = -1;
                     break;
                 }
             }
             break;
         }
         else
         {
             // If we reached final child then we will break
             if(temp -> next == nullptr)
             {
                 break;
             }
             temp = temp -> next;
         }
     }
     // If there is no matching suffix then we will add it as a new node
     if(!suffixExists)
     {
         Node* newSuffix = createNewNode(startIndexOfSuffix, startIndexOfLastSuffix);
         temp -> next = newSuffix;
     }
     return;
}
