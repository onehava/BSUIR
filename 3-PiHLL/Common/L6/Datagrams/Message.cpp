#include <iomanip>
#include "Message.h"

// Message class implementation

Message::Message(){
    m_sender_id = -1;
    m_message_id = -1;
}

Message::Message(std::uint64_t sender_id, std::uint64_t message_id, std::string content){
    m_sender_id = sender_id;
    m_message_id = message_id;
    m_content = std::move(content);
}

std::uint64_t Message::get_sender_id() const {
    return m_sender_id;
}

std::uint64_t Message::get_message_id() const {
    return m_message_id;
}

const std::string &Message::get_content() const {
    return m_content;
}

bool Message::operator==(const Message &other) const {
    return (m_sender_id == other.m_sender_id)
           &&
           (m_message_id == other.m_message_id)
           &&
           (m_content == other.m_content);
}

bool Message::operator!=(const Message &other) const {
    return not operator==(other);
}

// Binary serde implementation

void Message::binary_serialize(std::ostream &ostream) const {
    ostream.write(reinterpret_cast<const char*>(&m_sender_id), sizeof(m_sender_id));
    ostream.write(reinterpret_cast<const char*>(&m_message_id), sizeof(m_message_id));

    auto size = m_content.size();
    auto data = m_content.data();

    ostream.write(reinterpret_cast<const char*>(&size), sizeof(size));
    ostream.write(data, static_cast<std::streamsize>(size));
}

void Message::binary_deserialize(std::istream &istream) {
    istream.read(reinterpret_cast<char*>(&m_sender_id), sizeof(m_sender_id));
    istream.read(reinterpret_cast<char*>(&m_message_id), sizeof(m_message_id));

    std::string::size_type size;
    istream.read(reinterpret_cast<char*>(&size), sizeof(size));

    m_content = std::string(size, 0);
    istream.read(&m_content[0], static_cast<std::streamsize>(size));
}

// Text serde implementation

void Message::text_serialize(std::ostream &ostream) const {
    ostream << m_sender_id << ' ' << m_message_id << ' ' << std::quoted(m_content) << '\n';
}

void Message::text_deserialize(std::istream &istream){
    istream >> m_sender_id >> m_message_id >> std::quoted(m_content);

}