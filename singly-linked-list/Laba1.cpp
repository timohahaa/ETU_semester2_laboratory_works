#include <iostream>
#include <string>
#include "singlyLinkedList.h"
#include <windows.h>
#include <conio.h>


#define BUFF_WIDTH 80
#define BUFF_HEIGHT 30
#define BUFF_LENGTH 2400
#define INPUT_WIDTH 20
#define INPUT_HEIGHT 7


#define UP_ARROW 72
#define DOWN_ARROW 80
#define ENTER 13
#define BACKSPACE 8
#define TAB 9

using namespace std;
//все глобальные переменные
int num = 1;
int list_count = 0;
//описание кнопок, для того, чтобы быстрее "собирать" лабы в дальнейшем
struct BUTTON {
    int index;
    wchar_t text[50];
};
ListNode* list = nullptr;
//BUTTON* makeListButton = {1, L"[Создать список]"}; //почему ошибка?
BUTTON makeListButton = {1, L"[Создать список]"};
BUTTON addByIndexButton = {2, L"[Добавить элемент по индексу]"};
BUTTON delByIndexButton = {3, L"[Удалить элемент по индексу]"};
BUTTON delListButton = {4, L"[Удалить список]"};
BUTTON insertFromButton = {5, L"[Вставить часть списка]"};
BUTTON delFromToButton = {6, L"[Удалить часть списка]"};
BUTTON reverseListButton = {7, L"[Перевернуть список за O(n)]"};
BUTTON bubbleSortButton = {8, L"[Сортировать список (пузырьком)]"};
BUTTON exitButton = {9, L"[Выход]"};

//все input параметры - тут некоторые можно было продублировать и сэкономить память, но я не стал этого делать чисто из-за красоты
int list_length = 0;
int insert_list_length = 0;
int index = 0;
int insIndex = 0;
int insValue = 0;
int delIndex = 0;
int from_index = 0;
int to_index;

//перемещение каретки по консоли
COORD position;
#define SET_CURSOR SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position)

void setConsoleAppearanse(short fontSize) {
    //SetConsoleOutputCP(1251); так быстрее и не нужен wchar, но скучнее!!!
    setlocale(LC_ALL, "Russian");
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
    GetCurrentConsoleFontEx(hStdOut, FALSE, &cfi);
    cfi.dwFontSize.X = 0;
    cfi.dwFontSize.Y = fontSize;
    wcscpy(cfi.FaceName, L"Terminal"); //некоторые шрифты "искожают" отношение columns/rows
    SetCurrentConsoleFontEx(hStdOut, FALSE, &cfi);

    HWND console = GetConsoleWindow();

    COORD bufferSize = {80, 30};
    SetConsoleScreenBufferSize(hStdOut, bufferSize);
    system("mode 80,30"); //потому что символ в высоту в два раза больше, чем в длинну, а разрешение в итоге должно быть 4:3 ПОТОМУ ЧТО ВОСЕМЬ
    ShowScrollBar(console, SB_BOTH, FALSE);
    //ShowWindow(console, SW_MAXIMIZE);

    CONSOLE_CURSOR_INFO ci;
    ci.bVisible = FALSE;
    ci.dwSize = 1;
    SetConsoleCursorInfo(hStdOut,&ci);
}

void ConsoleColorBlue() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_INTENSITY);
}
void ConsoleColorWhite() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
}
void ConsoleColorDefault() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}
void ClearRow(int row) {
    position.Y = row;
    position.X = 0;
    SET_CURSOR;
    for (int i = 0; i < BUFF_WIDTH; i++) {
        wcout << L" ";
    }
}
void PrintTextAllignCenter(int row, const wchar_t text[]) {
    position.Y = row;
    position.X = (BUFF_WIDTH - wcslen(text)) / 2;
    SET_CURSOR;
    wcout << text;
}
void PrintValueAllignCenter(int row, const wchar_t text[], int *value) {
    position.Y = row;
    position.X = (BUFF_WIDTH - wcslen(text)) / 2 - 1;
    SET_CURSOR;
    wcout << text;
    wcout << L"     ";
    position.X += wcslen(text);
    SET_CURSOR;
    wcout << *value;
}
void PrintButtonAllignCenter(int row, BUTTON* button) {
    position.Y = row;
    position.X = (BUFF_WIDTH - wcslen(button->text)) / 2 - 5;
    if (button->index == num) {
        ConsoleColorBlue();
        SET_CURSOR;
        wcout << L">>>  " << button->text << L"  <<<";
        ConsoleColorDefault();
    } else {
        SET_CURSOR;
        wcout << L"     " << button->text << L"     ";
    }
}

void menu() {
    ConsoleColorDefault();
    PrintTextAllignCenter(0, L"Выберите команду и нажмите ENTER");
    cout <<"\n================================================================================";
    PrintButtonAllignCenter(2, &makeListButton);
    PrintValueAllignCenter(3, L"размер: ", &list_length);
    PrintButtonAllignCenter(4, &addByIndexButton);
    PrintValueAllignCenter(5, L"индекс: ", &insIndex);
    PrintValueAllignCenter(6, L"значение: ", &insValue);
    PrintButtonAllignCenter(7, &delByIndexButton);
    PrintValueAllignCenter(8, L"индекс: ", &delIndex);
    PrintButtonAllignCenter(9, &delListButton);
    PrintButtonAllignCenter(10, &insertFromButton);
    PrintValueAllignCenter(11, L"индекс: ", &index);
    PrintValueAllignCenter(12, L"размер: ", &insert_list_length);
    PrintButtonAllignCenter(13, &delFromToButton);
    PrintValueAllignCenter(14, L"индекс С: ", &from_index);
    PrintValueAllignCenter(15, L"индекс ПО: ", &to_index);
    PrintButtonAllignCenter(16, &reverseListButton);
    PrintButtonAllignCenter(17, &bubbleSortButton);
    PrintButtonAllignCenter(18, &exitButton);
    cout <<"\n================================================================================";
    PrintTextAllignCenter(20, L"РЕЗУЛЬТАТ:");
    ClearRow(21);
    ClearRow(22);
    position.X = 0;
    position.Y = 21;
    SET_CURSOR;
    printList(list);
}

