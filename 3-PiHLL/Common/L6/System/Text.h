#ifndef LABORATORY_TEXT_H
#define LABORATORY_TEXT_H

#include "File.h"
#include <string>

class Text : public File{
public:
    class Serialize;
    class Deserialize;

    Text(const std::string& filename);
    Text(const std::string& filename, std::ios_base::openmode mode);

    void write(const Serialize& source);
    void read(Deserialize& destination);
};

class Text::Serialize {
public:
    friend class Text;
    virtual void text_serialize(std::ostream& ostream) const = 0;
};

class Text::Deserialize {
public:
    friend class Text;
    virtual void text_deserialize(std::istream& istream) = 0;
};

#endif
