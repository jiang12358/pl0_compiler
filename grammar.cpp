
#include "grammar.h"
#include <iostream>
#include "Lex.h"

namespace compiler {
	//extern void lex.getsym();

	void Grammar::init(std::string filename) {
		lex.init(filename);
	}

	void Grammar::program() {
		block();
		
		if (lex.sym == kDot) {
			std::cout << "This is a program" << std::endl;
		}
	}

	void Grammar::block() {
		lex.getsym();
		if (lex.sym == kConst) {
			lex.getsym();
			const_declare();
		}

		while (lex.sym == kComma) {
			lex.getsym();
			if (lex.sym == kIdent) {
				const_declare();
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
			var_declare();
		}

		while (lex.sym == kFunction || lex.sym == kProcedure) {
			if (lex.sym == kFunction) {
				lex.getsym();
				function_declare();
			}

			if (lex.sym == kProcedure) {
				lex.getsym();
				procedure_declare();
			}
		}
		
		if (lex.sym == kBegin) {
			multistatement();
		}
		std::cout << "this is a block" << std::endl;
	}

	void Grammar::const_declare() {
		if (lex.sym == kIdent) {
			lex.getsym();
			if (lex.sym == kEqual) {
				lex.getsym();
				const_();
			}
		}
		std::cout << "This is a const declaration" << std::endl;
	}

	void Grammar::const_() {
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

	void Grammar::var_declare() {
		var_define();
		while (lex.sym == kSemiColon) {
			lex.getsym();
			var_define();
		}
		std::cout << "This is a variable declaration" << std::endl;
	}

	void Grammar::var_define() {
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
					array_declare();
				}
				else {
					error();
				}
			}
			std::cout << "This is a variable defination" << std::endl;
		}
		
	}

	void Grammar::array_declare() {
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

	void Grammar::function_declare() {
		if (lex.sym == kIdent) {
			lex.getsym();
			if (lex.sym == kLBracket) {
				lex.getsym();
				if (lex.sym != kRBracket) {
					format_parameter();
				}
				if (lex.sym == kRBracket) {
					lex.getsym();
					if (lex.sym == kColon) {
						lex.getsym();
						if (lex.sym == kInteger || lex.sym == kChar || lex.sym == kReal) {
							lex.getsym();
							if (lex.sym == kSemiColon) {
								block();
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

	void Grammar::procedure_declare() {
		if (lex.sym == kIdent) {
			lex.getsym();
			if (lex.sym == kLBracket) {
				lex.getsym();
				if (lex.sym != kRBracket) {
					format_parameter();
				}

				//std::cout << lex.sym << std::endl;
				if (lex.sym == kRBracket) {
					lex.getsym();
					if (lex.sym == kSemiColon) {

						//std::cout << lex.sym << std::endl;
						block();
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

	void Grammar::format_parameter() {
		one_parameter();
		while (lex.sym == kSemiColon) {
			lex.getsym();
			one_parameter();
		}
		std::cout << "This is a format parameter" << std::endl;
	}

	void Grammar::one_parameter() {
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

	void Grammar::statement() {
		
		switch (lex.sym) {
		case kBegin: {
			multistatement();
			break;
		}
		case kIf: {
			ifstatement();
			break;
		}
		case kFor: {
			forstatement();
			break;
		}
		case kRepeat: {
			repeatstatement();
			break;
		}
		case kWrite: {
			writestatement();
			break;
		}
		case kRead: {
			readstatement();
			break;
		}
		case kIdent: {
			lex.getsym();
			
			if (lex.sym == kLBracket) {
				lex.getsym();
				if (lex.sym != kRBracket) {
					lex.getsym();
					parameter_handle();
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
				becomestatement();
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

	void Grammar::multistatement() {
		//std::cout << lex.sym << std::endl;
		lex.getsym();
			statement();
			while (lex.sym == kSemiColon) {
				lex.getsym();
				statement();
			}
			if (lex.sym != kEnd) {
				error();
			}
			lex.getsym();
		std::cout << "This is a multistatement" << std::endl;
	}

	void Grammar::ifstatement() {
		lex.getsym();
		condition();
		if (lex.sym == kThen) {
			lex.getsym();
			statement();
			if (lex.sym == kElse) {
				lex.getsym();
				statement();
			}
		}
		std::cout << "This is an if statement" << std::endl;
	}

	void Grammar::forstatement() {
		lex.getsym();
		if (lex.sym == kIdent) {
			lex.getsym();
			if (lex.sym == kBecome) {
				lex.getsym();
			}
			else {
				error();
			}
			
			expression();
			if (lex.sym == kTo || lex.sym == kDownto) {
				lex.getsym();
				expression();
				
				if (lex.sym == kDo) {
					lex.getsym();
					statement();
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

	void Grammar::repeatstatement() {
		lex.getsym();
		statement();
		if (lex.sym == kUntil) {
			lex.getsym();
			condition();
		}
		else {
			error();
		}
		std::cout << "This is a repeat statement" << std::endl;
	}

	void Grammar::writestatement() {
		lex.getsym();
		if (lex.sym == kLBracket) {
			lex.getsym();
			if (lex.sym == kString) {
				lex.getsym();
				if (lex.sym == kComma) {
					lex.getsym();
					expression();
				}
			}
			else {
				expression();
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

	void Grammar::readstatement() {
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

	void Grammar::becomestatement() {
			if (lex.sym == kLSquareBracket) {
				lex.getsym();
				expression();
				if (lex.sym != kRSquareBracket) {
					error();
				}
				else {
					lex.getsym();
				}
			}

			if (lex.sym == kBecome) {
				lex.getsym();
				expression();
			}
		
		std::cout << "This is a become statement" << std::endl;
	}

	void Grammar::expression() {
		if (lex.sym == kPlus || lex.sym == kMinus) {
			lex.getsym();
		}
		item();
		while (lex.sym == kPlus || lex.sym == kMinus) {
			lex.getsym();
			item();
		}
		std::cout << "This is an expression" << std::endl;
	}

	void Grammar::item() {
		factor();
		while (lex.sym == kMulti || lex.sym == kDevide) {
			lex.getsym();
			factor();
		}
		std::cout << "This is an item" << std::endl;
	}

	void Grammar::factor() {
		if (lex.sym == kIdent) {
			lex.getsym();
			if (lex.sym == kLSquareBracket) {
				lex.getsym();
				expression();
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
					parameter_handle();
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
			expression();
			if (lex.sym != kRBracket) {
				error();
			}
			else {
				lex.getsym();
			}
		}
		std::cout << "This is a factor" << std::endl;
	}

	void Grammar::parameter_handle() {
		if (lex.sym != kRBracket) {
			expression();
			while (lex.sym == kComma) {
				lex.getsym();
				expression();
			}
		}
		std::cout << "This is a parameter expression" << std::endl;
	}

	void Grammar::condition() {
		expression();
		if (lex.sym == kEqual || lex.sym == kLess || lex.sym == kLessEqual || lex.sym == kNotEqual ||
			lex.sym == kGreater || lex.sym == kGreaterEqual) {
			lex.getsym();
			expression();
			std::cout << "This a condition" << std::endl;
		}
	}

}  // namespace compiler

int main(int argc, char *argv[]) {
	compiler::Grammar grammar;
	grammar.init(std::string(argv[1]));
	grammar.program();
}