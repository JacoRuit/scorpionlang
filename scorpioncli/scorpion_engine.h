#ifndef SCORPION_ENGINE_H
#define SCORPION_ENGINE_H

#include <vector>
#include <string>
#include <map>
#include "scorpion_parser.h"

#define SCORPION_FUNCTION_ARGS \
		Scorpion::Engine *engine, Scorpion::Scope *scope, Scorpion::Object *me

#define SCORPION_FUNCTION(name) \
		void name(SCORPION_FUNCTION_ARGS)

#define SCORPION_FUNCTION_PTR(name) \
		void (*name) (SCORPION_FUNCTION_ARGS)

#define SCORPION_REF(obj) obj->refcount++

#define SCORPION_O_FLAG_NATIVE		0x01
#define SCORPION_O_FLAG_READONLY	0x02

namespace Scorpion {
	class SCORPIONAPI Object;
	class SCORPIONAPI Engine;
	class SCORPIONAPI Scope;

	class SCORPIONAPI RuntimeException {
	public:
		std::string message;
		RuntimeException(std::string message) : message(message) { }
	};

	class SCORPIONAPI Class {
	public:
		std::map<std::string, Scorpion::Object *> properties;
		virtual void invoke(SCORPION_FUNCTION_ARGS) = 0;
		//SCOPRION_FUNCTION_MPTR(invokem, Class);

		virtual ~Class() = 0;
	};

	class SCORPIONAPI Object {
	public:
		Scorpion::Scope *scope;
		Scorpion::Class *classPtr;
		int flags;
		int refcount;
		Scorpion::Scope *owner;
		//SCOPRION_FUNCTION_MPTR(invokem, Object);

		Object() : owner(NULL), scope(NULL), classPtr(NULL) { }
		Object(Scorpion::Object *obj);
		~Object();
	};

	class SCORPIONAPI GarbageCollector {
	public:
		virtual void collect(Scorpion::Scope *scope);
	};

	class SCORPIONAPI Scope {
		std::map<std::string, Scorpion::Object *> objects;
		std::map<std::string, Scorpion::Class *> classes;
		Scorpion::Scope *parent;
		Scorpion::GarbageCollector *gc;
	public:
		friend GarbageCollector;
		const std::map<std::string, Scorpion::Object *>::iterator end;

		Scope(Scorpion::GarbageCollector *gc);
		Scope(Scorpion::Scope *parent, Scorpion::GarbageCollector *gc) : gc(gc), parent(parent) { }
		~Scope();

		void addObject(const std::string name, Scorpion::Object *object);
		void addClass(const std::string name, Scorpion::Class *sclass);

		Scorpion::Class *getClass(std::string *name);
		Scorpion::Object *getObject(std::string *name);
		Scorpion::Object *cretObject(std::string *name);

		void removeClass(std::string *name, bool cleanup);
		void removeObject(std::string *name, bool cleanup);
		void removeClasses(bool cleanup);

		void ref();
		void deref();
		
		void instantiate(Scorpion::Object *object, Scorpion::Class *sclass);

		Scorpion::Object* createObject(const std::string className);
		Scorpion::Object* createSingleton(Scorpion::Class *sclass);

		void tick();
	};


	class SCORPIONAPI Engine {
	public: 
		Scorpion::Scope *globalScope;
		
		//Engine();
		~Engine();

		void execute(std::vector<Scorpion::Parser::line *> *lines, Scorpion::Scope *scope);

		void callMethod(Scorpion::Object *object, std::string name);
		void callFunction(Scorpion::Object *object);

		void copyObject(Scorpion::Object *target, Scorpion::Object *original);
	};

	
	
};


#endif