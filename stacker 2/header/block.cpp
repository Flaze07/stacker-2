#include "block.hpp"
#include <iostream>
#include <conio.h>
#include <windows.h>

void ClearScreen()
  {
  HANDLE                     hStdOut;
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  DWORD                      count;
  DWORD                      cellCount;
  COORD                      homeCoords = { 0, 0 };

  hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );
  if (hStdOut == INVALID_HANDLE_VALUE) return;

  /* Get the number of cells in the current buffer */
  if (!GetConsoleScreenBufferInfo( hStdOut, &csbi )) return;
  cellCount = csbi.dwSize.X *csbi.dwSize.Y;

  /* Fill the entire buffer with spaces */
  if (!FillConsoleOutputCharacter(
    hStdOut,
    (TCHAR) ' ',
    cellCount,
    homeCoords,
    &count
    )) return;

  /* Fill the entire buffer with the current colors and attributes */
  if (!FillConsoleOutputAttribute(
    hStdOut,
    csbi.wAttributes,
    cellCount,
    homeCoords,
    &count
    )) return;

  /* Move the cursor home */
  SetConsoleCursorPosition( hStdOut, homeCoords );
  }

Block::Block() :
    level(1),
    nBlocks(5),
    cooldown(100),
    direction(Direction::right),
    end(false)
{
    for (int i = 0; i < max_width; ++i)
    {
        for (int j = 0; j < max_height; ++j)
        {
            display[i + (max_width * j)].Char.AsciiChar = 'X';
            display[i + (max_width * j)].Attributes = FOREGROUND_INTENSITY;
        }
    }
    for (int i = 1; i < max_width - 1; ++i)
    {
        for (int j = 1; j < max_height - 1; ++j)
        {
            display[i + (max_width * j)].Char.AsciiChar = ' ';
            display[i + (max_width * j)].Attributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
        }
    }
    pos = (max_width / 2) - (nBlocks / 2);
    for (int i = 0; i < nBlocks; ++i)
    {
        display[(pos + i) + (max_width * ( (max_height - 1) - level))].Char.AsciiChar = '@';
    }
    timer = clock();
    holder = timer + cooldown;
}

void Block::run()
{
    timer = clock();
    if (kbhit())
    {
        char c = getch();
        if (c != 32) return;
        int temp = nBlocks;
        for (int i = 0; i < temp; ++i)
        {
            if (display[(pos + i) + (max_width * ( (max_height - 1) - (level - 1) ) )].Char.AsciiChar == ' ')
            {
                nBlocks--;
                display[(pos + i) + (max_width *( (max_height - 1) - level) )].Char.AsciiChar = ' ';
            }
        }
        ++level;
        if (nBlocks == 0)
        {
            condition = Condition::lose;
            end = true;
            return;
        }
        if (max_height - level == 1)
        {
            if (nBlocks == 0)
            {
                condition = Condition::lose;
                end = true;
                return;
            }
            else
            {
                condition == Condition::win;
                end = true;
                return;
            }
        }
        switch (level)
        {
        case 3 :
            if (nBlocks < 4) break;
            nBlocks = 4;
            break;
        case 6 :
            if (nBlocks < 3) break;
            nBlocks = 3;
            break;
        case 8 :
            if (nBlocks < 2) break;
            nBlocks = 2;
            break;
        case 10 :
            if (nBlocks < 1) break;
            nBlocks = 1;
            break;
        }
        pos = 1;
        for (int i = 0; i < nBlocks; ++i)
        {
            display[(pos + i) + (max_width * ( (max_height - 1) - level) )].Char.AsciiChar = '@';
        }
        return;
    }
    if (timer >= holder)
    {
        holder = timer + cooldown;
        if (direction == Direction::right)
        {
            if (display[(pos + nBlocks) + (max_width * ( (max_height - 1) - level) )].Char.AsciiChar == 'X')
            {
                direction = Direction::left;
                return;
            }
            display[(pos + nBlocks) + (max_width * ( (max_height - 1) - level ) )].Char.AsciiChar = '@';
            display[pos++ + (max_width * ( (max_height - 1) - level) )].Char.AsciiChar = ' ';
        }
        else if (direction == Direction::left)
        {
            if (display[(pos - 1) + (max_width * ( (max_height - 1) - level) )].Char.AsciiChar == 'X')
            {
                direction = Direction::right;
                return;
            }
            display[(pos - 1) + (max_width * ( (max_height - 1) - level) )].Char.AsciiChar = '@';
            display[(pos + (nBlocks - 1)) + (max_width * ( (max_height - 1) - level) )].Char.AsciiChar = ' ';
            --pos;
        }
    }
}

void Block::print()
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD BufferSize = {max_width, max_height};
    COORD StartFrom = {0, 0};
    SMALL_RECT WriteArea = {0, 0, max_width, max_height};
    WriteConsoleOutput(hOut, display, BufferSize, StartFrom, &WriteArea);
    COORD lol = {0, max_height + 1};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), lol);
    std::cout << pos;
}

void Block::check()
{
    if (end)
    {
        if (condition == Condition::win)
        {
            ClearScreen();
            std::cout << "you win...congrats\n";
            std::cin.ignore();
            exit(1);
        }
        else if (condition == Condition::lose)
        {
            ClearScreen();
            std::cout << "you lose...loser\n";
            std::cin.ignore();
            exit(1);
        }
    }
}