void InitWindow(int topLeftX, int topLeftY) {
    position.Y = topLeftY;
    position.X = topLeftX;
    SET_CURSOR;
    ConsoleColorBlue();
    wcout << L"*";
    for (int i = 0; i < INPUT_WIDTH - 2; i++) {wcout << L"-";}
    wcout << L"*";
    for (int i = 0; i < INPUT_HEIGHT - 2; i++) {
        position.Y ++;
        SET_CURSOR;
        wcout << L"|";
        for (int j = 0; j < INPUT_WIDTH - 2; j++) {wcout << L" ";}
        wcout << L"|";
    }
    position.Y ++;
    SET_CURSOR;
    wcout << L"*";
    for (int i = 0; i < INPUT_WIDTH - 2; i++) {wcout << L"-";}
    wcout << L"*";
    ConsoleColorDefault();
}
void UpdateWindow(int topLeftX, int topLeftY, int* value) {
    position.Y = topLeftY + 3;
    position.X = topLeftX + INPUT_WIDTH / 2 - 1;
    SET_CURSOR;
    wcout << L"  ";
    SET_CURSOR;
    wcout << *value;
    position.Y = topLeftY + 5;
    position.X = topLeftX + 1 ;
    SET_CURSOR;
    if (*value > 20) {wcout << L"Неверное значение!";
    } else {wcout << L"                  ";}
}
void DeleteWindow(int topLeftX, int topLeftY) {
    position.Y = topLeftY;
    position.X = topLeftX;
    SET_CURSOR;
    for (int i = 0; i < INPUT_HEIGHT; i++) {
        for (int j = 0; j < INPUT_WIDTH; j++) {wcout << L" ";}
        position.Y ++;
        SET_CURSOR;
    }
}

void ValueInput(int fromRow, int toRow, int* value, const wchar_t text[]) {
    int topY = (toRow - fromRow - 7) / 2 + fromRow;
    int topX = (BUFF_WIDTH - INPUT_WIDTH) / 2;
    InitWindow(topX, topY);
    position.Y = topY + 1;
    position.X = (BUFF_WIDTH - wcslen(text)) / 2;
    SET_CURSOR;
    wcout << text;
    UpdateWindow(topX, topY, value);
    int key = 0;
    while (key != -1) {
        key = getch();
        if (key >= 48 && key <= 57) {
            *value = (*value) * 10 + key - 48;
            if (*value >= 100) {*value /= 10;}
        }
        if (key == BACKSPACE) {*value /= 10;}
        UpdateWindow(topX, topY, value);
        if (key == ENTER) {
            if (*value <= 20) {
                DeleteWindow(topX, topY);
                menu();
                key = -1;
            }
        }
    }
}

void ButtonFunctionHandler() {
    //выше описать все функции, срабатываемые при нажатии кнопок - слегка костыльно, но да ладно)))
    switch (num) {
        case 1:
            if (!list_count) {
                ValueInput(1, 19, &list_length, L"Введите размер");
                list = makeList(list_length);
                menu();
                list_count = 1;
            }
            if (list == nullptr) {
                ValueInput(1, 19, &list_length, L"Введите размер");
                list = makeList(list_length);
                menu();
                list_count = 1;
            }
            break;
        case 2:
            ValueInput(1, 19, &insIndex, L"Введите индекс");
            ValueInput(1, 19, &insValue, L"Введите значение");
            list = addByIndex(list, insIndex, insValue);
            menu();
            break;
        case 3:
            ValueInput(1, 19, &delIndex, L"Введите индекс");
            list = delByIndex(list, delIndex);
            menu();
            break;
        case 4:
            list_count = 0;
            list = (ListNode*)delList(list);
            menu();
            break;
        case 5: {
            ValueInput(1, 19, &index, L"Введите индекс");
            ValueInput(1, 19, &insert_list_length, L"Введите размер");
            ListNode *list_to_insert = makeList(insert_list_length);
            list = insertFrom(list, list_to_insert, index);
            menu();
            //delList(list_to_insert);
            break;
        }
        case 6:
            ValueInput(1, 19, &from_index, L"Введите индекс С");
            ValueInput(1, 19, &to_index, L"Введите индекс ПО");
            list = delFromTo(list, from_index, to_index);
            menu();
            break;
        case 7:
            list = reverseList(list);
            menu();
            break;
        case 8:
            list = bubbleSortList(list);
            menu();
            break;
        case 9:
            num = 69;
            break;
        default:
            break;
    }
}

int main() {
    setConsoleAppearanse(40);
    menu();
    while (num != 69) {
        switch (getch()) {
            case UP_ARROW:
                num -= 1;
                if (num == 0) {num = 9;}
                menu();
                break;
            case DOWN_ARROW:
                num += 1;
                if (num == 10) {num = 1;}
                menu();
                break;
            case ENTER:
                ButtonFunctionHandler();
                break;
            default:
                break;
        }
    }
    return 0;
}
