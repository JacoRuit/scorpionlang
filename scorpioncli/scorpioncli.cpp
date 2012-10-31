// scorpioncli.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include "scorpion_parser.h"
#include "scorpion_engine.h"
#include "scorpion_classes.h"
#include "scorpion_std.h"

using namespace Scorpion::Parser;

SCORPION_FUNCTION(helloFunc){
	std::cout << "HEYAEYEYEYAEYEYEYEYE and i cry sometimes when i'm lying in bed" << std::endl;
}

class TestProvider : public TokenProvider {
	int tokenize(std::string *line_part, std::vector<line *>::iterator *line) { 
		if(line_part->find("test") == 0){ 
			std::cout << "test:D";
			return 4;
			
		}
		return 0;
	}
};

int main(int argc, char* argv[])
{
	
	std::cout << "Scorpion";
	std::cin.get();
	Tokenizer t;
	std::string code = "sdf=\"Sdf string\"\nsdfCopy = sdf\nhello[sdf,sdf,\"sdf is een var\" ]\nprint[]\n##comment";
	Scorpion::Scope *sc = new Scorpion::Scope(new Scorpion::GarbageCollector());
	Scorpion::Std::link(sc);
	try 
	{
		std::vector<Scorpion::Parser::line *>tokens  = t.tokenize(&code);
		std::cout << "tokens length " << sizeof(tokens) << std::endl;
		std::cout << "execute! :)" << std::endl;
		Scorpion::Engine ngin;
		
		Scorpion::Class *helloFuncClass = new Scorpion::NativeFunction(&helloFunc);
		sc->addObject("hello", sc->createSingleton(helloFuncClass));
		
		ngin.execute(&tokens, sc);
		for(std::vector<Scorpion::Parser::line *>::iterator it = tokens.begin(); it != tokens.end(); ++it){
			for(std::vector<Scorpion::Parser::Token *>::iterator it2 = (*it)->tokens.begin(); it2 != (*it)->tokens.end(); ++it2){
				for(std::vector<Scorpion::Parser::Token *>::iterator intit = (*it2)->inner_tokens.begin(); intit != (*it2)->inner_tokens.end(); ++intit){
					for(std::vector<Scorpion::Parser::constant_token *>::iterator cit = (*intit)->constants.begin(); cit != (*intit)->constants.end(); ++cit){
						std::cout << (*cit)->type << ":" << (*cit)->svalue;
					}
				}
				delete *it2;
			}
			delete *it;
		}
		sc->deref();
		delete sc;
	}catch (ParseException &e) {
		std::cout << e.what() << std::endl;
		std::cout << e.message << std::endl;
		sc->deref();
		delete sc;
	}catch (Scorpion::RuntimeException &ex){
		std::cout << ex.message << std::endl;
		sc->deref();
		delete sc;
	}
	Scorpion::Std::cleanup();
	std::cin.get();
	return 0;
}