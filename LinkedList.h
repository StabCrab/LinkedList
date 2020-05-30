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
        Node(const ValueType& value, Node* next = nullptr);
        ~Node();

        void insertNext(const ValueType& value);
        void removeNext();
        ValueType value;
        Node* next;
    };
    Node* _firstNode;
    size_t	_size;
    Node* _lastNode;

// класс узла списка
// по своей сути, может содержать любые данные,
// можно реализовать и ассоциативный массив, просто добавив
// поле с ключем в узел и, с учетом этого, поменять методы DoublyLinkedList
// (доступ по ключу, поиск по ключу и т.д.)
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
    long long int findIndex(ValueType& value) const;
    Node* findNode(ValueType& value) const;

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
    this->next = nullptr;
    this->value = 0;
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
LinkedList<ValueType>::LinkedList()
{
    _firstNode = nullptr;
    this->_size = 0;
}

template<class ValueType>
LinkedList<ValueType>::LinkedList(const LinkedList& copyList)
{
    this->_size = copyList._size;
    if (this->_size == 0) {
        this->_firstNode = nullptr;
        return;
    }
    this->_firstNode = new Node(copyList._firstNode->value);

    Node* currentNode = this->_firstNode;
    Node* currentCopyNode = copyList._firstNode;

    while (currentCopyNode->next) {
        currentNode->next = new Node(currentCopyNode->value);
        currentCopyNode = currentCopyNode->next;
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
    this->_firstNode = bufList._firstNode;

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
    delete[] this;
    this->_size = moveList._size;
    this->_firstNode = moveList._firstNode;

    moveList._size = 0;
    moveList._firstNode = nullptr;

    return *this;
}

template<class ValueType>
LinkedList<ValueType>:: ~LinkedList()
{
    Node* node = _firstNode;
    Node* nextNode = _firstNode->next;
    while(nextNode != nullptr)
    {
        delete node;
        node = nextNode;
        nextNode = node->next;
    }
    delete node;
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
        assert(pos < 0);
    }
    else if (pos >= this->_size) {
        assert(pos >= this->_size);
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
        assert(pos < 0);
    }
    else if (pos > this->_size) {
        assert(pos > this->_size);
    }

    if (pos == 0) {
        pushFront(value);
    }
    if (pos == _size - 1)
        pushBack(value);
    else{
        Node* bufNode = this->_firstNode;
        for (size_t i = 0; i < pos-1; ++i) {
            bufNode = bufNode->next;
        }
        bufNode->insertNext(value);
        ++_size;
    }
}

template<class ValueType>
void LinkedList<ValueType>::insertAfterNode(Node* node, const ValueType& value)
{
    node->insertNext(value);
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
        Node *node = new Node(value, nullptr);
        this->_lastNode->next = node;
        this->_lastNode = _lastNode->next;
    }
}

template<class ValueType>
void LinkedList<ValueType>::pushFront(const ValueType& value)
{
    _firstNode = new Node(value, _firstNode);
    if (_size == 0 )
        _lastNode = _firstNode;
    ++_size;
}

template<class ValueType>
void LinkedList<ValueType>:: remove(const size_t pos)
{
    if (pos < 0)
        assert(pos< 0);

    if (pos > this->_size)
        assert(pos > this->_size);
    if (pos == 0) {
        removeFront();
    }
    else {
        Node* node = this->_firstNode;
        for (size_t i = 0; i < pos - 1; ++i) {
            node = node->next;
        }
        node->removeNext();
        this->_size--;
    }
}
template <class ValueType>
void LinkedList<ValueType>:: removeNextNode(Node* node)
{
    Node *deletedNode = node->next;
    node->next = node->next->next;
    delete deletedNode;
}

template<class ValueType>
void LinkedList<ValueType>::removeFront()
{
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
    delete this->_lastNode;
    this->_lastNode = node;
    this->_lastNode->next = nullptr;
    this->_size--;
}

template<class ValueType>
long long int LinkedList<ValueType>::findIndex(ValueType& value) const
{
    long long int i = 0;
    Node *currentNode = _firstNode;
    while (i < _size)
    {
        if (currentNode->value == value)
            return i;
    }
    std:: cout << "No Node" << std :: endl;
    return -1;
}

template<class ValueType>
class LinkedList<ValueType>::Node* LinkedList<ValueType>::findNode(ValueType& value) const
{
    long long int i = 0;
    Node *currentNode = _firstNode;
    while (i < _size)
    {
        if (currentNode->value == value)
            return currentNode;
    }
    std:: cout << "No Node" << std :: endl;
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
    _lastNode = _firstNode;
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
    LinkedList reversedList = *this;
    reversedList.reverse();
    return reversedList;
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

