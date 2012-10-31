#include "scorpion_function_call_token.h"
#include <iostream>

int Scorpion::Parser::TokenProviders::FunctionCallTokenProvider::tokenize(std::string *line_part, std::vector<line *>::iterator *sline)
{ 
	int spos = 0;
	int parsed = 0;
	if((spos = line_part->find("[")) != std::string::npos && spos != 0){ 
		Token *ftoken = new Token(SCORPION_T_FUNCTION_CALL);
		ftoken->addConstant(SCORPION_CT_FUNCTION_NAME, line_part->substr(0, spos));
		parsed += spos;
		int epos = line_part->find_last_not_of("]");
		if(epos == std::string::npos || spos > epos)
			return 0;
		std::string arglist = line_part->substr(spos + 1, epos - spos);
		Token *args = new Token(SCORPION_T_ARGS);
		if(!string_empty(arglist)){
			int commapos = arglist.find(",");
			int prevpos = 0;
			try{
				if(commapos == std::string::npos) 
					args->addConstant(parse_value(&arglist));
				else{
					while(commapos != std::string::npos){
						args->addConstant(parse_value(&(arglist.substr(prevpos, commapos - prevpos))));
						prevpos = commapos + 1;
						commapos = arglist.find(",", prevpos);
					}
					if(prevpos == arglist.length())
						throw ParseException("Missing argument in argument list");
					args->addConstant(parse_value(&(arglist.substr(prevpos, arglist.length() - prevpos))));
				}
			}catch(ParseException &ex){
				delete args;
				delete ftoken;
				throw ex;
			}
		}
		ftoken->addToken(args);
		(**sline)->tokens.push_back(ftoken);
		parsed += 2;
		parsed += epos - spos;
		return parsed;
	}
	return 0;
}