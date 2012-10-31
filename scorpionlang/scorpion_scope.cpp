#define SCORPION_BUILD 1
#include "scorpion_engine.h"

#include <iostream>
static int init = 0;

Scorpion::Scope::Scope(GarbageCollector *gc) : gc(gc)
{
	this->parent = NULL;
	std::cout << "Scope object created " << ++init << std::endl;
}

Scorpion::Scope::~Scope()
{
	this->removeClasses(true);
	delete this->gc;
	std::cout << "Scope object deleted " << init-- << std::endl;
}

void Scorpion::Scope::addObject(const std::string name, Object *object)
{
	object->owner = this;
	object->refcount = 1;
	this->objects.insert(std::pair<std::string, Object *>(name, object));
}

void Scorpion::Scope::addClass(const std::string name, Class *sclass)
{
	this->classes.insert(std::pair<std::string, Class *>(name, sclass));
}

Scorpion::Object* Scorpion::Scope::getObject(std::string *name)
{
	std::map<std::string, Object *>::iterator it = this->objects.find(*name);
	if(it == this->objects.end()){
		if(this->parent != NULL)
			return this->parent->getObject(name);
		throw RuntimeException("Object \"" + *name + "\" not in scope");
	}
	return it->second;
}

Scorpion::Object* Scorpion::Scope::cretObject(std::string *name)
{
	std::map<std::string, Object *>::iterator it = this->objects.find(*name);
	if(it == this->objects.end()){
		Object *retval = new Object;
		this->addObject(*name, retval);
		return retval;
	}
	return it->second;
}

Scorpion::Class* Scorpion::Scope::getClass(std::string *name)
{
	std::map<std::string, Class *>::iterator it = this->classes.find(*name);
	if(it == this->classes.end()){
		if(this->parent != NULL)
			return this->parent->getClass(name);
		throw RuntimeException("Class \"" + *name + "\" not in scope");
	}
	return it->second;
}

void Scorpion::Scope::removeClass(std::string *name, bool cleanup)
{
	std::map<std::string, Class *>::iterator it = this->classes.find(*name);
	if(it == this->classes.end()){
		if(this->parent != NULL)
			return this->parent->removeClass(name, cleanup);
		throw RuntimeException("Class \"" + *name + "\" not in scope");
	}
	delete it->second;
	this->classes.erase(it);
}

void Scorpion::Scope::removeObject(std::string *name, bool cleanup)
{
	std::map<std::string, Object *>::iterator it = this->objects.find(*name);
	if(it == this->objects.end()){
		if(this->parent != NULL)
			return this->parent->removeObject(name, cleanup);
		throw RuntimeException("Object \"" + *name + "\" not in scope");
	}
	delete it->second;
	this->objects.erase(it);
}

void Scorpion::Scope::removeClasses(bool cleanup)
{
	if(cleanup) {
		for(std::map<std::string, Class *>::iterator it = this->classes.begin(); it != this->classes.end(); ++it){
			delete it->second;
		}
	}
	this->classes.clear();
}

void Scorpion::Scope::ref()
{
	for(std::map<std::string, Object *>::iterator it = this->objects.begin(); it != this->objects.end(); ++it){
		SCORPION_REF(it->second);
	}
}

void Scorpion::Scope::deref()
{
	for(std::map<std::string, Object *>::iterator it = this->objects.begin(); it != this->objects.end(); ++it){
		if(--(it->second->refcount) < 1 && it->second->owner == this){
			delete it->second;			
		}
	}
	this->tick();
}

void Scorpion::Scope::instantiate(Object *object, Class *sclass)
{
	object->classPtr = sclass;
	if(object->scope != NULL){
		object->scope->deref();
		delete object->scope;
	}
	object->scope = new Scope(new GarbageCollector());
	for(std::map<std::string, Object *>::iterator it = sclass->properties.begin(); it != sclass->properties.end(); ++it){
		object->scope->addObject(it->first, new Object(it->second));
	}
}

Scorpion::Object *Scorpion::Scope::createSingleton(Class *sclass)
{
	Object *retval = new Object;
	this->instantiate(retval, sclass);
	return retval;
}

Scorpion::Object *Scorpion::Scope::createObject(const std::string className)
{
	Object *retval = new Object;
	std::map<std::string, Class *>::iterator cit =  this->classes.find(className); //TODO check iterator != npos
	this->instantiate(retval, cit->second);
	return retval;
}

void Scorpion::Scope::tick(){
	this->gc->collect(this);
	if(this->parent != NULL)
		this->parent->tick();
}

void Scorpion::Scope::copy(Scope *target)
{
	this->ref();
	target->objects = this->objects;
}