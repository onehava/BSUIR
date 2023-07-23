#include "History.h"

// History class implementation

History::History() = default;

History::History(List<Message> &&messages) : m_messages(std::move(messages)){}

History::History(const List<Message>& messages) : m_messages(messages){}



const List<Message> &History::get_messages() const {
    return m_messages;
}

// Binary serde implementation

void History::binary_serialize(std::ostream &ostream) const {
    auto size = m_messages.size();
    ostream.write(reinterpret_cast<char*>(&size), sizeof(size));

    for(const auto& message : m_messages){
        message.binary_serialize(ostream);
    }
}

void History::binary_deserialize(std::istream &istream) {
    std::size_t size;
    istream.read(reinterpret_cast<char*>(&size), sizeof(size));

    while(size--){
        Message message;
        message.binary_deserialize(istream);
        m_messages.push_back(message);
    }
}

bool History::operator==(const History &other) const {
    if(this == &other){
        return true;
    }

    const auto& ma = m_messages;
    const auto& mb = other.m_messages;

    if(ma.size() != mb.size()){
        return false;
    }

    auto i = ma.begin();
    auto j = mb.begin();

    while(!(i == ma.end() || j == mb.end())){
        if(*i == *j){
            ++i;
            ++j;
        }
        else{
            return false;
        }
    }

    return true;
}

bool History::operator!=(const History &other) const {
    return not operator==(other);
}

// Text serde implementation

void History::text_serialize(std::ostream &ostream) const {
    auto size = m_messages.size();
    ostream << size << '\n';

    for(const auto& message : m_messages){
        message.text_serialize(ostream);
    }
}

void History::text_deserialize(std::istream &istream) {
    std::size_t size;
    istream >> size;

    while(size--){
        Message message;
        message.text_deserialize(istream);
        m_messages.push_back(message);
    }
}