//
// Created by Inqognitoo on 13.11.2022.
//

#include <iostream>
#include "my_string.h"

void String::mem_reserve(String *dest){
    auto length = dest->m_length;
    auto capacity = (length <= 15) ? 15: length;

    char* pointer = new char[capacity];

    dest->m_capacity = capacity;
    dest->m_container = pointer;
}

void String::mem_extend(String *dest){
    auto length = dest->m_length;
    auto capacity = dest->m_capacity;

    if(capacity > length){
        return;
    }

    while(capacity <= length){
        capacity <<= 2;
    }

    dest->m_capacity = capacity;

    char* pointer = new char[capacity];
    strcpy(pointer, dest->m_container);

    delete dest->m_container;
    dest->m_container = pointer;
}

String::String(){
    mem_reserve(this);
    m_container[0] = '\0';
}

String::String(const char *source){
    m_length = strlen(source);
    mem_reserve(this);
    strcpy(m_container, source);
}

String::String(const String &source) : String(source.m_container){}

String::~String(){
    delete m_container;
}

std::size_t String::length() const {
    return m_length;
}

std::size_t String::capacity() const {
    return m_capacity;
}

void String::reserve(std::size_t capacity){
    if(m_capacity < capacity){
        char* pointer = new char[capacity];
        strcpy(pointer, m_container);
        delete m_container;

        m_capacity = capacity;
        m_container = pointer;
    }
}

const char *String::c_str(){
    return m_container;
}

String &String::operator+(const String &second){
    std::size_t length = m_length + second.m_length;

    auto* result = new String;
    result->m_length = length;
    mem_extend(result);

    strcpy(result->m_container, m_container);
    strcpy(result->m_container + m_length, second.m_container);

    return *result;
}

String &String::operator=(const String &second){
    if(this == &second){
        return *this;
    }

    m_length = second.m_length;
    mem_extend(this);
    strcpy(m_container, second.m_container);

    return *this;
}

String &String::operator+=(const String &second){
    std::size_t prev = m_length;
    std::size_t curr = m_length + second.m_length;

    m_length = curr;
    mem_extend(this);
    strcpy(m_container + prev, second.m_container);

    return *this;
}

bool String::operator==(const String &second){
    if(m_length != second.m_length){
        return false;
    }

    if(strcmp(m_container, second.m_container) != 0){
        return false;
    }

    return true;
}

bool String::operator!=(const String &second){
    return !(*this == second);
}

char& String::operator[](const std::size_t index){
    return m_container[index];
}

String &String::operator()(std::size_t pos, std::size_t len){
    std::size_t length = std::min(len, m_length - pos);

    auto* substr = new String;
    substr->m_length = length;
    mem_extend(substr);
    memcpy(substr->m_container, m_container + pos, length);
    substr->m_container[length] = '\0';
    return *substr;
}

std::ostream& operator<<(std::ostream &ostream, const String &String){
    return ostream << String.m_container;
}

std::istream& operator>>(std::istream &istream, String& string){
    std::istream::sentry sentry(istream);

    if(!sentry){
        return istream;
    }

    std::size_t capacity = string.m_capacity;
    std::size_t index = 0;

    int next;

    while((next = istream.get()) != EOF && !std::iswspace(next)){
        if(index == capacity){
            string.m_length = capacity;
            String::mem_extend(&string);
        }

        string.m_container[index++] = static_cast<char>(next);
    }

    string.m_length = index;
    string.m_container[index] = '\0';

    return istream;
}