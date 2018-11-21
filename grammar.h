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

		void block();
		void const_declare();
		void var_declare();
		void var_define();

		void procedure_declare();
		void function_declare();

		void parameter_handle();

		void format_parameter();

		void one_parameter();

		void array_declare();

		void statement();
		void multistatement();
		void ifstatement();
		void forstatement();
		void repeatstatement();
		void writestatement();
		void readstatement();
		void becomestatement();


		void const_();

		void condition();
		void expression();
		void item();
		void factor();

	private:
		Lex lex;
	};
}  // namespace compiler

#endif