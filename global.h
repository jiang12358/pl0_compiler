#ifndef GLOBAL
#define GLOBAL
#include <iostream>
#include <map>
#include <string>
#define NMAX 10
namespace compiler {

	enum SymType {
		kNull = 0,
		kNumber = 1,
		kBecome = 2,
		kLessEqual = 3,
		kNotEqual = 4,
		kLess = 5,
		kGreaterEqual = 6,
		kGreater = 7,
		kComma = 8,
		kIdent = 9,
		kSemiColon = 10,
		kVar = 11,
		kBegin = 12,
		kEnd = 13,
		kPlus = 14,
		kMinus = 15,
		kConst = 16,
		kRepeat = 17,
		kRead = 18,
		kWrite = 19,
		kTo = 20,
		kIf = 21,
		kThen = 22,
		kDo = 23,
		kProcedure = 24,
		kMulti = 25,
		kDevide = 26,
		kLBracket = 27,
		kRBracket = 28,
		kEqual = 29,
		kDot = 30,
		kFunction = 31,
		kDownto = 32,
		kUntil = 33,
		kArray = 34,
		kOf = 35,
		kInteger = 36,
		kChar = 37,
		kReal = 38,
		kColon = 39,
		kLSquareBracket = 40,
		kRSquareBracket = 41,
		kSingleQuote = 42,
		kDoubleQuote = 43,
		kElse = 44,
		kFor = 45,
		kString = 46,
		kCharacter = 47,
		kRealNum = 48
	};



	static std::map<SymType, std::string> SymbolDict{ {kNull, "NULL"},
											  {kNumber, "NUMBER"},
											  {kBecome, "BECOME"},
											  {kLessEqual, "LessEqual"},
											  {kNotEqual, "NotEqual"},
											  {kLess, "Less"},
											  {kGreaterEqual, "GreaterEqual"},
											  {kGreater, "Greater"},
											  {kComma, "Comma"},
											  {kIdent, "Iden"},
											  {kSemiColon, "SemiColon"},
											  {kVar, "Var"},
											  {kBegin, "Begin"},
											  {kEnd, "End"},
											  {kPlus, "Plus"},
											  {kMinus, "Minus"},
											  {kConst, "Const"},
											  {kRead, "Read"},
											  {kRepeat, "Repeat"},
											  {kTo, "to"},
											  {kIf, "If"},
											  {kThen, "Then"},
											  {kDo, "Do"},
											  {kProcedure, "Procedure"},
											  {kMulti, "Multi"},
											  {kDevide, "Devide"},
											  {kLBracket, "LBracket"},
											  {kRBracket, "RBracket"},
											  {kEqual, "Equal"},
											  {kElse, "Else"},
											  {kWrite, "Write"},
											  {kDot, "Dot"},
											  {kColon, "Colon"},
											  {kSingleQuote, "SingleQuote"},
											  {kDoubleQuote, "DoubleQuote"},
											  {kFunction, "Function"},
											  {kUntil, "Until"},
											  {kDownto, "Downto"},
											  {kArray, "Array"},
											  {kOf, "Of"},
											  {kInteger, "Integer"},
											  {kChar, "Char"},
											  {kReal, "Real"},
											  {kFor, "For"},
											  {kString, "string"},
											  {kCharacter, "character"},
											  {kLSquareBracket, "LSquareBracket"},
											  {kRSquareBracket, "RSquareBracket"} };

	static std::map<char, SymType> CharSymbol{
		{',', kComma},         {';', kSemiColon},
		{'+', kPlus},          {'-', kMinus},
		{'.', kDot},           {':', kColon},
		{'"', kDoubleQuote},   {'\'', kSingleQuote},
		{'*', kMulti},         {'/', kDevide},
		{'(', kLBracket},      {')', kRBracket},
		{'=', kEqual},         {'[', kLSquareBracket},
		{']', kRSquareBracket} };

	static std::map<int, std::string> CodeSymbol{ {0, "LDA"},{1,"LOD"}, {2, "LDI"}, {3, "DIS"}, {8, "FCT"}, {9, "INT"}, {10, "JMP"}, {11, "JPC"}, {12, "SWT"}, {13, "CAS"}, {14, "F1U"}, {15, "F2U"},{16, "F1D"}, {17, "F2D"}, {18, "MKS"}, {19, "CAL"}, {20, "IDX"}, {21, "IXX"}, {22, "LDB"}, {23, "CPB"}, {24, "LDC"}, {25, "LDR"}, {26, "FLT"}, {27, "RED"}, {28, "WRS"}, {29, "WRW"}, {30, "WRU"}, {31, "HLT"}, {32, "EXP"}, {33, "EXF"}, {34, "LDT"}, {35, "NOT"}, {36, "MUS"}, {37, "WRR"}, {38,"STO"}, {39, "EQR"}, {40, "NER"}, {41, "LSR"}, {42,"LER"}, {43, "GTR"}, {44, "GER"}, {45, "EQL"}, {46, "NEQ"}, {47, "LSS"}, {48, "LER"}, {49, "GRT"}, {50, "GEQ"}, {51, "ORR"}, {52, "ADD"}, {53, "SUB"}, {54, "ADR"}, {55, "SUR"}, {56, "AND"}, {57, "MUL"}, {58, "DIV"}, {59, "MOD"}, {60, "MUR"}, {61, "DIR"}, {62, "RDL"}, {63, "WRL"} }
	;
	static std::map<std::string, SymType> WordSymbol{ {"const", kConst},
											  {"var", kVar},
											  {"begin", kBegin},
											  {"end", kEnd},
											  {"repeat", kRepeat},
											  {"do", kDo},
											  {"read", kRead},
											  {"write", kWrite},
											  {"to", kTo},
											  {"else", kElse},
											  {"if", kIf},
											  {"then", kThen},
											  {"function", kFunction},
											  {"downto", kDownto},
											  {"until", kUntil},
											  {"for", kFor},
											  {"array", kArray},
											  {"of", kOf},
											  {"integer", kInteger},
											  {"char", kChar},
											  {"real", kReal},
											  {"procedure", kProcedure} };

	//SymType sym;

	static void error() {
		std::cout << "ERROR" << std::endl;
		std::exit(0);
	}

	typedef struct {
		std::string name;
		int level;
		SymType kind;
		int address;
		bool ifArray;
		int num;
		double real;
		char c;
	} table;

	typedef struct {
		std::string op;
		int x;
		int y;
	} code;
}  // namespace compiler
#endif