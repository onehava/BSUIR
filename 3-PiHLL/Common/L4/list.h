#pragma once

#include <cstddef>
#include <stdexcept>

template <typename T>
class List
{
public:
    class iterator;
    class const_iterator;

    // constructors
    List();
    List(std::size_t count, const T& element);
    List(const std::initializer_list<T>& elements);

    List(const List<T>& other);
    List& operator=(const List<T>& other);

    List(List<T>&& other) noexcept;
    List& operator=(List<T>&& other) noexcept;

    // destructor
    ~List();

    // iterators
    iterator begin();
    iterator end();

    // const iterators
    const_iterator begin() const;
    const_iterator end() const;

    // access
    T& front();
    T& back();

    // const access
    const T& front() const;
    const T& back() const;

    // front methods
    void push_front(const T& element);
    void pop_front();

    // back methods
    void push_back(const T& element);
    void pop_back();

    // access methods
    T& at(std::size_t index);
    T& operator[](std::size_t index);

    // const access methods
    const T& at(std::size_t index) const;
    const T& operator[](std::size_t index) const;

    // middle methods
    void insert(std::size_t index, const T& element);
    void erase(std::size_t index);

    // size methods
    std::size_t size() const;
    bool empty() const;

private:
    struct node;

    node *m_begin, *m_end;
    std::size_t m_size;
};

// node implementation

template <typename T>
struct List<T>::node
{
    node();
    node(const T& item);

    node *prev, *next;
    T element;
};

template <typename T>
List<T>::node::node(const T& item){
    element = item;
}

template <typename T>
List<T>::node::node() : node(nullptr){}

// iterator implementation

template <typename T>
class List<T>::iterator
{
public:
    iterator(node* pointer);
    iterator();
    T& operator*() const;

    iterator& operator++();
    iterator operator++(int);

    iterator& operator--();
    iterator operator--(int);

    bool operator==(const iterator& other) const;
    bool operator!=(const iterator& other) const;

private:
    node* m_pointer;
};

template<typename T>
List<T>::iterator::iterator() : m_pointer(nullptr){}

template<typename T>
List<T>::iterator::iterator(node *pointer) : m_pointer(pointer){}

template<typename T>
T &List<T>::iterator::operator*() const
{
    return m_pointer->element;
}

template<typename T>
typename List<T>::iterator &List<T>::iterator::operator++()
{
    m_pointer = m_pointer->next;
    return *this;
}

template<typename T>
typename List<T>::iterator List<T>::iterator::operator++(int)
{
    iterator tmp = *this;
    m_pointer = m_pointer->next;
    return tmp;
}

template<typename T>
typename List<T>::iterator &List<T>::iterator::operator--()
{
    m_pointer = m_pointer->prev;
    return *this;
}

template<typename T>
typename List<T>::iterator List<T>::iterator::operator--(int)
{
    iterator tmp = *this;
    m_pointer = m_pointer->prev;
    return tmp;
}

template<typename T>
bool List<T>::iterator::operator==(const List<T>::iterator &other) const
{
    return m_pointer == other.m_pointer;
}

template<typename T>
bool List<T>::iterator::operator!=(const List<T>::iterator &other) const
{
    return m_pointer != other.m_pointer;
}

// const iterator implementation

template <typename T>
class List<T>::const_iterator
{
public:
    const_iterator(node* pointer);
    const_iterator();
    const T& operator*() const;

    const_iterator& operator++();
    const_iterator operator++(int);

    const_iterator& operator--();
    const_iterator operator--(int);

    bool operator==(const const_iterator& other) const;
    bool operator!=(const const_iterator& other) const;

private:
    node* m_pointer;
};

template<typename T>
List<T>::const_iterator::const_iterator() : m_pointer(nullptr){}

template<typename T>
List<T>::const_iterator::const_iterator(node *pointer) : m_pointer(pointer){}

template<typename T>
const T &List<T>::const_iterator::operator*() const
{
    return m_pointer->element;
}

template<typename T>
typename List<T>::const_iterator &List<T>::const_iterator::operator++()
{
    m_pointer = m_pointer->next;
    return *this;
}

template<typename T>
typename List<T>::const_iterator List<T>::const_iterator::operator++(int)
{
    const_iterator tmp = *this;
    m_pointer = m_pointer->next;
    return tmp;
}

template<typename T>
typename List<T>::const_iterator &List<T>::const_iterator::operator--()
{
    m_pointer = m_pointer->prev;
    return *this;
}

template<typename T>
typename List<T>::const_iterator List<T>::const_iterator::operator--(int)
{
    const_iterator tmp = *this;
    m_pointer = m_pointer->prev;
    return tmp;
}

template<typename T>
bool List<T>::const_iterator::operator==(const List<T>::const_iterator &other) const
{
    return m_pointer == other.m_pointer;
}

template<typename T>
bool List<T>::const_iterator::operator!=(const List<T>::const_iterator &other) const
{
    return m_pointer != other.m_pointer;
}

// list implementation

template <typename T>
List<T>::List()
{
    m_size = 0;
    m_begin = m_end = nullptr;
}

template <typename T>
List<T>::List(std::size_t count, const T& element)
{
    m_size = 0;
    m_begin = m_end = nullptr;

    while(count--){
        push_back(element);
    }
}

