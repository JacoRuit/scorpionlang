#define SCORPION_BUILD 1
#include "scorpion_parser.h"

Scorpion::Parser::ParseException::ParseException() throw() {
	this->message = "";
}

Scorpion::Parser::ParseException::ParseException(const std::string message) throw()
{
	this->message = message;
}

const char* Scorpion::Parser::ParseException::what() const throw()
{
	return "The Scorpion tokenizer couldn't parse a part of the script.";
}
