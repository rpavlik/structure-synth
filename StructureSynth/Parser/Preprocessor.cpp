#include "Preprocessor.h"

#include <QStringList>

#include "../../SyntopiaCore/Exceptions/Exception.h"
#include "../../SyntopiaCore/Logging/Logging.h"

using namespace SyntopiaCore::Exceptions;
using namespace SyntopiaCore::Logging;


namespace StructureSynth {
	namespace Parser {	

		QString Preprocessor::Process(QString input) {
			QStringList in = input.split(QRegExp("\r\n|\r|\n"));
			QStringList out;
			bool inComment = false;
			for (int i = 0; i < in.size(); i++) {
				QString l = in[i];
				l.remove(QRegExp("//.*$")); // remove single-line comments 
				if (l.contains("/*")) inComment = true;   // remove multi-line comments
				
				if (l.contains("*/")) {
					inComment = false;
				} else {
					if (!inComment) {
						out.append(l);	
					}
				}
			}
			return out.join("\r\n");
		}
	}
}