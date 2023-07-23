//
// Created by Inqognitoo on 13.11.2022.
//

#ifndef LABORATORY_MY_STRING_H
#define LABORATORY_MY_STRING_H

#include <cstddef>
#include <istream>
#include <ostream>
#include <cstring>

class String
{
public:
    String();
    String(const char* source);
    String(const String& source);
    ~String();

    String& operator+(const String& second);
    String& operator=(const String& second);
    String& operator+=(const String& second);

    bool operator==(const String& second);
    bool operator!=(const String& second);

    char& operator[](std::size_t index);
    String& operator()(std::size_t begin, std::size_t length = -1);

    friend std::ostream& operator<<(std::ostream& ostream, const String& string);
    friend std::istream& operator>>(std::istream& istream, String& string);

    std::size_t length() const;
    std::size_t capacity() const;

    const char* c_str();

    void reserve(std::size_t capacity);

private:
    static void mem_reserve(String* dest);
    static void mem_extend(String* dest);

    std::size_t m_length = 0;
    std::size_t m_capacity = 0;

    char* m_container = nullptr;
};


#endif //LABORATORY_MY_STRING_H
