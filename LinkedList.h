//
// Created by trykr on 30.05.2020.
//
#pragma once
#include <cstdlib>
#include <iostream>
#include <cassert>
template <class ValueType>
class LinkedList
{
public:
    struct Node{
    public:
        Node();
        explicit Node(const ValueType& value, Node* next = nullptr);
        ~Node();

        void insertNext(const ValueType& value);
        void removeNext();

        ValueType value;
        Node* next;
        void forceNodeDelete(Node* node);
    };

private:
    Node* _firstNode;
    size_t	_size;

// класс узла списка
// по своей сути, может содержать любые данные,
// можно реализовать и ассоциативный массив, просто добавив
// поле с ключем в узел и, с учетом этого, поменять методы DoublyLinkedList
// (доступ по ключу, поиск по ключу и т.д.)
public:
    ////
    LinkedList();
    LinkedList(const LinkedList& copyList);
    LinkedList& operator=(const LinkedList& copyList);

    LinkedList(LinkedList&& moveList) noexcept;
    LinkedList& operator=(LinkedList&& moveList) noexcept;

    ~LinkedList();
    ////
    // доступ к значению элемента по индексу
    ValueType& operator[](const size_t pos) const;
    // доступ к узлу по индексу

    Node* getNode(const size_t pos) const;
    // вставка элемента по индексу, сначала ищем, куда вставлять (О(n)), потом вставляем (O(1))
    void insert(const size_t pos, const ValueType& value);
    // вставка элемента после узла, (O(1))
    void insertAfterNode(Node* node, const ValueType& value);
    // вставка в конец (О(n))
    void pushBack(const ValueType& value);
    // вставка в начало (О(1))
    void pushFront(const ValueType& value);

    // удаление
    void remove(const size_t pos);
    void removeNextNode(Node* node);
    void removeFront();
    void removeBack();

    // поиск, О(n)
    long long int findIndex(const ValueType& value) const;
    Node* findNode(const ValueType& value) const;

    // разворот списка
    void reverse();
    LinkedList reverse() const;// полчение нового списка (для константных объектов)
    LinkedList getReverseList() const; // чтобы неконстантный объект тоже мог возвращать новый развернутый список

    size_t size() const;

    void print(); // этот метод нужен исключительно для отладки

};

template<class ValueType>
LinkedList<ValueType>::Node::Node()
{
    this->value = ValueType();
    this->next = nullptr;
}

template<class ValueType>
LinkedList<ValueType>::Node::Node(const ValueType& value, Node* next)
{
    this->value = value;
    this->next = next;
}

template<class ValueType>
LinkedList<ValueType>::Node::~Node() {}

template<class ValueType>
void LinkedList<ValueType>::Node::insertNext(const ValueType& value)
{
    Node* newNode = new Node(value, this->next);
    this->next = newNode;
}

template<class ValueType>
void LinkedList<ValueType>::Node::removeNext()
{
    Node* removeNode = this->next;
    Node* newNext = removeNode->next;
    delete removeNode;
    this->next = newNext;
}

template<class ValueType>
void LinkedList<ValueType>::Node::forceNodeDelete(Node *node)
{
    if (node == nullptr) {
        return;
    }
    Node* nextDeleteNode = node->next;
    delete node;
    forceNodeDelete(nextDeleteNode);
}

template<class ValueType>
LinkedList<ValueType>::LinkedList()
{
    _firstNode = nullptr;
    this->_size = 0;
}

template<class ValueType>
LinkedList<ValueType>::LinkedList(const LinkedList& copyList)
{
    this->_size = copyList._size;
    this->_firstNode = new Node(copyList._firstNode->value);

    Node* currentNode = this->_firstNode;
    Node* currentCopyNode = copyList._firstNode;

    while (currentCopyNode->next) {
        currentCopyNode = currentCopyNode->next;
        currentNode->next = new Node (currentCopyNode->value);
        currentNode = currentNode->next;
    }
}

template<class ValueType>
LinkedList<ValueType>& LinkedList<ValueType>:: operator=(const LinkedList& copyList)
{
    if (this == &copyList) {
        return *this;
    }
    LinkedList bufList(copyList);
    this->_size = bufList._size;
    if (this->_firstNode != nullptr)
    {
        _firstNode->forceNodeDelete(_firstNode);
    }
    this->_firstNode = new Node(bufList._firstNode->value);
    Node* currentNode = this->_firstNode;
    Node* currentCopyNode = copyList._firstNode;

    while (currentCopyNode->next) {
        currentCopyNode = currentCopyNode->next;
        currentNode->next = new Node (currentCopyNode->value);
        currentNode = currentNode->next;
    }


    return *this;
}

template<class ValueType>
LinkedList<ValueType>::LinkedList(LinkedList&& moveList) noexcept
{
    this->_size = moveList._size;
    this->_firstNode = moveList._firstNode;

    moveList._size = 0;
    moveList._firstNode = nullptr;
}

template<class ValueType>
LinkedList<ValueType>& LinkedList<ValueType>::operator=(LinkedList&& moveList) noexcept
{
    if (this == &moveList) {
        return *this;
    }
    _firstNode->forceNodeDelete(_firstNode);
    this->_size = moveList._size;
    this->_firstNode = moveList._firstNode;

    moveList._size = 0;
    moveList._firstNode = nullptr;

    return *this;
}

