#ifndef LABORATORY_BINARY_H
#define LABORATORY_BINARY_H

#include "File.h"
#include <string>

class Binary : public File {
public:
    class Serialize;
    class Deserialize;

    Binary(const std::string& filename);
    Binary(const std::string& filename, std::ios_base::openmode mode);

    void write(const Serialize& source);
    void read(Deserialize& destination);
};

class Binary::Serialize {
public:
    friend class Binary;
    virtual void binary_serialize(std::ostream& ostream) const = 0;
};

class Binary::Deserialize {
public:
    friend class Binary;
    virtual void binary_deserialize(std::istream& istream) = 0;
};

#endif
