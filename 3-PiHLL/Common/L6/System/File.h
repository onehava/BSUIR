#ifndef LABORATORY_FILE_H
#define LABORATORY_FILE_H

#include <string>
#include <fstream>

class File {
public:
    bool is_open() const;
    void close();

protected:
    std::fstream m_stream;

    File(const std::string& filename, std::ios_base::openmode mode);
    File(const std::string& filename);
};

#endif
