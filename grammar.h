#ifndef GRAMMAR
#define GRAMMAR
#include "global.h"
#include "Lex.h"

namespace compiler {
	class Grammar {

	
	public:
		Grammar() {};

		void init(std::string filename);


		void program();

		void block(int level);
		void const_declare(int level);
		void var_declare(int level);
		void var_define(int level);

		void procedure_declare(int level);
		void function_declare(int level);

		void parameter_handle(int level);

		void format_parameter(int level);

		void one_parameter(int level);

		void array_declare(int level);

		void statement(int level);
		void multistatement(int level);
		void ifstatement(int level);
		void forstatement(int level);
		void repeatstatement(int level);
		void writestatement(int level);
		void readstatement(int level);
		void becomestatement(int level);


		void const_(int level);

		void condition(int level);
		void expression(int level);
		void item(int level);
		void factor(int level);

		void enter(int level, std::string name, int pdx, int ptx, objectType kind);

	private:
		Lex lex;
		std::vector<table> tables;
	public:
		
	};
}  // namespace compiler

#endif