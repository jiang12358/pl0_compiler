
#include "grammar.h"
#include <iostream>
#include "Lex.h"

namespace compiler {
	//extern void lex.getsym();

	void Grammar::init(std::string filename) {
		lex.init(filename);
	}

	void Grammar::program() {
		block(0);
		
		if (lex.sym == kDot) {
			std::cout << "This is a program" << std::endl;
		}
	}

	void Grammar::block(int level) {
		lex.getsym();
		if (lex.sym == kConst) {
			lex.getsym();
			const_declare(level);
		}

		while (lex.sym == kComma) {
			lex.getsym();
			if (lex.sym == kIdent) {
				const_declare(level);
			}
			else {
				break;
			}
		}

		if (lex.sym == kSemiColon) {
			lex.getsym();
		} 

		if (lex.sym == kVar) {
			lex.getsym();
			var_declare(level);
		}

		while (lex.sym == kFunction || lex.sym == kProcedure) {
			if (lex.sym == kFunction) {
				lex.getsym();
				function_declare(level);
			}

			if (lex.sym == kProcedure) {
				lex.getsym();
				procedure_declare(level);
			}
		}
		
		if (lex.sym == kBegin) {
			multistatement(level);
		}
		std::cout << "this is a block" << std::endl;
	}

	void Grammar::const_declare(int level) {
		if (lex.sym == kIdent) {
			lex.getsym();
			if (lex.sym == kEqual) {
				lex.getsym();
				const_(level);
				enter(level, lex.id, 0, 0, kConst);
			}
		}
		std::cout << "This is a const declaration" << std::endl;
	}

	void Grammar::const_(int level) {
		if (lex.sym == kPlus || lex.sym == kMinus) {
			lex.getsym();
			if (lex.sym == kRealNum || lex.sym == kNumber) {
				lex.getsym();
				std::cout << "This is a const" << std::endl;
			}
		}
		else if (lex.sym == kRealNum || lex.sym == kNumber) {
			lex.getsym();
			std::cout << "This is a const" << std::endl;
		}
		else if (lex.sym == kCharacter) {
			lex.getsym();
			std::cout << "This is a const" << std::endl;
		}
	}

	void Grammar::var_declare(int level) {
		var_define(level);
		while (lex.sym == kSemiColon) {
			lex.getsym();
			var_define(level);
		}
		std::cout << "This is a variable declaration" << std::endl;
	}

	void Grammar::var_define(int level) {
		if (lex.sym == kIdent) {
			lex.getsym();
			while (lex.sym == kComma) {
				lex.getsym();
				if (lex.sym == kIdent) {
					lex.getsym();
				}
			}

			if (lex.sym == kColon) {
				lex.getsym();
				if (lex.sym == kInteger || lex.sym == kReal || lex.sym == kChar) {
					lex.getsym();
				}
				else if (lex.sym == kArray) {
					lex.getsym();
					array_declare(level);
				}
				else {
					error();
				}
			}
			std::cout << "This is a variable defination" << std::endl;
		}
		
	}

	void Grammar::array_declare(int level) {
		if (lex.sym == kLSquareBracket) {
			lex.getsym();
			if (lex.sym == kNumber) {
				lex.getsym();
				if (lex.sym == kRSquareBracket) {
					lex.getsym();
					if (lex.sym == kOf) {
						lex.getsym();
						if (lex.sym == kInteger || lex.sym == kReal || lex.sym == kChar) {
							lex.getsym();
							std::cout << "This is an array defination" << std::endl;
						}
					}
				}
			}
		}
	}

	void Grammar::function_declare(int level) {
		if (lex.sym == kIdent) {
			lex.getsym();
			if (lex.sym == kLBracket) {
				lex.getsym();
				if (lex.sym != kRBracket) {
					format_parameter(level);
				}
				if (lex.sym == kRBracket) {
					lex.getsym();
					if (lex.sym == kColon) {
						lex.getsym();
						if (lex.sym == kInteger || lex.sym == kChar || lex.sym == kReal) {
							lex.getsym();
							if (lex.sym == kSemiColon) {
								block(level);
								if (lex.sym == kSemiColon) {
									lex.getsym();
									std::cout << "This is a function declaration" << std::endl;
								}
							}
						}
					}
				}
			}
		}
		
	}

