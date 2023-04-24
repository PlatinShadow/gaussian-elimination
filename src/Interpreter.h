#include <sstream>
#include <optional>

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
            Lexer(const std::string& input);
            Token next();

        private:
            Token number();
            Token identifier();
            char peek();
            char eat();

            std::string m_Input;
            size_t m_Index;
    };

    /**
     * Interpreter
     * Interprets a given set of strings and returns the 
     * linear equation system in form of a matrix and coloumn-vector
     */
    class Interpreter {
    public:
        Interpreter() = default;

        /**
         * Lexes and Parses a line of input
         * @param line the line to lex and parse
         * @returns true if there was no syntax error
        */
        bool processLine(std::string& line);    

    private:
        enum class State {
            P0 = 0,
            P1,
            P2,
            P3,
            INVALID
        };

        State parserState0(Token& t);
        State parserState1(Token& t);
        State parserState2(Token& t);
        State parserState3(Token& t);

        State m_State;
    };

} // namespace LES
