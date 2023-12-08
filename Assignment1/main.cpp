#include <iostream>
#include "SuffixTree.h"

int main()
{
    SuffixTree t("bananabanaba$");
    t.printSuffixTree(t.root);
    return 0;
}
