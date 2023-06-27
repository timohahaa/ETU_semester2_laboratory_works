#ifndef SINGLYLINKEDLIST_H
#define SINGLYLINKEDLIST_H

#include <iostream>
using namespace std;

struct ListNode {
    int value;
    ListNode* next = nullptr;
};

ListNode* makeList(int length) {
    if (length < 1) {return nullptr;}
    ListNode* head = new ListNode;
    head->value = 1;
    ListNode* current = head;
    for (int i = 1; i < length; i++) {
        current->next = new ListNode;
        current = current->next;
        current->value = i + 1;
    }
    return head;
}

void printList(ListNode* list) {
    while (list != nullptr) {
        cout << list->value << "-->";
        list = list->next;
    }
    cout << endl;
}

int listLength(ListNode* list) {
    int length = 0;
    while (list != nullptr) {
        length++;
        list = list->next;
    }
    return length;
}

ListNode* addByIndex(ListNode* list, int index, int data) { //индексация с нуля
    if (index > listLength(list)) {return list;}
    if (index == 0) {
        ListNode* newHead = new ListNode;
        newHead->value = data;
        newHead->next = list;
        return newHead;
    }
    int i = 0;
    ListNode* current = list;
    ListNode* previous = list;
    while (i < index) {
        i++;
        previous = current;
        current = current->next;
    }
    ListNode* newNode = new ListNode;
    newNode->value = data;
    previous->next = newNode;
    newNode->next = current;
    return list;
}

ListNode* delByIndex(ListNode* list, int index) {
    if (index >= listLength(list)) {return list;}
    ListNode* current = list;
    if (index == 0) {
        current = list;
        list = list->next;
        delete current; //я не попался на вашу хитрую ловушку!!!
        return list;
    }
    int i = 0;
    ListNode* previous = list;
    while (i < index) {
        i++;
        previous = current;
        current = current->next;
    }
    previous->next = current->next;
    delete current; //ААААААА НЕГРЫ! ПОМОГИТЕ!
    return list;
}

void* delList(ListNode* list) {
    ListNode* current = list;
    while (list != nullptr) {
        list = list->next;
        delete current; //а все!!! нет больше счастья и радости!!! удалили!!!
        current = list;
    }
    return nullptr;
}

ListNode* findByIndex(ListNode* list, int index) {
    if (index >= listLength(list)) {return nullptr;}
    int i = 0;
    while (i < index) {
        i++;
        list = list->next;
    }
    return list;
}

ListNode* reverseList(ListNode* list) {
    if (list == nullptr) {return list;}
    ListNode* head = list;
    ListNode* head2 = list;
    ListNode* temp;
    while (head->next != nullptr) {
        temp = head->next;
        head->next = head->next->next;
        temp->next = head2;
        head2 = temp;
    }
    return head2;
}

ListNode* delFromTo(ListNode* list, int from_index, int to_index) { // [from_index; to_index] удаляется
    if (from_index < 0 || to_index < 0) {return list;}
    if (from_index > to_index) {return list;}
    int listlength = listLength(list);
    if (from_index >= listlength || to_index >= listlength) {return list;}
    ListNode* current = list;
    ListNode* previous;
    int i = 0;
    if (from_index == 0) {
        while (i <= to_index) {
            i++;
            previous = current;
            current = current->next;
            delete previous;
        }
        return current;
    }
    //if (from_index == to_index) {return delByIndex(list, from_index);}
    //тут предпочли скорость памяти, ибо delByIndex обязательно будет считать listLength
    while (i < from_index ) {
        i++;
        previous = current;
        current = current->next;
    }
    ListNode* temp;
    while (i <= to_index) {
        i++;
        temp = current;
        current = current->next;
        delete temp; //приходит как то улитка в бар...
    }
    previous->next = current;
    return list;
}

ListNode* insertFrom(ListNode* destination, ListNode* source, int index) {
    if (source == nullptr) {return destination;}
    if (destination == nullptr) {return source;}
    if (index < 0) {return destination;}
    int listlength = listLength(destination);
    if (index > listlength) {return destination;}
    ListNode* current = destination;
    ListNode* endOfSource;
    ListNode* insertAfterNode;
    if (index == listlength) {
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = source;
        return destination;
    }
    endOfSource = source;
    while (endOfSource->next != nullptr) { //нашли попу source
        endOfSource = endOfSource->next;
    }
    if (index == 0) {
        endOfSource->next = destination;
        return source;
    }
    int i = 0;
    current = destination;
    while (i < index) {
        i++;
        insertAfterNode = current;
        current = current->next;
    }
    insertAfterNode->next = source;
    endOfSource->next = current;
    return destination;
}

ListNode* bubbleSortList(ListNode* list) {
    if (list == nullptr) {return list;}
    if (list->next == nullptr) {return list;}
    ListNode* current;
    ListNode* next;
    int temp;
    int listlength = listLength(list);
    for (int i = 0; i < listlength - 1; i++) {
        current = list;
        for (int j = 0; j < listlength - 1 - i; j++) {
            next = current->next;
            if (current->value > next->value) {
                temp = current->value;
                current->value = next->value;
                next->value = temp;
            }
            current = current->next;
        }
    }
    return list;
}
#endif