template <typename T>
List<T>::List(const std::initializer_list<T> &elements)
{
    m_size = 0;
    m_begin = m_end = nullptr;

    for(auto e : elements){
        push_back(e);
    }
}

template <typename T>
List<T>::List(const List<T> &other)
{
    m_size = other.m_size;
    m_begin = m_end = nullptr;

    auto node = other.m_begin;

    while(node){
        this->push_back(node->element);
        node = node->next;
    }
}

template <typename T>
List<T>& List<T>::operator=(const List<T>& other){
    if(this == &other){
        return *this;
    }

    m_size = other.m_size;
    auto node = other.m_begin;

    while(node){
        this->push_back(node->element);
        node = node->next;
    }
}

template <typename T>
List<T>::List(List<T> &&other) noexcept
{
    m_size = 0;
    m_begin = m_end = nullptr;

    std::swap(m_size, other.m_size);
    std::swap(m_begin, other.m_begin);
    std::swap(m_end, other.m_end);
}

template <typename T>
List<T>& List<T>::operator=(List<T>&& other) noexcept {
    if(this == &other){
        return *this;
    }

    m_size = 0;
    m_begin = m_end = nullptr;

    std::swap(m_size, other.m_size);
    std::swap(m_begin, other.m_begin);
    std::swap(m_end, other.m_end);
}

template <typename T>
List<T>::~List()
{
    auto ptr = m_begin;

    while(ptr){
        auto prev = ptr;
        ptr = ptr->next;

        delete prev;
    }
}

template <typename T>
T& List<T>::at(std::size_t index)
{
    if(m_size <= index){
        throw std::out_of_range("out of range exception");
    }

    return operator[](index);
}

template <typename T>
T& List<T>::operator[](std::size_t index)
{
    auto ptr = m_begin;

    while(index--){
        ptr = ptr->next;
    }

    return ptr->element;
}

template <typename T>
const T& List<T>::at(std::size_t index) const
{
    if(m_size <= index){
        throw std::out_of_range("out of range exception");
    }

    return operator[](index);
}

template <typename T>
const T& List<T>::operator[](std::size_t index) const
{
    auto ptr = m_begin;

    while(index--){
        ptr = ptr->next;
    }

    return ptr->element;
}

template <typename T>
typename List<T>::iterator List<T>::begin()
{
    return List::iterator(m_begin);
}

template <typename T>
typename List<T>::iterator List<T>::end()
{
    return List::iterator(nullptr);
}

template <typename T>
typename List<T>::const_iterator List<T>::begin() const
{
    return List::const_iterator(m_begin);
}

template <typename T>
typename List<T>::const_iterator List<T>::end() const
{
    return List::const_iterator(nullptr);
}

template <typename T>
T& List<T>::front()
{
    return m_begin->element;
}

template <typename T>
T& List<T>::back()
{
    return m_end->element;
}

template <typename T>
const T& List<T>::front() const
{
    return m_begin->element;
}

template <typename T>
const T& List<T>::back() const
{
    return m_end->element;
}

template <typename T>
void List<T>::push_front(const T& element)
{
    auto member = new node(element);
    member->prev = nullptr;
    member->next = m_begin;

    if(m_begin){
        m_begin->prev = member;
        m_begin = member;
    }
    else{
        m_begin = member;
        m_end = member;
    }

    m_size++;
}

template <typename T>
void List<T>::pop_front()
{
    auto member = m_begin->next;

    delete m_begin;

    if(member){
        member->prev = nullptr;
        m_begin = member;
    }
    else{
        m_begin = nullptr;
        m_end = nullptr;
    }

    m_size--;
}

template <typename T>
void List<T>::push_back(const T& element)
{
    auto* member = new node(element);
    member->prev = m_end;
    member->next = nullptr;

    if(m_end){
        m_end->next = member;
        m_end = member;
    }
    else{
        m_begin = member;
        m_end = member;
    }

    m_size++;
}

template <typename T>
void List<T>::pop_back()
{
    auto member = m_end->prev;

    delete m_end;

    if(member){
        member->next = nullptr;
        m_end = member;
    }
    else{
        m_begin = nullptr;
        m_end = nullptr;
    }

    m_size--;
}

template <typename T>
void List<T>::insert(std::size_t index, const T& element)
{
    if(index == 0){
        push_front(element);
    }
    else if(index == m_size){
        push_back(element);
    }
    else{
        auto ptr = m_begin;

        while(index--){
            ptr = ptr->next;
        }

        auto prev = ptr->prev;
        auto next = ptr;

        auto member = new node(element);
        member->prev = prev;
        member->next = next;

        prev->next = member;
        next->prev = member;

        m_size++;
    }
}

template <typename T>
void List<T>::erase(std::size_t index)
{
    auto ptr = m_begin;

    while(index--){
        ptr = ptr->next;
    }

    if(ptr == m_begin){
        pop_front();
    }
    else if(ptr == m_end){
        pop_back();
    }
    else{
        auto prev = ptr->prev;
        auto next = ptr->next;

        delete ptr;

        prev->next = next;
        next->prev = prev;

        m_size--;
    }
}

template <typename T>
std::size_t List<T>::size() const
{
    return m_size;
}

template<typename T>
bool List<T>::empty() const
{
    return m_size == 0;
}