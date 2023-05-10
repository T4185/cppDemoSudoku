// cppDemoSudoku.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <math.h>
#include <algorithm>
#include <random>
#include <array>
#include <chrono>
#include <stdlib.h>
#include <time.h>
#include <chrono>
#include <thread>
#include <Windows.h>


const int EmptyVal = 0;
const int SuSize = 9;
std::array<int, 9> values{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
int numberOfSolution = 1;


//position du curseur
void setCursorPosition(int x, int y)
{
    static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    std::cout.flush();
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hOut, coord);
}

//rendre la fonction sleep plus accessible
void sleep(int zzz)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(zzz));
}






void SuAfficher(int puzzle[SuSize][SuSize], bool clear = true)
{
    if (clear)
    {
        setCursorPosition(0, 0);
    }
    std::string text, separator, padding;

    for (int i = 0; i < SuSize; i++)
    {
        text = "|";
        separator = " -";
        padding = "|";
        for (int j = 0; j < SuSize; j++)
        {
            std::string value = puzzle[i][j] == EmptyVal ? " " : std::to_string(puzzle[i][j]);
            text += "  " + value + "  |";
            separator += "------";
            padding += "     |";
            if (j % 3 == 2 && j != SuSize - 1)
            {
                text += "|";
                padding += "|";
            }
        }
        if (i != 0 && i % 3 == 0)
        {
            std::replace(separator.begin(), separator.end(), '-', '=');
        }
        std::cout << separator << std::endl;
        std::cout << padding << std::endl;
        std::cout << text << std::endl;
        std::cout << padding << std::endl;
    }
    std::cout << separator << std::endl;
}


bool Valider(int puzzle[SuSize][SuSize], int row, int col, int value)
{
    for (int c = 0; c < SuSize; c++)
    {
        if (puzzle[row][c] == value)
            return false;
    }
    for (int r = 0; r < SuSize; r++)
    {
        if (puzzle[r][col] == value)
            return false;
    }
    int startRow = floor(row / 3) * 3, startCol = floor(col / 3) * 3;
    for (int r = startRow; r < startRow + 3; r++)
    {
        for (int c = startCol; c < startCol + 3; c++)
        {
            if (puzzle[r][c] == value)
                return false;
        }
    }
    return true;
}

//On vien "Mapper" les endroits a remplir
bool SansNombre(int puzzle[SuSize][SuSize])
{
    for (int r = 0; r < SuSize; r++)
    {
        for (int c = 0; c < SuSize; c++)
        {
            if (puzzle[r][c] == EmptyVal)
                return true;
        }
    }
    return false;
}
//Guarder le Sudoku unsolved
void CopierOriginal(int origin[SuSize][SuSize], int(&copy)[SuSize][SuSize])
{
    for (int r = 0; r < SuSize; r++)
    {
        for (int c = 0; c < SuSize; c++)
        {
            copy[r][c] = origin[r][c];
        }
    }
}


bool SuRemplir(int(&puzzle)[SuSize][SuSize])//Remplir 
{
    int row, col;
    for (int i = 0; i < SuSize * SuSize; i++)
    {
        row = floor(i / SuSize);
        col = i % SuSize;
        if (puzzle[row][col] == EmptyVal)
        {
            unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();//Random baser sur le compteur du systeme
            std::shuffle(values.begin(), values.end(), std::default_random_engine(seed));
            for (int j = 0; j < SuSize; j++)
            {
                if (Valider(puzzle, row, col, values[j]))
                {
                    puzzle[row][col] = values[j];
                    if (!SansNombre(puzzle) || SuRemplir(puzzle))
                    {
                        return true;
                    }
                }
            }
            break;
        }
    }
    puzzle[row][col] = EmptyVal;
    return false;
}


bool SuSolve(int puzzle[SuSize][SuSize], bool visualize = false)
{
    int row, col;
    for (int i = 0; i < SuSize * SuSize; i++)
    {
        row = floor(i / SuSize);
        col = i % SuSize;
        if (puzzle[row][col] == EmptyVal)
        {
            for (int value = 1; value <= SuSize; value++)
            {
                if (Valider(puzzle, row, col, value))
                {
                    puzzle[row][col] = value;
                    if (visualize)
                    {
                        //sleep(100);
                        SuAfficher(puzzle);
                    }
                    if (!SansNombre(puzzle))
                    {
                        numberOfSolution++;
                        if (visualize)
                        {
                            //sleep(100);
                            SuAfficher(puzzle);
                            return true;
                        }
                        break;
                    }
                    else if (SuSolve(puzzle, visualize))
                    {
                        return true;
                    }
                }
            }
            break;
        }
    }
    puzzle[row][col] = EmptyVal;
    if (visualize)
    {
        //sleep(100);
        SuAfficher(puzzle);
    }
    return false;
}


void SuGen(int(&puzzle)[SuSize][SuSize], int difficulty = 1)
{
    for (int i = 0; i < SuSize; i++)
    {
        for (int j = 0; j < SuSize; j++)
        {
            puzzle[i][j] = EmptyVal;
        }
    }
    SuRemplir(puzzle);
    srand((unsigned)time(0));
    int attempt = difficulty;
    while (attempt > 0)
    {
        int row = floor(rand() % SuSize);
        int col = floor(rand() % SuSize);
        while (puzzle[row][col] == EmptyVal)
        {
            row = floor(rand() % SuSize);
            col = floor(rand() % SuSize);
        }
        int backupValue = puzzle[row][col];
        puzzle[row][col] = EmptyVal;
        numberOfSolution = 0;
        SuSolve(puzzle);
        if (numberOfSolution != 1)
        {
            puzzle[row][col] = backupValue;
            attempt--;
        }
    }
}

int main(int, char**)
{
    int puzzle[SuSize][SuSize];
    int origin[SuSize][SuSize];
    SuGen(puzzle);
    CopierOriginal(puzzle, origin);
    system("cls");
    SuAfficher(puzzle);
    std::string run;
    std::cout << "Solve puzzle (Y/n) ";
    std::cin >> run;
    if (run == "n" || run == "N")
    {
        std::cout.flush();
        return 0;
    }
    SuSolve(puzzle, true);
    SuAfficher(origin, false);
    std::cout.flush();

    return 0;
}