template<class ValueType>
LinkedList<ValueType>:: ~LinkedList()
{
    _firstNode->forceNodeDelete(this->_firstNode);
//    if (_firstNode == nullptr)
//    {
//        return;
//    }
//    if (_firstNode->next == nullptr)
//    {
//        delete _firstNode;
//        return;
//    }
//    Node* node = _firstNode;
//    Node* nextNode = _firstNode->next;
//    while(nextNode != nullptr)
//    {
//        delete node;
//        node = nextNode;
//        nextNode = node->next;
//    }
//    delete node;
}

template<class ValueType>
ValueType& LinkedList<ValueType>:: operator[](const size_t pos) const
{
    return getNode(pos)->value;
}

template<class ValueType>
class LinkedList<ValueType>::Node* LinkedList<ValueType>::getNode(const size_t pos) const
{
    if (pos < 0) {
        throw std::runtime_error ("pos < 0");

    }
    else if (pos >= this->_size) {
        throw std::runtime_error (pos >= this->_size);
    }

    Node* bufNode = this->_firstNode;
    for (int i = 0; i < pos; ++i) {
        bufNode = bufNode->next;
    }

    return bufNode;
}

template<class ValueType>
void LinkedList<ValueType>::insert(const size_t pos, const ValueType& value)
{
    if (pos < 0) {
        throw std::runtime_error ("pos < 0");
    }
    else if (pos > this->_size) {
        throw std::runtime_error ("pos > this->_size");
    }

    if (pos == 0) {
        pushFront(value);
    }
    else{
        Node* bufNode = this->_firstNode;
        for (size_t i = 0; i < pos - 1; i++) {
            bufNode = bufNode->next;
        }
        bufNode->insertNext(value);
        _size++;
    }
}

template<class ValueType>
void LinkedList<ValueType>::insertAfterNode(Node* node, const ValueType& value)
{
    node->insertNext(value);
    this->_size++;
}

template<class ValueType>
void LinkedList<ValueType>::pushBack(const ValueType& value)
{
    if (_size == 0) {
        pushFront(value);
        return;
    }
    else
    {
        insert(_size, value);
    }
}

template<class ValueType>
void LinkedList<ValueType>::pushFront(const ValueType& value)
{
    _firstNode = new Node(value, _firstNode);
    ++_size;
}

template<class ValueType>
void LinkedList<ValueType>:: remove(const size_t pos)
{
    if (pos < 0)
        throw std::runtime_error("pos< 0");

    if (pos > this->_size)
        throw std::runtime_error ("pos > this->_size");
    if (pos == 0) {
        removeFront();
    }
    else {
        Node* node = getNode(pos - 1);
        removeNextNode(node);
    }
}
template <class ValueType>
void LinkedList<ValueType>:: removeNextNode(Node* node)
{
    Node *deletedNode = node->next;
    node->next = node->next->next;
    delete deletedNode;
    _size--;
}

template<class ValueType>
void LinkedList<ValueType>::removeFront()
{
    if (_size == 0)
    {
        throw std::runtime_error("list is empty");
    }
    Node *node = _firstNode;
    this->_firstNode = this->_firstNode->next;
    delete node;
    this->_size--;
}

template<class ValueType>
void LinkedList<ValueType>:: removeBack()
{
    Node* node = _firstNode;
    for (int i = 0; i < this->_size - 2; i++)
    {
        node = node->next;
    }
    delete node->next;
    node->next = nullptr;
    _size--;
}

template<class ValueType>
long long int LinkedList<ValueType>::findIndex(const ValueType& value) const
{
    long long int i = 0;
    Node *currentNode = _firstNode;
    while (i < _size)
    {
        if (currentNode->value == value)
            return i;
        i++;
        currentNode = currentNode->next;
    }
    return -1;
}

template<class ValueType>
class LinkedList<ValueType>::Node* LinkedList<ValueType>::findNode(const ValueType& value) const
{
    size_t i = 0;
    Node *currentNode = _firstNode;
    while (i < _size)
    {
        if (currentNode->value == value)
            return currentNode;
        i++;
        currentNode = currentNode->next;
    }
    return nullptr;
}
template<class ValueType>
void LinkedList<ValueType>:: reverse()
{
    Node *previousNode= _firstNode;
    Node *currentNode = _firstNode->next;
    Node *nextNode = currentNode->next;
    _firstNode->next = nullptr;
    currentNode->next = previousNode;
    while (nextNode->next != nullptr)
    {
        previousNode = currentNode;
        currentNode = nextNode;
        nextNode = nextNode->next;
        currentNode->next = previousNode;
    }
    previousNode = currentNode;
    currentNode = nextNode;
    currentNode->next = previousNode;
    _firstNode = currentNode;
}
template<class ValueType>
LinkedList<ValueType> LinkedList<ValueType>:: reverse() const
{
    LinkedList reversedList = *this;
    reversedList.reverse();
    return reversedList;
}


template<class ValueType>
LinkedList<ValueType> LinkedList<ValueType>:: getReverseList() const
{
    return this->reverse();
}

template<class ValueType>
size_t LinkedList<ValueType> :: size() const
{
    return _size;
}

template<class ValueType>
void LinkedList<ValueType>::print()
{
    Node* node = this->_firstNode;
    while(node->next != nullptr)
    {
        std::cout << node->value<< std::endl;
        node = node->next;
    }
    std::cout << node->value<< std::endl;
}

