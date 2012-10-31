#define SCORPION_BUILD 1
#include "scorpion_engine.h"

void Scorpion::Class::invoke(SCORPION_FUNCTION_ARGS) 
{
	throw "not_implemented";
}

Scorpion::Class::~Class()
{
	for(std::map<std::string, Object *>::iterator it = this->properties.begin(); it != this->properties.end(); ++it){
		delete it->second;
	}
}