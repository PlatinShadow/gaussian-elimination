#include <sstream>
#include <optional>
#include <vector>

namespace LES {

    /**
     * Token
     * Stores the value and type of token 
     * generaeted by the lexer
    */
    class Token {
    public:
        enum class Type {
            END = 0,
            UNKNOWN,
            IDENTIFIER,
            NUMBER,
            SIGN
        };

        Token(Type type, const std::string& value);
        Token(Type type, char value);

        Type getType() const;

        bool operator==(Type other) const;
   
        std::string asString() const;
        int asInt() const;

    private:
        Type m_Type;
        std::string m_Value;
    };


    /**
     * Lexer
     * Converts a string into tokens
    */
    class Lexer {
        public:
            /**
            * Constructor
            * @param input the string to be lexed 
            */
            Lexer(const std::string& input);

            /**
             * Returns the next token from the string
             * @returns said token 
            */
            Token next();

        private:
            Token number();
            Token identifier();
            char peek();
            char eat();

            std::string m_Input;
            size_t m_Index;
    };

    struct Term {
        int Scalar;
        std::string Literal;
    };

    /**
     * Interpreter
     * Interprets a given set of strings and returns the 
     * linear equation system in form of a matrix and coloumn-vector
     * 
     * The language (+|-)*((+|-)(NUMBER IDENTIFIER|IDENTIFIER))+ is
     * implemented as a finite state machine 
     */
    class Interpreter {
    public:
        Interpreter();

        /**
         * Lexes and Parses a line of input
         * @param line the line to lex and parse
         * @returns the terms parsed from the line (if no syntax error occured)
        */
        std::optional<std::vector<Term>> processLine(std::string& line);

    private:
        enum class State {
            START = 0,
            SIGN,
            NUMBER,
            IDENTIFIER,
            INVALID
        };

        State parserStateStart(Token& t);
        State parserStateSign(Token& t);
        State parserStateNumber(Token& t);
        State parserStateIdentifier(Token& t);

        State m_State;

        int m_CurrentSign;
        int m_CurrentScalar;
        std::string m_CurrentIdentifier;
        std::vector<Term> m_Terms;
    };

} // namespace LES
