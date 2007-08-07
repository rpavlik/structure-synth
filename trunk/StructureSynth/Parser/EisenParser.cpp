#include "EisenParser.h"

#include "../../AppCore/Logging/Logging.h"
#include "../Model/CustomRule.h"

using namespace AppCore::Logging;
using namespace StructureSynth::Model;

namespace StructureSynth {
	namespace Parser {	
		
		/*
			EISENSCRIPT
			-----------

			EBNF:
			-----

			program = { set | rule } ;
			rule = 'RULE' ,  rule_name, { rule_modifier } , '{', { set | action } , '}' ;
			rule_modifier = { 'MAXDEPTH' integer | WEIGHT float }
			action = transformation rule_ref ;
			set = 'SET'  , var_name , string ;

			'rule_ref', 'var_name', and 'string' are text strings with a reasonable set of allowed characters.

			Example:
			--------

			//
			//	C++ style multiline comments are allowed.
			//  A pre-processor strips comments, and imports '#include's
			//
			#include "../basicstuff.es"
			SET BackgroundColor = #F00  // All 'SET' commands outside the scope of a rule are executed at startup.

			RULE core {
				r1 
			}

			RULE r1 5 {
			}

			RULE r2 1 {
			}
		*/


		EisenParser::EisenParser(Tokenizer* tokenizer) : tokenizer(tokenizer) {
		};

		EisenParser::~EisenParser() {
		};


		void EisenParser::getSymbol() {
			symbol = tokenizer->getSymbol();
		};

		bool EisenParser::accept(Symbol::SymbolType st) {
			if (symbol.type == st) {
				getSymbol();
				return true;
			}
			return false;
		}

		void EisenParser::ruleModifierList(CustomRule* customRule) {
			while (symbol.type == Symbol::Operator) { 
				if (symbol.text == "weight") {
					getSymbol();
					double param = symbol.intValue;
					if (!accept(Symbol::Number)) {
						throw (ParseError("Rule modifier 'weight' expected numerical argument. Found: " + symbol.text));
					}
					customRule->setWeight(param);

				} else if (symbol.text == "maxdepth") {
					getSymbol();
					int param = symbol.getNumerical();
					if (!symbol.isInteger || !accept(Symbol::Number)) {
						throw (ParseError("Rule modifier 'maxdepth' expected integer argument. Found: " + symbol.text));
					}
					customRule->setMaxDepth(param);
				}
			}

			if (!symbol.type == Symbol::LeftBracket) {
				throw (ParseError("After rule modifier list: expected a left bracket. Found: " + symbol.text));
			}
		}

		Rule* EisenParser::rule() {
			// rule = 'RULE' ,  rule_name, '{', { set | action }  , '}' ;
			
			
			if (!accept(Symbol::Rule)) throw (ParseError("Unexpected: trying to parse Rule not starting with rule identifier. Found: " + symbol.text));
			
			QString ruleName = symbol.text;
			if (!accept(Symbol::UserString)) throw (ParseError("After rule identifier a rule name is expected. Found: " + symbol.text));
			CustomRule* customRule = new CustomRule(ruleName);

			if (symbol.type != Symbol::LeftBracket) {
				// This must be a rule_modifier list.
				ruleModifierList(customRule);
			}


			if (!accept(Symbol::LeftBracket)) throw (ParseError("After rule name a left bracket is expected. Found: " + symbol.text));
			
			

			// TODO: implement rest of types:
			// Possible actions:
			//   SET something = something
			//   rulename
			//   { yaw 20 size 0.1 } rulename
			//   20 * { forward 10 } rulename
			while (symbol.type == Symbol::LeftBracket || symbol.type == Symbol::UserString ||
				symbol.type == Symbol::Number || symbol.type == Symbol::Set) {    
				
				if (symbol.type == Symbol::Set) {
					Action a = setAction(); 
					customRule->appendAction(a);
				} else {
					Action a = action(); 
					customRule->appendAction(a);
				}
			}

			if (!accept(Symbol::RightBracket)) throw (ParseError("A rule definition must end with a right bracket. Found: "+symbol.text));

			return customRule;
		}

