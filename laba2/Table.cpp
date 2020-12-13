#include "Table.h"

Table::Table(string pathToFile, int rows, int cols) {
    this->pathToFile = pathToFile;
    this->rows = rows;
    this->cols = cols;
}

void Table::drawAndFillTable(HWND hWnd) {
    RECT rect;
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);
    int colWidth = this->wndWidth / this->cols;
    int tableHeight = 0;
    int currMaxColHeight = 0;
    for (int i = 0; i < this->rows; i++)
    {
        currMaxColHeight = 0;
        for (int j = 0; j < this->cols; j++)
        {
            SetTextColor(hdc, RGB(0, 34, 245));

            LPCSTR dataItem = this->dataMatrix[i][j].c_str();
            int lenOfDataItem = strlen(dataItem);

            SetRect(&rect, j * colWidth + 1, tableHeight + 1, (j + 1) * colWidth - 1, this->wndHeight);
            int strHeight = DrawTextA(hdc, dataItem, lenOfDataItem, &rect, DT_WORDBREAK);

            if (strHeight > currMaxColHeight)
                currMaxColHeight = strHeight;

        }

        tableHeight += currMaxColHeight;

        MoveToEx(hdc, 0, tableHeight, NULL);
        LineTo(hdc, this->wndWidth, tableHeight);
    }
    this->maxColHeight = currMaxColHeight;

    for (int i = 1; i < this->cols; i++)
    {
        int colPosition = i * colWidth;
        MoveToEx(hdc, colPosition, 0, NULL);
        LineTo(hdc, colPosition, tableHeight);
    }
    EndPaint(hWnd, &ps);
}

vector<vector<string>> Table::fillTextInMatrix(vector<char> data, vector<vector<int>> dataLenMatrix) {
    vector<vector<string>> dataMatrixTmp(this->rows, vector<string>(this->cols));
    int currInd = 0;
    string tmp;
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->cols; j++) {
            dataMatrixTmp[i][j] = "";
            for (int m = currInd; m <= currInd + dataLenMatrix[i][j]; m++) {
                dataMatrixTmp[i][j] += data[m];
            }
            currInd += dataLenMatrix[i][j] + 1;
        }
    }
    this->dataMatrix = dataMatrixTmp;
    return this->dataMatrix;
}

vector<vector<int>> Table::getMatrixWithLens(int size) {
    vector<vector<int>> dataLenMatrix(this->rows, vector<int>(this->cols));
    srand(time(NULL));
    int available = size;
    int generatedLen;
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->cols; j++) {
            generatedLen = (rand() % available/5) + 0;
            dataLenMatrix[i][j] = generatedLen;
            available -= generatedLen;
        }
    }
    return dataLenMatrix;
}

vector<vector<string>> Table::loadTextInMatrix() {
    ifstream file(this->pathToFile, ios::binary | ios::ate);
    streamsize size = file.tellg();
    file.seekg(0, ios::beg);
    vector<char> buffer(size);

    if (file.read(buffer.data(), size))
    {
        vector<vector<int>> dataLenMatrix = this->getMatrixWithLens(size);
        this->dataMatrix = this->fillTextInMatrix(buffer, dataLenMatrix);
    }

    return this->dataMatrix;
}

vector<vector<string>> Table::getDataMatrix() {
    return this->dataMatrix;
}

void Table::setWndWidth(int width) {
    this->wndWidth = width;
}
int Table::getWndWidth() {
    return this->wndWidth;
}

void Table::setWndHeight(int height) {
    this->wndHeight = height;
}
int Table::getWndHeight() {
    return this->wndHeight;
}

void Table::scrollTable(HWND hWnd, WPARAM wParam) {
    POINT point;
    GetWindowOrgEx(GetDC(hWnd), &point);
    short zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
    int sign = (zDelta > 0) ? 1 : -1;
    int y = point.y - sign * 30;
    if (y >= 0 && this->wndHeight < this->maxColHeight - y + 20)
    {
        point.y = y;
        InvalidateRect(hWnd, NULL, TRUE);
    }
    SetWindowOrgEx(GetDC(hWnd), point.x, point.y, 0);
}