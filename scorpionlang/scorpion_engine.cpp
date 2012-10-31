#define SCORPION_BUILD 1
#include "scorpion_engine.h"
#include "scorpion_std.h"
#include <iostream>

void Scorpion::Engine::execute(std::vector<Scorpion::Parser::line *> *lines, Scope *scope = new Scope(new GarbageCollector()))
{
	std::vector<Scorpion::Parser::Token *>::iterator it = lines->front()->tokens.begin();
	std::vector<Scorpion::Parser::line *>::iterator lit = lines->begin();
	while(it != (*lit)->tokens.end())
	{
		if((*it)->type == SCORPION_T_FUNCTION_CALL){
			Scope *callStack = new Scope(new GarbageCollector());
			std::cout << "JACALL";
			if(!(*it)->hasConstant(SCORPION_CT_FUNCTION_NAME))
				throw "false";
			Object *function = scope->getObject(&((*it)->getConstant(SCORPION_CT_FUNCTION_NAME)->svalue));
			if(function != NULL)
				this->callFunction(function);	
			callStack->deref();
			delete callStack;
		}else if((*it)->type == SCORPION_T_ASSIGN){
			std::string lhsName = (*it)->getToken(SCORPION_T_LHS)->getConstant(SCORPION_CT_VARIABLE)->svalue;
			Object *lhs = scope->cretObject(&lhsName); //TODO IF NOT EXIST: INSTANTIATE
			//ELSE REINSANTIATE :D
			Scorpion::Parser::Token *rhsToken = (*it)->getToken(SCORPION_T_RHS);
			if(rhsToken->hasConstant(SCORPION_CT_VARIABLE)){
				Object *rhs = scope->getObject(&(rhsToken->getConstant(SCORPION_CT_VARIABLE)->svalue));
				this->copyObject(lhs, rhs);
			}else if(rhsToken->hasConstant(SCORPION_CT_STRING_CONSTANT)){
				scope->instantiate(lhs, scope->getClass(&(std::string("String"))));
				lhs->flags = SCORPION_O_FLAG_NATIVE | SCORPION_O_FLAG_STRING;
				lhs->rawValue.str = new std::string(rhsToken->getConstant(SCORPION_CT_STRING_CONSTANT)->svalue);
			}else if(rhsToken->hasConstant(SCORPION_CT_INT_CONSTANT)){
				scope->instantiate(lhs, scope->getClass(&std::string("Integer")));
				lhs->flags = SCORPION_O_FLAG_NATIVE | SCORPION_O_FLAG_STRING;
			}
		}else{
			std::cout << "Unknown token with type: " << (*it)->type;
		}
		if(++it == (*lit)->tokens.end())
		{
			if(++lit == lines->end()){
				return;
			}else{
				it = (*lit)->tokens.begin();
			}
		}
	}
}

void Scorpion::Engine::callFunction(Object *object)
{
	object->classPtr->invoke(0, 0, object);
}

void Scorpion::Engine::copyObject(Object *target, Object *original)
{
	if(target->scope != NULL){
		target->scope->deref();
		delete target->scope;
	}
	target->scope = new Scope(new GarbageCollector());
	original->scope->copy(target->scope);
	target->classPtr = original->classPtr;
	target->flags = original->flags;
	target->rawValue = original->rawValue;
	if(SCORPION_IS_STRING(target))
		target->rawValue.str = new std::string(*original->rawValue.str);
}

Scorpion::Engine::~Engine()
{
	//this->globalScope->removeAll(true);
}

