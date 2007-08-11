#include "Tokenizer.h"

#include <QStringList>

#include "../../AppCore/Exceptions/Exception.h"
#include "../../AppCore/Logging/Logging.h"

using namespace AppCore::Exceptions;
using namespace AppCore::Logging;


namespace StructureSynth {
	namespace Parser {	

		Tokenizer::~Tokenizer() {
		}

		Tokenizer::Tokenizer(QString input) {

			QStringList operators;
			operators << "a" << "alpha" << "h" << "hue" << "sat" << "b" << "brightness" << "v" << "x" << "y" << "z" << "rx" << "ry" << "rz" << "s" << "fx" << "fy" << "fz" << "maxdepth" << "weight" << "md" << "w";

			currentSymbol = -1;

			// We will split on whitespace and line breaks
			// TODO: Respect quotations ( file = "C:\Program Files\Test" )
			QStringList l = input.split(QRegExp("\\s|\\r|\\n"), QString::SkipEmptyParts);

			for (int i = 0; i < l.size(); i++) {
				QString s = l[i];
				QString sl = l[i].toLower();

				if (sl == "rule") {
					symbols.append(Symbol(Symbol::Rule, s));
				} else if (sl == "{") {
					symbols.append(Symbol(Symbol::LeftBracket, s));
				} else if (sl == ">") {
					symbols.append(Symbol(Symbol::MoreThan, s));
				}else if (sl == "}") {
					symbols.append(Symbol(Symbol::RightBracket, s));
				} else if (sl == "*") {
					symbols.append(Symbol(Symbol::Multiply, s));
				} else if (sl == "set") {
					symbols.append(Symbol(Symbol::Set, s));
				} else if (QString("+-0123456789").contains(s[0])) {
					// It is a number (hopefully)

					if (s.count("/") == 1) {
						QString s1 = s.section("/",0,0);
						QString s2 = s.section("/",1,1);
						bool succes1 = false;
						int i1 = s1.toInt(&succes1);
						bool succes2 = false;
						int i2 = s2.toInt(&succes2);

						if ((i1 && i2) && (i2 != 0)) {
							Symbol ns(Symbol::Number, s);
							ns.isInteger = false;
							ns.floatValue = ((double)i1)/i2;
							symbols.append(ns);
							continue;

						} else {
							throw ParseError("Invalid fraction found: " + s);
						}
					}

					bool succes = false;
					int i = s.toInt(&succes);

					if (succes) {
						Symbol ns(Symbol::Number, s);
						ns.isInteger = true;
						ns.intValue = i;
						symbols.append(ns);
						continue;
					}

					// the number was not an integer... Is it a floating-point value?

					double d = s.toDouble(&succes);

					if (succes) {
						Symbol ns(Symbol::Number, s);
						ns.isInteger = false;
						ns.floatValue = d;
						//INFO(QString("Added float value:%1").arg(ns.floatValue));
						symbols.append(ns);
						continue;
					}

					throw ParseError("Invalid symbol found: " + s);
				} else if (operators.contains(sl) ) {
					QString longName = sl;
					
					// Resolve abbreviations
					if (longName == "md") longName = "maxdepth";
					if (longName == "w") longName = "weight";
					if (longName == "h") longName = "hue";
					if (longName == "b") longName = "brightness";
					if (longName == "a") longName = "alpha";
					
					
					Symbol ns(Symbol::Operator, longName);
					symbols.append(ns);

				} else {
					// TODO: We should check syntax of userstring here... (we dont want strings like slk"{/})
					Symbol ns(Symbol::UserString, sl);
					symbols.append(ns);
				}
			}






		}

		Symbol Tokenizer::getSymbol() {
			currentSymbol++;

			if (currentSymbol < symbols.size()) {
				return symbols[currentSymbol];
			} 	

			return Symbol(Symbol::End, "#END#");
		}
	}
}