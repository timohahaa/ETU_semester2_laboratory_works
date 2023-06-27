#include <iostream>
#include <windows.h>
#include <process.h>
#include <conio.h>
#include <vector>
#include <fstream>
#include <sstream>
#include "diff.h"


using namespace std;
short BUFF_WIDTH = 120, BUFF_HEIGHT = 9000; //we set BUFF_WIDTH to be even. Why? so the output looks nicer))
//look at printComparedFiles()

CONSOLE_SCREEN_BUFFER_INFOEX csbiOld;
CONSOLE_FONT_INFOEX cfiOld;
CONSOLE_CURSOR_INFO ciOld;

void setConsoleAppearence(short fontSize, const wchar_t* font, short bufferWidth, short bufferHeight) {
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

    COORD buffSize;
    buffSize.X = bufferWidth;
    buffSize.Y = bufferHeight;
    SetConsoleScreenBufferSize(hStdOut, buffSize);

    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
    GetCurrentConsoleFontEx(hStdOut, FALSE, &cfi);
    cfi.dwFontSize.X = 0;
    cfi.dwFontSize.Y = fontSize;
    wcscpy(cfi.FaceName, font);
    SetCurrentConsoleFontEx(hStdOut, FALSE, &cfi);

    CONSOLE_CURSOR_INFO ci;
    ci.bVisible = FALSE;
    ci.dwSize = 1;
    SetConsoleCursorInfo(hStdOut,&ci);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED| FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
}
void getConsoleAppearence() {
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

    csbiOld.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
    GetConsoleScreenBufferInfoEx(hStdOut, &csbiOld);

    cfiOld.cbSize = sizeof(CONSOLE_FONT_INFOEX);
    GetCurrentConsoleFontEx(hStdOut, FALSE, &cfiOld);

    GetConsoleCursorInfo(hStdOut, &ciOld);
}
void unsetConsoleAppearence() {
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleScreenBufferInfoEx(hStdOut, &csbiOld);
    SetCurrentConsoleFontEx(hStdOut, FALSE, &cfiOld);
    SetConsoleCursorInfo(hStdOut,&ciOld);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED| FOREGROUND_GREEN | FOREGROUND_BLUE);
}
void SetCursor(int x, int y) {
    COORD position;
    position.X = x;
    position.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
}

string readFile(const char* filename) {
    ifstream file;
    file.open(filename);
    if (!file) {
        cout << "Can not open such file: "<< filename;
        exit(EXIT_FAILURE);
    }
    stringstream buffer;
    buffer << file.rdbuf();
    if (!file.good()) {
        cout << "Error while reading file: "<< filename;
        exit(EXIT_FAILURE);
    }
    string str = buffer.str();
    return str;
}
int lineCount(string file) {
    int count = 0, i;
    for (i = 0; i < file.length(); i++) {
        if (file[i] == '\n') {count++;}
    }
    if (file[i-1] != '\n') {count++;}
    return count;
}
void printComparedFiles(string file1, string file2, string LCS) {
    //first file (or left file)
    int curY = 1;
    SetCursor(0, curY);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED| FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    for (int i = 0; i < BUFF_WIDTH; i++) {
        cout<<"=";
    }
    curY = 2;
    int LCSindex = 0;
    for (int i = 0; i < file1.length(); i++) {
        if (file1[i] == LCS[LCSindex]) {
            if (file1[i] == '\n') {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                cout << "\\n";
                LCSindex++;
            } else {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                                        FOREGROUND_RED| FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                cout << file1[i];
                LCSindex++;
            }
        } else {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                                    BACKGROUND_RED | FOREGROUND_RED| FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            if (i > 0) {
                if (file1[i-1] == '\n' && file1[i] == '\n') {
                    cout << "////////> empty line <////////";
                } else if (file1[i-1] != '\n' && file1[i] == '\n') {
                    cout << "\\n";
                } else {cout << file1[i];}
            } else {
                if (file1[i] == '\n') {
                    cout << "\\n";
                } else {cout << file1[i];}
            }
        }
        if (i == BUFF_WIDTH / 2 - 1 || file1[i] == '\n') {
            curY++;
            SetCursor(0, curY);
        }
    }
    //second file (or right file)
    curY = 2;
    LCSindex = 0;
    int curX = BUFF_WIDTH / 2 + 2;
    SetCursor(curX, curY);
    for (int i = 0; i < file2.length(); i++) {
        if (file2[i] == LCS[LCSindex]) {
            if (file2[i] == '\n') {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                cout << "\\n";
                LCSindex++;
            } else {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                                        FOREGROUND_RED| FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                cout << file2[i];
                LCSindex++;
            }
        } else {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                                    BACKGROUND_RED | FOREGROUND_RED| FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            if (i > 0) {
                if (file2[i-1] == '\n' && file2[i] == '\n') {
                    cout << "////////> empty line <////////";
                } else if (file2[i-1] != '\n' && file2[i] == '\n') {
                    cout << "\\n";
                } else {cout << file2[i];}
            } else {
                if (file2[i] == '\n') {
                    cout << "\\n";
                } else {cout << file2[i];}
            }
        }
        if (i == BUFF_WIDTH / 2 || file2[i] == '\n') {
            curY++;
            SetCursor(curX, curY);
        }
    }
    //line in the middle
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED| FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    curX = BUFF_WIDTH / 2;
    int lineCount1 = lineCount(file1);
    int lineCount2 = lineCount(file2);
    int maxHeight = (lineCount1 > lineCount2) ? lineCount1 : lineCount2;
    maxHeight += 5;
    for (int i = 1; i < maxHeight + 2; i++) {
        curY = i;
        SetCursor(curX, curY);
        cout << "||";
    }
    SetCursor(0, curY);
    for (int i = 0; i < BUFF_WIDTH; i++) {
        cout<<"=";
    }
    SetCursor(curX, curY);
    cout<<"||";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED| FOREGROUND_GREEN | FOREGROUND_BLUE);
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cout << "Not enough arguments!";
        exit(EXIT_FAILURE);
    }
    int len1 = strlen(argv[1]);
    int len2 = strlen(argv[2]);
    char* end1 = (argv[1] + len1 - 4);
    char* end2 = (argv[2] + len2 - 4);
    if (strcmp(end1, ".txt") != 0 || strcmp(end2, ".txt") != 0) {
        cout<< "This diff works only with \'.txt\' files";
        exit(EXIT_FAILURE);
    }
    string file1 = readFile(argv[1]);
    string file2 = readFile(argv[2]);
    if (file1.length() == 0 ) {
        cout<< "First file is empty!";
        exit(EXIT_FAILURE);
    }
    if (file2.length() == 0 ) {
        cout<< "Second file is empty!";
        exit(EXIT_FAILURE);
    }
    string LCS = myDiff(file1, file2);

    getConsoleAppearence();
    system("cls");
    setConsoleAppearence(20, L"Terminal", BUFF_WIDTH, BUFF_HEIGHT);

    char* filename1 = strrchr(argv[1], 92);
    char* filename2 = strrchr(argv[2], 92);
    cout<< ">>Comparing files: ";
    if (filename1 != nullptr) {
        cout<< ++filename1;
    } else {cout<< argv[1];}
    cout<< " and ";
    if (filename2 != nullptr) {
        cout<< ++filename2;
    } else {cout<< argv[2];}

    //cout<< ">>Comparing files: "<< argv[1]<< " and "<<argv[2];
    printComparedFiles(file1, file2, LCS);
    cout<< "\n>>Press ENTER to exit.";

    while (getch() != 13) {}
    system("cls");
    unsetConsoleAppearence();
    return 0;
}