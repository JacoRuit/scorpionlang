#define SCORPION_BUILD 1
#include "scorpion_classes.h"


Scorpion::NativeFunction::NativeFunction(SCORPION_FUNCTION_PTR(invoke))
{
	this->invokePtr = invoke;
}

void Scorpion::NativeFunction::invoke(SCORPION_FUNCTION_ARGS)
{
	this->invokePtr(engine, scope, me);
}
Scorpion::ScorpionFunction::ScorpionFunction(std::vector<Scorpion::Parser::line *> *lines)
{
	this->lines = lines;
}

SCORPION_FUNCTION(Scorpion::ScorpionFunction::invoke)
{
	engine->execute(this->lines, new Scope(new GarbageCollector()));
}

Scorpion::ScorpionFunction::~ScorpionFunction()
{
	for(std::vector<Scorpion::Parser::line *>::iterator it = this->lines->begin(); it != this->lines->end(); ++it){
		delete *it;
	}
}
