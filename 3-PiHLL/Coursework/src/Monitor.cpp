#include "Monitor.h"
#include "Error.h"
#include <iostream>
#include <iomanip>

Monitor::Monitor(Source &source, process_t process)
        : m_process(std::move(process)), m_source(source){}

void Monitor::run(){
    try{
        std::invoke(m_process);
        return;
    }
    catch(UnknownSymbolError& error){
        std::stringstream ss;

        ss << "Syntax error: unknown symbol `"
           << error.get_symbol()
           << "` at "
           << std::setw(2) << error.get_row()
           << ':'
           << std::setw(2) << error.get_pos();

        indicate(error.get_row(), error.get_pos(), ss.str());
    }
    catch(UnterminatedStringError& error){
        std::stringstream ss;

        ss << "Syntax error: unterminated string at "
           << std::setw(2) << error.get_row()
           << ':'
           << std::setw(2) << error.get_pos();

        indicate(error.get_row(), error.get_pos(), ss.str());
    }
    catch(UnexpectedLexemeError& error){
        std::stringstream ss;

        auto expected = error.get_expected();
        auto unexpected = error.get_unexpected();

        if(expected != Lexeme::Unknown){
            ss << "Syntax error: expected `"
               << to_string_view(expected) << "` but `"
               << to_string_view(unexpected) << "` provided at "
               << std::setw(2) << error.get_row()
               << ':'
               << std::setw(2) << error.get_pos();

            indicate(error.get_row(), error.get_pos(), ss.str());
        }
    }
    catch(WrongExprTypeError& error){
        std::stringstream ss;

        Lexeme first = error.get_first();
        Lexeme last = error.get_last();

        auto row = first.get_row();
        auto a = first.get_pos();
        auto b = last.get_pos() + last.get_view().length();

        ss << "Type error: expected `"
           << to_string_view(error.get_expected()) << "` expression but `"
           << to_string_view(error.get_unexpected()) << "` provided at "
           << std::setw(2) << row
           << ':'
           << std::setw(2) << a;

        underline(row, a, b, ss.str());
    }
    catch(UnknownUnaryExprError& error){
        std::stringstream ss;

        auto lexeme = error.get_operation();
        auto op = lexeme.get_type();
        auto type = error.get_type();

        ss << "Type error: unknown unary operation `"
           << to_string_view(op) << "` with type `"
           << to_string_view(type) << "` at "
           << std::setw(2) << lexeme.get_row()
           << ':'
           << std::setw(2) << lexeme.get_pos();

        indicate(lexeme.get_row(), lexeme.get_pos(), ss.str());
    }
    catch(UnknownBinaryExprError& error){
        std::stringstream ss;

        auto lexeme = error.get_operation();
        auto left = error.get_left_type();
        auto right = error.get_right_type();
        auto op = lexeme.get_type();

        ss << "Type error: unknown binary operation `"
           << to_string_view(op) << "` with types `"
           << to_string_view(left) << "` and `"
           << to_string_view(right) << "` at "
           << std::setw(2) << lexeme.get_row()
           << ':'
           << std::setw(2) << lexeme.get_pos();

        indicate(lexeme.get_row(), lexeme.get_pos(), ss.str());
    }
    catch(std::exception& e){
        std::cerr << "Runtime error: " << e.what() << std::endl;
    }

    std::exit(64);
}

void Monitor::indicate(std::size_t row, std::size_t pos,
                       const std::string& message){
    auto line = m_source.get_by_index(row);
    auto indent = std::string(pos - 1, ' ');

    std::cerr << '\n' << line
              << indent << "^\n"
              << indent << message << std::endl;
}

void Monitor::underline(std::size_t row,
                        std::size_t pos1, std::size_t pos2,
                        const std::string& message){
    auto line = m_source.get_by_index(row);
    auto indent = std::string(pos1 - 1, ' ');
    auto waves = std::string(pos2 - pos1, '~');

    std::cerr << '\n' << line
              << indent << waves << '\n'
              << indent << "^\n"
              << indent << message << std::endl;
}