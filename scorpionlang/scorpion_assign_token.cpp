#include "scorpion_assign_token.h"
#include <iostream>

int Scorpion::Parser::TokenProviders::AssignTokenProvider::tokenize(std::string *line_part, std::vector<line *>::iterator *line)
{ 
	int ispos = 0;
	int parsed = 0;
	if((ispos = line_part->find("=")) != std::string::npos && ispos != 0){ 
		Token *atoken = new Token(SCORPION_T_ASSIGN);
		Token *lhs = new Token(SCORPION_T_LHS);
		Token *rhs = new Token(SCORPION_T_RHS);
		constant_token *lhsVal = parse_value(&(line_part->substr(0, ispos)));
		parsed += ispos;
		lhs->addConstant(lhsVal);
		atoken->addToken(lhs);
		atoken->addToken(rhs);
		try{
			if(lhsVal->type == SCORPION_CT_STRING_CONSTANT || lhsVal->type == SCORPION_CT_INT_CONSTANT)
				throw ParseException("Can't assign to a constant");
			rhs->addConstant(parse_value(&(line_part->substr(ispos + 1, line_part->length() - ispos - 1))));
			parsed = line_part->length();
		
		}catch(ParseException &ex){
			delete atoken;
			throw ex;
		}
		
		(**line)->tokens.push_back(atoken);

		return parsed;
	}
	return 0;
}