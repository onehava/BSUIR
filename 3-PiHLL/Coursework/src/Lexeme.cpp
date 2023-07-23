#include "Lexeme.h"
#include <unordered_map>

Lexeme::Lexeme(){
    m_type = Lexeme::Unknown;
    m_row = -1;
    m_pos = -1;
}

Lexeme::Lexeme(Lexeme::Type type, std::string_view view,
               std::size_t row, std::size_t pos){
    m_type = type;
    m_view = view;
    m_row = row;
    m_pos = pos;
}

Lexeme::Type Lexeme::get_type() const {
    return m_type;
}

bool Lexeme::has_type(Type type) const {
    return m_type == type;
}

std::string_view Lexeme::get_view() const {
    return m_view;
}

std::size_t Lexeme::get_row() const {
    return m_row;
}

std::size_t Lexeme::get_pos() const {
    return m_pos;
}

Lexeme::Type to_lexeme_type(std::string_view view){
    static std::unordered_map<std::string_view, Lexeme::Type> conversion = {
            {"AND",   Lexeme::And}, {"OR", Lexeme::Or}, {"NOT", Lexeme::Not},
            {"LET",   Lexeme::Let},
            {"INPUT", Lexeme::Input}, {"PRINT", Lexeme::Print},
            {"GOTO",  Lexeme::Goto},
            {"IF",    Lexeme::If}, {"THEN", Lexeme::Then}, {"ELSE", Lexeme::Else},
            {"ENDIF", Lexeme::Endif},
            {"FOR",   Lexeme::For}, {"TO", Lexeme::To}, {"STEP", Lexeme::Step},
            {"NEXT", Lexeme::Next},
            {"WHILE", Lexeme::While}, {"WEND", Lexeme::Wend},
    };

    auto it = conversion.find(view);
    return it == conversion.end() ? Lexeme::Ident : it->second;
}

std::string_view to_string_view(Lexeme::Type type){
    static const char* const conversion[] = {
            "Floating", "Integer", "String", "Ident", "Unknown",
            "Comma", "Colon", "Semicolon",
            "Equals", "NotEquals", "Less", "LessEquals", "Greater", "GreaterEquals",
            "OpenParen", "CloseParen",
            "Plus", "Minus", "Asterisk", "Slash",
            "And", "Or", "Not",
            "Let",
            "Input", "Print",
            "Goto",
            "If", "Then", "Else", "EndIf",
            "For", "To", "Step", "Next",
            "While", "Wend",
            "Eol",
            "Eof",
    };

    return conversion[type];
}