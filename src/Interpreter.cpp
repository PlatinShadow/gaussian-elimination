#include "Interpreter.h"
#include <iostream>

#define EXPECT(token,x) if(token != x) return false;
#define EXPECT_EITHER(token,x,y) if(token != x && token != y) return false;


namespace LES {
    /**
     * TOKEN
    */
    Token::Token(Token::Type type, const std::string& value) 
    : m_Type(type), m_Value(value) {}

    Token::Token(Token::Type type, char value) 
    : m_Type(type), m_Value(1, value) {}

    std::string Token::asString() const {
        return std::string(m_Value);
    }

    int Token::asInt() const {
        return std::stoi(m_Value);
    }

    bool Token::operator==(Type other) const {
        return m_Type == other;
    }

    Token::Type Token::getType() const {
        return m_Type;
    }


    /**
     * Lexer
    */
    Lexer::Lexer(const std::string& input) : m_Input(input), m_Index(0) {}

    char Lexer::peek() {
        return m_Input[m_Index];
    }

    char Lexer::eat() { 
        return m_Input[m_Index++];
    }

    Token Lexer::number() {
        std::stringstream stream;

        while(std::isdigit(peek())) {
            stream << eat();
        }

        return Token(Token::Type::NUMBER, stream.str());
    }

    Token Lexer::identifier() {
        std::stringstream stream;

        while(std::isalnum(peek())) {
            stream << eat();
        }

        return Token(Token::Type::IDENTIFIER, stream.str());
    }

    Token Lexer::next() {
        while (std::isspace(peek())) eat(); //Skip whitespaces

        if(std::isdigit(peek())) {
            return number();
        }

        if(std::isalpha(peek())) {
            return identifier();
        }

        switch (peek()) {
            case '\0': return Token(Token::Type::END, '\0');
            case '+': return Token(Token::Type::SIGN, eat());
            case '-': return Token(Token::Type::SIGN, eat());
            default: return Token(Token::Type::UNKNOWN, eat());
        }
    }

    /**
     * Interpreter
    */
    Interpreter::State Interpreter::parserState0(Token& t) {
        if(t == Token::Type::SIGN) {

            return State::P1;
        } else if(t == Token::Type::NUMBER) {

            return State::P2;
        } else if(t == Token::Type::IDENTIFIER) {
      
            return State::P3;
        }

        return State::INVALID;
    }

    Interpreter::State Interpreter::parserState1(Token& t) {
        if(t == Token::Type::NUMBER) {
            return State::P2;
        } else if (t == Token::Type::IDENTIFIER) {
            return State::P3;
        }
        
        return State::INVALID;
    }

    Interpreter::State Interpreter::parserState2(Token& t) {
        if(t == Token::Type::IDENTIFIER) {
            return State::P3;
        }

        return State::INVALID;
    }

    Interpreter::State Interpreter::parserState3(Token& t) {
        if(t == Token::Type::SIGN) {
            return State::P1;
        }

        return State::INVALID;
    }


    bool Interpreter::processLine(std::string& in) {
        Lexer lexer(in);
        m_State = State::P0;

        //We are parsing this lang:
        //(+|-)*((+|-)(NUMBER IDENTIFIER|IDENTIFIER))+

        int scalar = 0;
        std::string literal;

        for(Token token = lexer.next(); token != Token::Type::END; token = lexer.next()) {

            switch (m_State) {
                case State::P0: m_State = parserState0(token); break;
                case State::P1: m_State = parserState1(token); break;
                case State::P2: m_State = parserState2(token); break;
                case State::P3: m_State = parserState3(token); break;

                case State::INVALID: return false;
                default: std::cerr << "Error: invalid parser state." << std::endl; abort(); break;
            }

        }

        //Only return true if we end on an Identifier
        return m_State == State::P3;
    }
    
} // namespace LES