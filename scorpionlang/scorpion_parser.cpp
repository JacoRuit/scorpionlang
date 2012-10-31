#define SCORPION_BUILD 1
#include "scorpion_parser.h"

void Scorpion::Parser::string_replace(std::string *str, std::string target, std::string replacement)
{
	int pos = 0;
	while((pos = str->find(target, pos)) != std::string::npos)
	{
		str->replace(pos, target.length(), replacement);
	}
}

bool Scorpion::Parser::string_empty(std::string str)
{
	string_replace(&str, " ", "");
	return str.empty();
}

void Scorpion::Parser::filter_line(std::string *code)
{
	string_replace(code, CR_CHAR, "");
	int pos = code->find_first_of(COMMENT_CHAR);
	if(pos != std::string::npos)
		code->replace(pos, code->length() - pos, "");
}

Scorpion::Parser::line* Scorpion::Parser::parse_line(std::string *code)
{
	line *sline = new line;
	int pos = 0;
	while((pos = code->find(TAB_CHAR, pos)) != std::string::npos){
		sline->depth++;
		if(pos - sline->depth != 0){
			delete sline;
			throw ParseException("Line " + *code + " contains a tab in the middle of the line.");
		}
	}
	pos = pos == std::string::npos ? 0 : pos;
	sline->str = code->substr(pos, code->length());
	return sline;
}

std::vector<Scorpion::Parser::line *> Scorpion::Parser::parse_lines(std::string *code)
{
	std::stringstream stream(*code);
	std::vector<line *> lines;
	std::string cline;
	int current_line = 0;
	while(std::getline(stream, cline)){
		current_line++;
		filter_line(&cline);
		if(string_empty(cline))
			continue;
		line *sline = parse_line(&cline);
		sline->number = current_line;
		lines.push_back(sline);
	}
	return lines;
}

Scorpion::Parser::constant_token* Scorpion::Parser::parse_value(std::string *value) 
{
	int charc = 0;
	bool instr = false;
	bool invar = false;
	bool done = false;
	constant_token *token = new constant_token;
	std::stringstream builder;
	for(std::string::iterator it = value->begin(); it != value->end(); ++it){
		if(charc == 0 && *it == ' ') continue;
		if(done && *it != ' '){
			delete token;
			throw ParseException("Can't parse value " + *value);
		}
		charc++;
		if(*it == '"'){
			if(!instr){
				if(invar || charc > 1){
					delete token;
					throw ParseException("Can't parse variable name " + *value);
				}else
					instr = true;
			}
			else{
				instr = false;
				done = true;
				token->ivalue = 0;
				token->svalue = builder.str();
				token->type = SCORPION_CT_STRING_CONSTANT;
			}
		}else if(invar && (*it == ' ' || it == value->end() -  1)){
			builder << *it;
			invar = false;
			done = true;
			token->ivalue = 0;
			token->svalue = builder.str();
			token->type = SCORPION_CT_VARIABLE;
		}else{
			if(!instr && !invar)
				invar = true;
			builder << *it;
		}
	}
	return token;
}