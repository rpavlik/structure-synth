#pragma once

#include <QString>
#include <QList>

#include "../../SyntopiaCore/Exceptions/Exception.h"

namespace StructureSynth {
	namespace Parser {	

		/// The preprocessor is responsible for removing comments and resolving '#include'
		/// reference to other EisenScript files.
		class Preprocessor {

		public:
			
			static QString Process(QString input);

		private:
			Preprocessor() {};
		};

	}
}