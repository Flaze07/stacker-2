#include <iostream>
#include "header/block.hpp"

using namespace std;

int main()
{
    Block b;
    while (1)
    {
        b.run();
        b.print();
        b.check();
    }
    return 0;
}
