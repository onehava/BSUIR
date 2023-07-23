#include "Binary.h"

Binary::Binary(const std::string &filename)
      : File(filename){}

Binary::Binary(const std::string &filename, std::ios_base::openmode mode)
      : File(filename, mode | std::ios_base::binary){}

void Binary::write(const Binary::Serialize &source){
    source.binary_serialize(m_stream);
}

void Binary::read(Deserialize &destination){
    destination.binary_deserialize(m_stream);
}