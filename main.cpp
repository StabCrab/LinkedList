#include "LinkedList.h"
int main()
{
    auto *list = new LinkedList<int>;
    list->pushBack(4);
    list->pushBack(32);
    list->pushFront(1);
    list->pushBack(2);
    list->removeBack();
//   list->print();
    list->removeFront();
    list->pushBack(1);
    list->pushBack(2);
    list->pushBack(3);
    list->pushBack(4);
    list->print();
    list->reverse();
    list->print();
//    list->print();
}
