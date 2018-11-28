#ifndef COMPILER_LEX
#define COMPILER_LEX
#include <iostream>
#include "Scanner.h"
#include "global.h"
namespace compiler {
	class Lex {
	public:
		Lex() {};

		void init(std::string filename);

		SymType getsym();

		void Run();

		SymType sym;

		std::string id;
	private:
		Scanner scanner;
		char c;

		
	};

}
#endif