#ifndef SCORPION_PARSER_H
#define SCORPION_PARSER_H

#include <vector>
#include <string>
#include <exception>
#include <sstream>


#define SCORPION_PARSER_VERSION "1.0.0-a"

#define COMMENT_CHAR	"##"
#define CR_CHAR			"\r"
#define TAB_CHAR		"\t"

#ifdef WIN32

#ifdef SCORPION_BUILD
#define SCORPIONAPI __declspec(dllexport)
#else
#define SCORPIONAPI __declspec(dllimport)
#endif

#else
#define SCORPIONAPI
#endif

#define SCORPION_T_FUNCTION_CALL	11
#define SCORPION_CT_FUNCTION_NAME	11

#define SCORPION_T_PARAMS			21
#define SCORPION_T_PARAM			22
#define SCORPION_T_ARGS				23
#define SCORPION_T_ARG				24

#define SCORPION_CT_STRING_CONSTANT		31
#define SCORPION_CT_INT_CONSTANT		32
#define SCORPION_CT_VARIABLE			33
#define SCORPION_CT_ARG_COUNT			34
#define SCORPION_CT_PARAM_COUNT			35

namespace Scorpion {
	namespace Parser {
		typedef struct _constant_token {
			int type;
			std::string svalue;
			int			ivalue;
		} constant_token;

		class SCORPIONAPI Token {
		public:
			std::vector<Token *> inner_tokens;
			std::vector<constant_token *> constants;
			int type;

			Token(int type);
			~Token();

			void addConstant(int type, std::string value);
			void addConstant(int type, int value);
			void addConstant(constant_token *constant);
			bool hasConstant(int type);
			constant_token* getConstant(int type);

			void addToken(Token *token);
			bool hasToken(int type);
			Token* getToken(int type);
		};


		typedef struct _line 
		{
			int depth;
			int number;
			std::string str;
			std::string::iterator str_it;
			std::vector<Token *> tokens;
		} line;

		class SCORPIONAPI TokenProvider {
		public:
			virtual int tokenize(std::string *line_part, std::vector<line *>::iterator *sline) = 0;
			virtual ~TokenProvider() {}
		};

		class SCORPIONAPI Tokenizer {
		public:
		    std::vector<Scorpion::Parser::TokenProvider *> providers;
			std::vector<Scorpion::Parser::TokenProvider *> rhsProviders;

			Tokenizer();
			~Tokenizer();
			std::vector<Scorpion::Parser::line *> tokenize(std::string *code);
			void tokenizeRHS(std::string *code, std::vector<Scorpion::Parser::Token *> *tokens);
			void tokenizeLHS(std::string *code, std::vector<Scorpion::Parser::Token *> *tokens);
		};

		

		class SCORPIONAPI ParseException : public std::exception { 
		public:
			std::string message;

			ParseException() throw();
			ParseException(const std::string message) throw();
			~ParseException() throw() { }
			const char* what() const throw();
		};

		void string_replace(std::string *str, std::string target, std::string replacement);
		bool string_empty(std::string str);

		void filter_line(std::string *code);
		line* parse_line(std::string *code);
		std::vector<line *> parse_lines(std::string *code);
		constant_token* parse_value(std::string *value);

	};
};

#endif