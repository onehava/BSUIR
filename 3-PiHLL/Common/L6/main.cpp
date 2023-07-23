#include <iostream>
#include <algorithm>

#include "list.h"
#include "Binary.h"
#include "History.h"

void write_binary(const History& history){
    // write
    Binary binary("binary_database.bin", std::ios::out);
    binary.write(history);
    binary.close();
}

void write_text(const History& history){
    // write
    Text text("text_database.txt", std::ios::out);
    text.write(history);
    text.close();
}

History read_binary(){
    // read
    Binary binary("binary_database.bin", std::ios::in);
    History history;
    binary.read(history);
    return history;
}

History read_text(){
    // read
    Text text("text_database.txt", std::ios::in);
    History history;
    text.read(history);
    return history;
}

int main(){
    List<Message> messages = {
        Message(0xCAFEBABE, 0, "Morning! Did you watch the anime we talked about yesterday?"),
        Message(0xDEAD10CC, 1, "Hay-o... Yes, and it was very satisfying."),
        Message(0xCAFEBABE, 2, "Oh, you think so? I'm glad you think so."),
        Message(0xCAFEBABE, 3, "What do you think about the main protagonist?"),
        Message(0xDEAD10CC, 4, "Hm... I think he acted like a jerk sometimes, but in the end..."),
        Message(0xDEAD10CC, 5, "...yes, actually, this was a very beautiful story."),
        Message(0xCAFEBABE, 6, "There is also film about their adventures on Mars, will you join?"),
        Message(0xDEAD10CC, 7, "With great pleasure."),
        Message(0xCAFEBABE, 8, "Okay, don't forget to grab some snacks then :p"),
        Message(0xDEAD10CC, 9, "'See you, space cowboy...'"),
    };

    History history(std::move(messages));

    // Test binary functionality

    write_binary(history);
    History binary_copy = read_binary();

    if(binary_copy == history){
        std::cout << "Binary copy equals to history" << std::endl;
    }

    // Test text functionality

    write_text(history);
    History text_copy = read_text();

    if(text_copy == history){
        std::cout << "Text copy equals to history" << std::endl;
    }
}