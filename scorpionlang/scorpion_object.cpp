#define SCORPION_BUILD 1
#include "scorpion_engine.h"

#include <iostream>
static int init = 0;

Scorpion::Object::Object() : owner(NULL), scope(NULL), classPtr(NULL), flags(0)
{
	std::cout << "Object object created " << ++init << std::endl;
}

Scorpion::Object::Object(Object *obj) : flags(0), owner(NULL)
{
	this->classPtr = obj->classPtr;

	//Object Scope never tick()'ed
	this->scope = new Scope(obj->scope, new GarbageCollector());
	std::cout << "Object object created " << ++init << std::endl;
}

Scorpion::Object::~Object()
{
	if(this->scope != NULL) {
		this->scope->deref();
		delete this->scope;
	}
	if(SCORPION_IS_STRING(this))
		delete this->rawValue.str;
	std::cout << "Object object deleted " << init-- << std::endl;
}