	void Grammar::procedure_declare(int level) {
		if (lex.sym == kIdent) {
			lex.getsym();
			if (lex.sym == kLBracket) {
				lex.getsym();
				if (lex.sym != kRBracket) {
					format_parameter(level);
				}

				//std::cout << lex.sym << std::endl;
				if (lex.sym == kRBracket) {
					lex.getsym();
					if (lex.sym == kSemiColon) {

						//std::cout << lex.sym << std::endl;
						block(level);
						//std::cout << lex.sym << std::endl;
						if (lex.sym == kSemiColon) {
							lex.getsym();
							std::cout << "This is a procedure declaration" << std::endl;
						}
					}
				}
			}
		}
		
	}

	void Grammar::format_parameter(int level) {
		one_parameter(level);
		while (lex.sym == kSemiColon) {
			lex.getsym();
			one_parameter(level);
		}
		std::cout << "This is a format parameter" << std::endl;
	}

	void Grammar::one_parameter(int level) {
		if (lex.sym == kVar) {
			lex.getsym();
		}
		if (lex.sym == kIdent) {
			lex.getsym();
			while (lex.sym == kComma) {
				lex.getsym();
				if (lex.sym == kIdent) {
					lex.getsym();
				}
				else {
					error();
				}
			}
			if (lex.sym == kColon) {
				lex.getsym();
				if (lex.sym == kInteger || lex.sym == kReal || lex.sym == kChar) {
					lex.getsym();
					std::cout << "This is a parameter" << std::endl;
				}
			}
		}
	}

	void Grammar::statement(int level) {
		
		switch (lex.sym) {
		case kBegin: {
			multistatement(level);
			break;
		}
		case kIf: {
			ifstatement(level);
			break;
		}
		case kFor: {
			forstatement(level);
			break;
		}
		case kRepeat: {
			repeatstatement(level);
			break;
		}
		case kWrite: {
			writestatement(level);
			break;
		}
		case kRead: {
			readstatement(level);
			break;
		}
		case kIdent: {
			lex.getsym();
			
			if (lex.sym == kLBracket) {
				lex.getsym();
				if (lex.sym != kRBracket) {
					lex.getsym();
					parameter_handle(level);
				}
				if (lex.sym != kRBracket) {
					error();
				}
				else {
					lex.getsym();
					std::cout << "This is a procedure call" << std::endl;
				}
			}
			else {
				becomestatement(level);
			}
			break;
		}
		case kEnd:
			break;
		default:
			break;
		}
		//std::cout << lex.sym << std::endl;
		std::cout << "this is a statement" << std::endl;
	}

	void Grammar::multistatement(int level) {
		//std::cout << lex.sym << std::endl;
		lex.getsym();
			statement(level);
			while (lex.sym == kSemiColon) {
				lex.getsym();
				statement(level);
			}
			if (lex.sym != kEnd) {
				error();
			}
			lex.getsym();
		std::cout << "This is a multistatement" << std::endl;
	}

	void Grammar::ifstatement(int level) {
		lex.getsym();
		condition(level);
		if (lex.sym == kThen) {
			lex.getsym();
			statement(level);
			if (lex.sym == kElse) {
				lex.getsym();
				statement(level);
			}
		}
		std::cout << "This is an if statement" << std::endl;
	}

	void Grammar::forstatement(int level) {
		lex.getsym();
		if (lex.sym == kIdent) {
			lex.getsym();
			if (lex.sym == kBecome) {
				lex.getsym();
			}
			else {
				error();
			}
			
			expression(level);
			if (lex.sym == kTo || lex.sym == kDownto) {
				lex.getsym();
				expression(level);
				
				if (lex.sym == kDo) {
					lex.getsym();
					statement(level);
				}
				else {
					error();
				}
			}
			else {
				error();
			}
		}
		std::cout << "This is a for statement" << std::endl;
	}

	void Grammar::repeatstatement(int level) {
		lex.getsym();
		statement(level);
		if (lex.sym == kUntil) {
			lex.getsym();
			condition(level);
		}
		else {
			error();
		}
		std::cout << "This is a repeat statement" << std::endl;
	}

