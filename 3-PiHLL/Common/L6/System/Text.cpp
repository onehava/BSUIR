#include "Text.h"

Text::Text(const std::string &filename)
    : File(filename){}

Text::Text(const std::string &filename, std::ios_base::openmode mode)
    : File(filename, mode){}

void Text::write(const Serialize &source){
    source.text_serialize(m_stream);
}

void Text::read(Deserialize &destination){
    destination.text_deserialize(m_stream);
}