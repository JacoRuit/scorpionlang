#define SCORPION_BUILD 1
#include "scorpion_std.h"
#include <iostream>

void Scorpion::Std::link(Scorpion::Scope *scope) 
{
	create_class_string(scope);
	create_class_integer(scope);
	create_function_print(scope);
}

void Scorpion::Std::cleanup() 
{
	delete print_function;
}

void Scorpion::Std::create_class_string(Scorpion::Scope *scope)
{
	Class *strClass = new Class;
	scope->addClass("String", strClass);
}

void Scorpion::Std::create_class_integer(Scorpion::Scope *scope)
{
	Class *intClass = new Class;
	scope->addClass("Integer", intClass);
}

void Scorpion::Std::__scorpion_std_print(SCORPION_FUNCTION_ARGS)
{
	std::cout << "Hey! You called print! :-)" << std::endl;
}

void Scorpion::Std::create_function_print(Scorpion::Scope *scope)
{
	print_function = new Scorpion::NativeFunction(&(__scorpion_std_print));
	scope->addObject("print", scope->createSingleton(print_function));
}