	void Grammar::writestatement(int level) {
		lex.getsym();
		if (lex.sym == kLBracket) {
			lex.getsym();
			if (lex.sym == kString) {
				lex.getsym();
				if (lex.sym == kComma) {
					lex.getsym();
					expression(level);
				}
			}
			else {
				expression(level);
			}
			if (lex.sym != kRBracket) {
				error();
			}
			else {
				lex.getsym();
			}
		}
		std::cout << "This is a writestatement" << std::endl;
	}

	void Grammar::readstatement(int level) {
		lex.getsym();
		if (lex.sym == kLBracket) {
			lex.getsym();
			if (lex.sym == kIdent) {
				lex.getsym();
				while (lex.sym == kComma) {
					lex.getsym();
					if (lex.sym != kIdent) {
						error();
					}
					lex.getsym();
				}

				if (lex.sym != kRBracket) {
					error();
				}
				else {
					lex.getsym();
				}
			}
			else {
				error();
			}
		}
		std::cout << "This is a readstatement" << std::endl;
	}

	void Grammar::becomestatement(int level) {
			if (lex.sym == kLSquareBracket) {
				lex.getsym();
				expression(level);
				if (lex.sym != kRSquareBracket) {
					error();
				}
				else {
					lex.getsym();
				}
			}

			if (lex.sym == kBecome) {
				lex.getsym();
				expression(level);
			}
		
		std::cout << "This is a become statement" << std::endl;
	}

	void Grammar::expression(int level) {
		if (lex.sym == kPlus || lex.sym == kMinus) {
			lex.getsym();
		}
		item(level);
		while (lex.sym == kPlus || lex.sym == kMinus) {
			lex.getsym();
			item(level);
		}
		std::cout << "This is an expression" << std::endl;
	}

	void Grammar::item(int level) {
		factor(level);
		while (lex.sym == kMulti || lex.sym == kDevide) {
			lex.getsym();
			factor(level);
		}
		std::cout << "This is an item" << std::endl;
	}

	void Grammar::factor(int level) {
		if (lex.sym == kIdent) {
			lex.getsym();
			if (lex.sym == kLSquareBracket) {
				lex.getsym();
				expression(level);
				if (lex.sym != kRSquareBracket) {
					error();
				}
				else {
					lex.getsym();
				}
			}
			else if (lex.sym == kLBracket) {
				lex.getsym();
				if (lex.sym != kRBracket) {
					parameter_handle(level);
				}
				if (lex.sym != kRBracket) {
					error();
				}
				else {
					lex.getsym();
					std::cout << "This is a function call" << std::endl;
				}
			}
		}
		else if (lex.sym == kNumber) {
			lex.getsym();
			std::cout << "This is a number" << std::endl;
		}
		else if (lex.sym == kReal) {
			lex.getsym();
			std::cout << "This is a real" << std::endl;
		}
		else if (lex.sym == kLBracket) {
			lex.getsym();
			expression(level);
			if (lex.sym != kRBracket) {
				error();
			}
			else {
				lex.getsym();
			}
		}
		std::cout << "This is a factor" << std::endl;
	}

	void Grammar::parameter_handle(int level) {
		if (lex.sym != kRBracket) {
			expression(level);
			while (lex.sym == kComma) {
				lex.getsym();
				expression(level);
			}
		}
		std::cout << "This is a parameter expression" << std::endl;
	}

	void Grammar::condition(int level) {
		expression(level);
		if (lex.sym == kEqual || lex.sym == kLess || lex.sym == kLessEqual || lex.sym == kNotEqual ||
			lex.sym == kGreater || lex.sym == kGreaterEqual) {
			lex.getsym();
			expression(level);
			std::cout << "This a condition" << std::endl;
		}
	}

	void Grammar::enter(int level, std::string name, int pdx, int ptx, objectType kind) {
		table temp = table();
		temp.name = name;
		temp.level = level;
		temp.kind = kind;
		tables.push_back(temp);
		


	}

}  // namespace compiler

int main(int argc, char *argv[]) {
	compiler::Grammar grammar;
	grammar.init(std::string(argv[1]));
	grammar.program();
}