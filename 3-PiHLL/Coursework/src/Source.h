#pragma once

#include <string>
#include <vector>
#include <ostream>

class Line;

class Source {
public:
    class iterator;

    Source(std::string text);

    iterator begin();
    iterator end();

    Line get_by_index(std::size_t index) const;

private:
    std::string m_text;
    std::vector<Line> m_content;
};

class Line {
public:
    friend std::ostream& operator<<(std::ostream& ostream,
                                    const Line& line);

private:
    friend class Source;
    Line(const char* pointer);
    const char* const m_pointer;
};

class Source::iterator {
public:
    iterator();

    const char& operator*() const;
    iterator& operator++();
    iterator operator++(int);

    bool operator!=(const iterator& other) const;
    bool operator==(const iterator& other) const;

    const char* get_ptr() const;
    std::size_t get_row() const;
    std::size_t get_pos() const;

private:
    friend class Source;
    iterator(const char* pointer);

    const char* m_cur;
    std::size_t m_row;
    std::size_t m_pos;
};