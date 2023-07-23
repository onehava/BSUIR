#ifndef LABORATORY_MESSAGE_H
#define LABORATORY_MESSAGE_H

#include "Binary.h"
#include "Text.h"

class Message : public Binary::Serialize, public Binary::Deserialize, public Text::Serialize, public Text::Deserialize {
public:
    Message();
    Message(std::uint64_t sender_id, std::uint64_t message_id, std::string content);
    std::uint64_t get_sender_id() const;
    std::uint64_t get_message_id() const;
    const std::string& get_content() const;

    bool operator==(const Message& other) const;
    bool operator!=(const Message& other) const;

    void binary_serialize(std::ostream &ostream) const override;
    void binary_deserialize(std::istream &istream) override;

    void text_serialize(std::ostream &ostream) const override;
    void text_deserialize(std::istream &istream) override;

private:
    std::uint64_t m_sender_id, m_message_id;
    std::string m_content;
};

#endif
