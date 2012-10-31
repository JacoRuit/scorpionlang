#define SCORPION_BUILD 1
#include "scorpion_parser.h"
#include "scorpion_function_call_token.h"
#include "scorpion_assign_token.h"


Scorpion::Parser::Tokenizer::Tokenizer()
{
	this->providers.push_back(new Scorpion::Parser::TokenProviders::FunctionCallTokenProvider());
	this->providers.push_back(new Scorpion::Parser::TokenProviders::AssignTokenProvider());
}

std::vector<Scorpion::Parser::line *> Scorpion::Parser::Tokenizer::tokenize(std::string *code)
{
	std::vector<line *> lines = parse_lines(code);
	
	std::vector<line *>::iterator ptr = lines.begin();
	(*ptr)->str_it = (*ptr)->str.begin();
	while(true)
	{
		int parsed = 0;
		std::string line_part((*ptr)->str_it, (*ptr)->str.end());
		for(std::vector<TokenProvider *>::iterator it = this->providers.begin(); 
			it != this->providers.end(); ++it)
		{
			parsed = (*it)->tokenize(&line_part, &ptr);
			if(parsed > 0) break;
		}
		if(parsed < 1)
		{
			throw ParseException("Can't tokenize line \"" + (*ptr)->str + "\"");
		}
		while(parsed > 0){
			if(++((*ptr)->str_it) == (*ptr)->str.end()) {
				if(++ptr == lines.end())
					return lines;
				else {
					(*ptr)->str_it = (*ptr)->str.begin();
				}
			}
			parsed--;
		}
	}
	return lines;
}

Scorpion::Parser::Tokenizer::~Tokenizer()
{
	for(std::vector<TokenProvider *>::iterator it = this->providers.begin(); 
		it != this->providers.end(); ++it)
	{
		delete *it;	
	}for(std::vector<TokenProvider *>::iterator it = this->rhsProviders.begin(); 
		it != this->rhsProviders.end(); ++it)
	{
		delete *it;	
	}

}