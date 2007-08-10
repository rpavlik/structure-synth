#pragma once

#include <QString>
#include <QList>

#include "../../AppCore/Exceptions/Exception.h"

namespace StructureSynth {
	namespace Parser {	

		class ParseError : public AppCore::Exceptions::Exception {
		public:
			ParseError(QString message) : Exception(message) {};
		};

		struct Symbol {
			enum SymbolType { Undefined, LeftBracket, RightBracket, MoreThan, End, Number, Multiply, UserString, Rule, Set, Operator } ;

			SymbolType type;


			Symbol() : floatValue(0), intValue(0), isInteger(false), type(Undefined) { 	};
			Symbol(SymbolType s, QString original) : floatValue(0), intValue(0), isInteger(false), type(s), text(original) { 	};

			
			/// yes, yes, it is a bloated representation. (I don't like unions...)
			QString text;  // The original text-string we parsed. Notice userstrings are converted to lower-case
			double floatValue;
			int    intValue;
			bool   isInteger;

			double getNumerical() {
				if (isInteger) return intValue;
				return floatValue;
			}
		};

		/// The Tokenizer divides an input stream into distinct symbols,
		/// for subsequent parsing.
		class Tokenizer {

		public:
			/// Constructor. 
			Tokenizer(QString input);

			/// Destructor
			~Tokenizer();

			/// Returns the next symbol
			Symbol getSymbol();

		private:
		
			QList<Symbol> symbols;
			int currentSymbol;
		};

	}
}