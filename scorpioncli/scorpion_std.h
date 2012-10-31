#ifndef SCORPION_STD_H
#define SCORPION_STD_H
#include "scorpion_classes.h"

#define SCORPION_STD_FUNCTION_NAME(name)	__scorpion_std_##name
#define SCORPION_STD_FUNCTION				SCORPION_STD_FUNCTION_NAME(name)(SCORPION_FUNCTION_ARGS)
#define SSTDF(name)							SCORPION_STD_FUNCTION(name)
#define SSTDFN(name)						SCORPION_STD_FUNCTION_NAME(name)

namespace Scorpion {
	namespace Std {
		static Scorpion::NativeFunction *print_function;

		void SCORPIONAPI link(Scorpion::Scope *scope);
		void SCORPIONAPI cleanup();

		void create_class_string(Scorpion::Scope *scope);
		void create_class_integer(Scorpion::Scope *scope);

		void SCORPIONAPI __scorpion_std_print(SCORPION_FUNCTION_ARGS); // SSTDF(print);
		void create_function_print(Scorpion::Scope *scope);
	};
};

#endif