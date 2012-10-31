#ifndef SCORPION_FUNCTION_TOKEN_H
#define SCORPION_FUNCTION_TOKEN

#include "scorpion_parser.h"



namespace Scorpion {
	namespace Parser {

		namespace TokenProviders {
			class FunctionCallTokenProvider : public TokenProvider {
			public:
				int tokenize(std::string *line_part, std::vector<line *>::iterator *line);
			};
		}

		namespace TokenHandlers {
		}
	}
}

#endif