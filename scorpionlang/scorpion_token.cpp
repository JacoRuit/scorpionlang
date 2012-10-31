#include "scorpion_parser.h"


Scorpion::Parser::Token::Token(int type)
{
	this->type = type;
}

void Scorpion::Parser::Token::addConstant(int type, std::string value) 
{
	constant_token *constant = new constant_token;
	constant->type = type;
	constant->svalue = value;
	constant->ivalue = 0;
	this->constants.push_back(constant);
}

void Scorpion::Parser::Token::addConstant(int type, int value) 
{
	constant_token *constant = new constant_token;
	constant->type = type;
	constant->svalue = "";
	constant->ivalue = value;
	this->constants.push_back(constant);
}

void Scorpion::Parser::Token::addConstant(constant_token *constant) 
{
	this->constants.push_back(constant);
}

bool Scorpion::Parser::Token::hasConstant(int type)
{
	if(this->constants.size() < 1)
		return false;
	for(std::vector<constant_token *>::iterator it = this->constants.begin(); it != this->constants.end(); ++it){
		if((*it)->type == type)
			return true;
	}
}

Scorpion::Parser::constant_token* Scorpion::Parser::Token::getConstant(int type)
{
	for(std::vector<constant_token *>::iterator it = this->constants.begin(); it != this->constants.end(); ++it){
		if((*it)->type == type)
			return *it;
	}
}

void Scorpion::Parser::Token::addToken(Token *token) 
{
	this->inner_tokens.push_back(token);
}

bool Scorpion::Parser::Token::hasToken(int type)
{
	if(this->inner_tokens.size() < 1)
		return false;
	for(std::vector<Token *>::iterator it = this->inner_tokens.begin(); it != this->inner_tokens.end(); ++it){
		if((*it)->type == type)
			return true;
	}
}

Scorpion::Parser::Token* Scorpion::Parser::Token::getToken(int type)
{
	for(std::vector<Token *>::iterator it = this->inner_tokens.begin(); it != this->inner_tokens.end(); ++it){
		if((*it)->type == type)
			return *it;
	}
}

Scorpion::Parser::Token::~Token() 
{
	for(std::vector<constant_token *>::iterator it = this->constants.begin(); it != this->constants.end(); ++it){
		delete *it;
	}
	for(std::vector<Token *>::iterator it = this->inner_tokens.begin(); it != this->inner_tokens.end(); ++it){
		delete *it;
	}
}