#include "Lexer.h"
#include "Error.h"

bool Character::is_alpha(char c){
    return c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z';
}

bool Character::is_digit(char c){
    return c >= '0' && c <= '9';
}

bool Character::is_space(char c){
    return c == ' ' || c == '\t' || c == '\r';
}

bool Character::is_ident(char c){
    return c == '_' || is_alpha(c) || is_digit(c);
}

Lexemas Lexer::tokenize(Source::iterator source){
    m_curr = source;
    Lexemas lexemas;

    do{
        Lexeme lexeme = next();
        lexemas.push_back(lexeme);

        if(lexeme.has_type(Lexeme::Eof)){
            break;
        }
    }
    while(true);

    return std::move(lexemas);
}

Lexeme Lexer::next(){
    do{
        while(Character::is_space(*m_curr)){
            m_curr++;
        }

        if(*m_curr == '\''){
            while(!(*m_curr == '\n' || *m_curr == '\0')) {
                m_curr++;
            }
        }
        else{
            break;
        }
    }
    while(true);

    m_prev = m_curr;
    return make(read_lexeme());
}

Lexeme Lexer::make(Lexeme::Type type){
    return {
            type,
            std::string_view(m_prev.get_ptr(), m_curr.get_ptr()),
            m_prev.get_row(),
            m_prev.get_pos(),
    };
}

bool Lexer::match(char required){
    if(*m_curr == required){
        m_curr++;
        return true;
    }
    else{
        return false;
    }
}

Lexeme::Type Lexer::read_lexeme(){
    char peek = *m_curr++;

    switch(peek){
        case '\n': return Lexeme::Eol;
        case '\0': return Lexeme::Eof;

        case ',': return Lexeme::Comma;
        case ':': return Lexeme::Colon;
        case ';': return Lexeme::Semicolon;

        case '+': return Lexeme::Plus;
        case '-': return Lexeme::Minus;
        case '*': return Lexeme::Asterisk;
        case '/': return Lexeme::Slash;

        case '(': return Lexeme::OpenParen;
        case ')': return Lexeme::CloseParen;

        case '=': return Lexeme::Equals;
        case '>': return match('=') ? Lexeme::GreaterEquals :
                         Lexeme::Greater;
        case '<': return match('>') ? Lexeme::NotEquals :
                         match('=') ? Lexeme::LessEquals : Lexeme::Less;

        default: break;
    }

    if(peek == '"'){
        return read_string();
    }

    if(Character::is_digit(peek)){
        return read_number();
    }

    if(Character::is_ident(peek)){
        return read_unique();
    }

    throw UnknownSymbolError(m_prev.get_row(), m_prev.get_pos(), *m_prev);
}

Lexeme::Type Lexer::read_number(){
    while(Character::is_digit(*m_curr)){
        m_curr++;
    }

    if(match('.')){
        while(Character::is_digit(*m_curr)){
            m_curr++;
        }

        return Lexeme::Floating;
    }

    return Lexeme::Integer;
}

Lexeme::Type Lexer::read_string(){
    while(true){
        if(*m_curr == '\n' || *m_curr == '\0'){
            throw UnterminatedStringError(m_prev.get_row(),
                                          m_prev.get_pos());
        }

        if(*m_curr++ == '"'){
            break;
        }
    }

    return Lexeme::String;
}

Lexeme::Type Lexer::read_unique(){
    while(Character::is_ident(*m_curr)){
        m_curr++;
    }

    return to_lexeme_type({m_prev.get_ptr(), m_curr.get_ptr()});
}