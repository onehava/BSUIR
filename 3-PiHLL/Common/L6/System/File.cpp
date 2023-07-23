#include "File.h"

bool File::is_open() const{
    return m_stream.is_open();
}

void File::close(){
    m_stream.close();
}

File::File(const std::string &filename){
    m_stream.open(filename);
}

File::File(const std::string &filename, std::ios_base::openmode mode){
    m_stream.open(filename, mode);
}