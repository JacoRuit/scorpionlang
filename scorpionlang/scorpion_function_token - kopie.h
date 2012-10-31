#ifndef SCORPION_FUNCTION_TOKEN_H
#define SCORPION_FUNCTION_TOKEN

#include "scorpion_parser.h"

#define SCORPION_T_FUNCTION 1
#define SCORPION_CT_FUNCTION_NAME 1

namespace Scorpion {
	namespace Parser {

		namespace TokenProviders {
			class FunctionTokenProvider : public TokenProvider {
				int tokenize(std::string *line_part, std::vector<line *>::iterator *line);
			};
		}

		namespace TokenHandlers {
		}
	}
}

#endif