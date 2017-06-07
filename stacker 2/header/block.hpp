#ifndef BLOCK_HPP_INCLUDED
#define BLOCK_HPP_INCLUDED

#include <ctime>
#include <windows.h>

class Block
{
private :
    enum class Direction {right, left};
    enum class Condition {win, lose};
    Direction direction;
    Condition condition;
    bool end;
    int level;
    int nBlocks;
    int pos;
    static const int max_width = 9;
    static const int max_height = 12;
    CHAR_INFO display[max_width * max_height];
    time_t timer;
    time_t holder;
    int cooldown;
public :
    Block();
    void run();
    void print();
    void check();
};

#endif // BLOCK_HPP_INCLUDED
