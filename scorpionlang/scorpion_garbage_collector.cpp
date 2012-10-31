#define SCORPION_BUILD 1
#include "scorpion_engine.h"

void Scorpion::GarbageCollector::collect(Scope *scope)
{
	for(std::map<std::string, Object *>::iterator it = scope->objects.begin(); it != scope->objects.end(); ++it){
		if(it->second->refcount < 1 && it->second->owner == scope){
			delete it->second;
			scope->objects.erase(it);
		}
	}
}