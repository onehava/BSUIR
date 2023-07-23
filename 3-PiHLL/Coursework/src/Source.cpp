#include <stdexcept>
#include "Source.h"

Source::Source(std::string text){
    bool ignore_case = true;
    m_content.push_back(&text.front());

    for(auto &curr : text){
        switch(curr){
            case '\n':
                m_content.push_back(&curr + 1);
                ignore_case = true;
                break;

            case '\"':
                ignore_case = not ignore_case;
                break;

            default:
                if(ignore_case){
                    curr = static_cast<char>(std::toupper(curr));
                }
        }
    }

    m_text = std::move(text);
}

Source::iterator Source::begin(){
    return {m_text.data()};
}

Source::iterator Source::end(){
    return {m_text.data() + m_text.size()};
}

Line Source::get_by_index(std::size_t index) const {
    try{
        return m_content.at(index - 1);
    }
    catch(...){
        throw std::out_of_range(
                "Runtime error: no line with index " + std::to_string(index));
    }
}

Line::Line(const char *pointer) : m_pointer(pointer){}

std::ostream &operator<<(std::ostream &ostream, const Line& line){
    auto curr = line.m_pointer;

    while(!(*curr == '\n' || *curr == '\0')){
        ostream << *curr;
        ++curr;
    }

    ostream << std::endl;
    return ostream;
}

Source::iterator::iterator(){
    m_cur = nullptr;
    m_row = 1;
    m_pos = 1;
}

Source::iterator::iterator(const char* pointer){
    m_cur = pointer;
    m_row = 1;
    m_pos = 1;
}

const char& Source::iterator::operator*() const{
    return *m_cur;
}

Source::iterator& Source::iterator::operator++(){
    switch(*m_cur){
        case '\0':
            return *this;

        case '\n':
            ++m_cur;
            ++m_row;
            m_pos = 1;
            return *this;

        default:
            ++m_cur;
            ++m_pos;
            return *this;
    }
}

Source::iterator Source::iterator::operator++(int){
    iterator temp = *this;
    ++(*this);
    return temp;
}

bool Source::iterator::operator!=(const iterator &other) const {
    return m_cur != other.m_cur;
}

bool Source::iterator::operator==(const iterator &other) const {
    return m_cur == other.m_cur;
}

const char *Source::iterator::get_ptr() const {
    return m_cur;
}

std::size_t Source::iterator::get_row() const {
    return m_row;
}

std::size_t Source::iterator::get_pos() const {
    return m_pos;
}