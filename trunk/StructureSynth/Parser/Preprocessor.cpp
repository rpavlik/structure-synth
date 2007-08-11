#include "Preprocessor.h"

#include <QStringList>

#include "../../AppCore/Exceptions/Exception.h"
#include "../../AppCore/Logging/Logging.h"

using namespace AppCore::Exceptions;
using namespace AppCore::Logging;


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