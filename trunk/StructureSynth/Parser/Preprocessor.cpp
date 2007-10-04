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
				l.remove(QRegExp("//.*$"));
				if (l.contains("/*")) inComment = true;
				if (l.contains("*/")) inComment = false;
				l.remove(QRegExp("/\\*.*$"));
				l.remove(QRegExp("^.*\\*/"));
				if (!inComment) {
					out.append(l);	
				}
			}
			return out.join("\r\n");
		}
	}
}