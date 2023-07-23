#ifndef LABORATORY_HISTORY_H
#define LABORATORY_HISTORY_H

#include "list.h"
#include "Binary.h"
#include "Message.h"

class History : public Binary::Serialize, public Binary::Deserialize, public Text::Serialize, public Text::Deserialize {
public:
    History();
    History(List<Message>&& messages);
    History(const List<Message>& messages);


    const List<Message>& get_messages() const;

    bool operator==(const History& other) const;
    bool operator!=(const History& other) const;

    void binary_serialize(std::ostream &ostream) const override;
    void binary_deserialize(std::istream &istream) override;

    void text_serialize(std::ostream &ostream) const override;
    void text_deserialize(std::istream &istream) override;

private:
    List<Message> m_messages;
};

#endif