		Transformation EisenParser::transformation() {

			QString type = symbol.text;
			if (!accept(Symbol::Operator)) throw (ParseError("Transformation: Expected transformation identifier (e.g. 'x' or 'rx'). Found: " + symbol.text));

			if (type == "x") {
				double param = symbol.getNumerical();
				if (!accept(Symbol::Number)) throw (ParseError("Transformation 'X' (X-axis translation): Expected numerical parameter. Found: " + symbol.text));
				INFO("TX:" + symbol.text);
				return Transformation::createX(param);
			} else if (type == "y") {
				double param = symbol.getNumerical();
				if (!accept(Symbol::Number)) throw (ParseError("Transformation 'Y' (Y-axis translation): Expected numerical parameter. Found: " + symbol.text));
				return Transformation::createY(param);
			} else if (type == "z") {
				double param = symbol.getNumerical();
				if (!accept(Symbol::Number)) throw (ParseError("Transformation 'Z' (Z-axis translation): Expected numerical parameter. Found: " + symbol.text));
				return Transformation::createZ(param);
			} else if (type == "rx") {
				double param = symbol.getNumerical();
				if (!accept(Symbol::Number)) throw (ParseError("Transformation 'RX' (X-axis rotation): Expected numerical parameter. Found: " + symbol.text));
				return Transformation::createRX(param);
			} else if (type == "ry") {
				double param = symbol.getNumerical();
				if (!accept(Symbol::Number)) throw (ParseError("Transformation 'RY' (Y-axis rotation): Expected numerical parameter. Found: " + symbol.text));
				return Transformation::createRY(param);
			} else if (type == "rz") {
				double param = symbol.getNumerical();
				if (!accept(Symbol::Number)) throw (ParseError("Transformation 'RZ' (Z-axis rotation): Expected numerical parameter. Found: " + symbol.text));
				return Transformation::createRZ(param);
			} else if (type == "s") {
				double param = symbol.getNumerical();
				if (!accept(Symbol::Number)) throw (ParseError("Transformation 'S' (size): Expected numerical parameter. Found: " + symbol.text));
				return Transformation::createScale(param,param,param);
			} else if (type == "fx") {
				return Transformation::createScale(-1,1,1);
			} else if (type == "fy") {
				return Transformation::createScale(1,-1,1);
			} else if (type == "fz") {
				return Transformation::createScale(1,1,-1);
			} else {
				throw (ParseError("Unknown transformation type: " + type));
			}
			return Transformation();
		}

		Transformation EisenParser::transformationList() {
			// A transformationlist is something like: 
			// { x 23 rx 23 }

			Transformation t;

			if (!accept(Symbol::LeftBracket)) throw (ParseError("Transformation List: Expected a left bracket. Found: " + symbol.text));

			while (symbol.type == Symbol::Operator) {    
				t.append(transformation());
			}

			if (!accept(Symbol::RightBracket)) throw (ParseError("Transformation List: Expected a right bracket or an operator. Found: " + symbol.text));

			return t;

		}

		Action EisenParser::action() {
			// There are 3 types of action statements:
			//  { yaw 20 pitch 30 roll 20 } rulename
			//  rulename
			//  20 * { forward 10 } 10 * { left 10 } rulename
			
			if (symbol.type == Symbol::LeftBracket) {
				Transformation t = transformationList();
				QString ruleName = symbol.text;
				if (!accept(Symbol::UserString)) throw (ParseError("Expected a rule name after the transformation list. Found: " + symbol.text));
				return Action(t, ruleName);
			} else if (symbol.type == Symbol::UserString) {
				QString ruleName = symbol.text;
				accept(Symbol::UserString);
				return Action(ruleName);
			} else if (symbol.type == Symbol::Number) {
				// TODO:
				throw (ParseError("Not impl yet. Found: " + symbol.text));
				accept(Symbol::UserString);
				return Action();
			} else {
				throw (ParseError("A rule action must start with either a number, a rule name or a left bracket. Found: "+symbol.text));	
			}
		}

		Action EisenParser::setAction() {
				accept(Symbol::Set);
				
				QString key = symbol.text;
				if (!accept(Symbol::UserString)) throw (ParseError("Expected a valid setting name. Found: " + symbol.text));
				QString value = symbol.text; 
				getSymbol(); // We will accept everything here! 
				INFO(key + " " + value);

				return Action(key,value);
		}

		RuleSet* EisenParser::ruleset() {
			RuleSet*  rs = new RuleSet();
			getSymbol();
	
			while (symbol.type == Symbol::Rule || symbol.type == Symbol::Set) {    
				if (symbol.type == Symbol::Rule) {
					Rule* r = rule(); 
					rs->addRule(r);
				}
				if (symbol.type == Symbol::Set) {
					WARNING("'Set' is not supported yet!");
				}
			}

			if (!accept(Symbol::End)) throw (ParseError("Unexpected symbol found. At this scope only RULE and SET statements are allowed. Found: " + symbol.text));
			return rs;
		}


		RuleSet* EisenParser::parseRuleset() {
			return ruleset();
		}
	}
}