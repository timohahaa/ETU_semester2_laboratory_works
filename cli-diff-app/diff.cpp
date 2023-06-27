#include <iostream>
#include <conio.h>
#include <vector>
#include <algorithm>
#include "diff.h"

using namespace std;
//http://www.xmailserver.org/diff2.pdf
/*
struct MyersNode {
    int k;
    int d;
    int x;
    int y;
    MyersNode* right = nullptr;
    MyersNode* down = nullptr;
    MyersNode* prev = nullptr;
};

int MyersDiff(string str1, string str2, MyersNode* head) {
    int N = str1.length(), M = str2.length();
    int max = N + M;
    int arr[2 * max + 1];
    head->k = 0;
    head->d = 0;
    head->x = 0;
    head->y = 0;
    MyersNode* temp = head;
    for (int i = 0; i < 2 * max + 1; i++) {
        arr[i] = 0;
    }
    int* v = &arr[max]; //negative indexes!!!
    v[1] = 0;
    int x = 0, y = 0;
    int direction;
    for (int d = 0; d < max; d++) {
        for (int k = -d; k < d; k += 2) {
            if (k == -d || k != d && v[k-1] < v[k+1]) {
                x = v[k+1];
                direction = 1;
            } else {
                x = v[k-1] + 1;
                direction = 0;
            }
            y = x - k;
            while (x < N && y < M && str1[x+1] == str2[y+1]) {
                x++;
                y++;
            }
            v[k] = x;
            if (direction) { //down
                temp->down = new MyersNode;
                temp->down->prev = temp;
                temp = temp->down;
                temp->x = x;
                temp->k = k;
                temp->y =x - k;
                temp->d = d;
            } else {
                temp->right = new MyersNode;
                temp->right->prev = temp;
                temp = temp->right;
                temp->x = x;
                temp->k = k;
                temp->y = x - k;
                temp->d = d;
            }
            if (x >= N && y >= M) {
                return d;
            }
        }
    }
    return 2281337;
}

void printGraph(MyersNode* head) {
    MyersNode* temp = head;
    if (temp != nullptr) {
        cout<<"x: "<<temp->x<<" y: "<<temp->y<<" k: "<<temp->k<<" d: "<<temp->d<<endl;
    }
    if (temp->right != nullptr) {
        cout<<"rigth: ";
        printGraph(temp->right);
    }
    if (temp->down != nullptr) {
        cout<<"down: ";
        printGraph(temp->down);
    }
}

MyersNode* findLast(MyersNode* head) {
    MyersNode *temp = head, *temp1 = head, *temp2 = head;
    if (temp != nullptr) {
        if (temp->x == 7 && temp->y == 6) {
            return temp;
        }
    }
    temp1 = temp->right;
    temp2 = temp->down;
    if (temp1 != nullptr) {
        temp1 = findLast(temp1);
    }
    if (temp2 != nullptr) {
        temp2 = findLast(temp2);
    }
    if (temp1 != nullptr) {
        if (temp1->x == 7 && temp1->y == 6) {
            return temp1;
        }
    }
    if (temp2 != nullptr) {
        if (temp2->x == 7 && temp2->y == 6) {
            return temp2;
        }
    }
    return nullptr;
}

void findDeletionsAndInsertions(vector<int>& deletions, vector<int>& insertions, MyersNode* lastNode) {
    MyersNode* temp = lastNode;
    //horizontal - deletions;
    //vertical - insertions
    int x, y, prev_x, prev_y;
    while (temp->d != 0) {
        x = temp->x;
        y = temp->y;
        temp = temp->prev;
        prev_x = temp->x;
        prev_y = temp->y;
        while (x > prev_x) {
            deletions.push_back(x);
            x--;
        }
        while (y > prev_y) {
            insertions.push_back(y);
            y--;
        }
    }
}
*/
string myDiff(string str1, string str2) {
    int s1Len = str1.length();
    int s2Len = str2.length();
    if (s1Len == 0) {return "";}
    if (s2Len == 0) {return "";}
    string LCS = "";
    short LCSmatrix[s1Len + 1][s2Len + 1];
    int i, j;
    for (i = 0; i < s1Len + 1; i++) {
        for (j = 0; j < s2Len + 1; j++) {
            LCSmatrix[i][j] = 0;
        }
    }
    //fill the LCS matrix
    for (i = 1; i < s1Len + 1; i++) {
        for (j = 1; j < s2Len + 1; j++) {
            if (str1[i - 1] == str2[j - 1]) {
                LCSmatrix[i][j] = LCSmatrix[i-1][j-1] + 1;
            } else { //str1[i - 1] != str2[j - 1]
                LCSmatrix[i][j] = (LCSmatrix[i-1][j] > LCSmatrix[i][j-1]) ? LCSmatrix[i-1][j] : LCSmatrix[i][j-1];
            }
        }
    }
    //backtrack the matrix and retrieve LCS
    i = s1Len;
    j = s2Len;
    while ( i > 0 && j > 0) {
        if (str1[i-1] == str2[j-1]) {
            LCS += str1[i-1];
            i--;
            j--;
        } else { //str1[i - 1] != str2[j - 1]
            if (LCSmatrix[i-1][j] > LCSmatrix[i][j-1]) {i--;} else {j--;}
        }
    }
    //print the LCS matrix
    /*
    for (i = 0; i < s1Len + 1; i++) {
        for (j = 0; j < s2Len + 1; j++) {
            cout<< LCSmatrix[i][j] << " ";
            if (j == s2Len) {cout << endl;}
        }
    }
     */
    reverse(LCS.begin(), LCS.end());
    return LCS;
}
/*
int main() {
    string str1 = "ABCABBA";
    string str2 = "CBABAC";
    /*
    MyersNode* head = new MyersNode;
    int ans = MyersDiff(str1, str2, head);
    MyersNode* last = findLast(head);
    vector<int> deletions, insertions;
    findDeletionsAndInsertions(deletions, insertions, last);
    cout<< "deletions: ";
    for (int i = 0; i < deletions.size(); i++) {
        cout<<deletions[i]<< " ";
    }
    cout<< "insertions: ";
    for (int i = 0; i < insertions.size(); i++) {
        cout<<insertions[i]<< " ";
    }

    //printGraph(last);
    //cout<< ans << endl;
    //printGraph(head);
     *
    while(getch() != 13){};
    return 0;
}
*/