#ifndef READTABLE_HPP
#define READTABLE_HPP

#include <fstream>
#include <iostream>

#define ROWS 12
#define COLUMNS 18

class readTable
{
    private:
        readTable() {};
    public:
        static int stateTable[ROWS][COLUMNS];
        static void createMatrix(const char* filepath);
};
#endif
