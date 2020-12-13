#ifndef LABA2_TABLE_H
#define LABA2_TABLE_H

#include <Windows.h>

#include <string>
#include <vector>
#include <fstream>
#include <ctime>

using namespace std;

class Table {
public:
    Table(string pathToFile, int rows, int cols);

    void drawAndFillTable(HWND hWnd);

    vector<vector<string>> loadTextInMatrix();
    vector<vector<string>> getDataMatrix();

    void scrollTable(HWND hWnd, WPARAM wParam);

    void setWndWidth(int width);
    int getWndWidth();

    void setWndHeight(int height);
    int getWndHeight();

private:
    int wndWidth = 0;
    int wndHeight = 0;

    int maxColHeight;

    int rows;
    int cols;
    string pathToFile;

    vector<vector<int>> getMatrixWithLens(int size);
    vector<vector<string>> fillTextInMatrix(vector<char> data, vector<vector<int>> dataLenMatrix);
    vector<vector<string>> dataMatrix;
};


#endif //LABA2_TABLE_H
