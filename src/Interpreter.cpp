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
    Interpreter::Interpreter() 
        : m_State(State::START), m_CurrentSign(1), m_CurrentScalar(1), m_CurrentIdentifier("") {}
   
    static int intFromSign(const std::string& sign) {
        return sign == "+" ? 1 : -1;
    }

    Interpreter::State Interpreter::parserStateStart(Token& t) {
        if(t == Token::Type::SIGN) {
            m_CurrentSign = intFromSign(t.asString());
            return State::SIGN;
        } else if(t == Token::Type::NUMBER) {
            m_CurrentScalar = t.asInt();
            return State::NUMBER;
        } else if(t == Token::Type::IDENTIFIER) {
            m_CurrentIdentifier = t.asString();
            return State::IDENTIFIER;
        }

        std::cerr << "Syntax Error: Expected NUMBER or IDENTIFIER or SIGN" << std::endl;

        return State::INVALID;
    }

    Interpreter::State Interpreter::parserStateSign(Token& t) {
        if(t == Token::Type::NUMBER) {
            m_CurrentScalar = t.asInt();
            return State::NUMBER;
        } else if (t == Token::Type::IDENTIFIER) {
            m_CurrentIdentifier = t.asString();
            return State::IDENTIFIER;
        }

        std::cerr << "Syntax Error: Expected NUMBER or IDENTIFIER" << std::endl;
        
        return State::INVALID;
    }

    Interpreter::State Interpreter::parserStateNumber(Token& t) {
        if(t == Token::Type::IDENTIFIER) {
            m_CurrentIdentifier = t.asString();
            return State::IDENTIFIER;
        }

        std::cerr << "Syntax Error: Expected IDENTIFIER" << std::endl;

        return State::INVALID;
    }

    Interpreter::State Interpreter::parserStateIdentifier(Token& t) {
        //Final state, save the parsed term
        m_Terms.push_back({
            m_CurrentSign * m_CurrentScalar,
            m_CurrentIdentifier
        });

        //Reset
        m_CurrentSign = 1;
        m_CurrentIdentifier = "";
        m_CurrentScalar = 1;

        if(t == Token::Type::SIGN) {
            m_CurrentSign = intFromSign(t.asString());
            return State::SIGN;
        }

        return State::INVALID;
    }


    std::optional<std::vector<Term>> Interpreter::processLine(std::string& in) {
        m_State = State::START;
        m_Terms.clear();
        m_CurrentIdentifier = "";
        m_CurrentScalar = 1;
        m_CurrentSign = 1;

        Lexer lexer(in);

        //We are parsing this lang:
        //(+|-)*((+|-)(NUMBER IDENTIFIER|IDENTIFIER))+

        for(Token token = lexer.next(); token != Token::Type::END; token = lexer.next()) {

            switch (m_State) {
                case State::START: m_State = parserStateStart(token); break;
                case State::SIGN: m_State = parserStateSign(token); break;
                case State::NUMBER: m_State = parserStateNumber(token); break;
                case State::IDENTIFIER: m_State = parserStateIdentifier(token); break;

                case State::INVALID: return {};
                default: std::cerr << "Error: invalid parser state." << std::endl; abort(); break;
            }

        }

        //Add remaining identifier
        if(m_State == State::IDENTIFIER) {
            m_Terms.push_back({
                m_CurrentSign * m_CurrentScalar,
                m_CurrentIdentifier
            });

            return m_Terms;
        } else {
            return {};
        }
    }
    
} // namespace LES