#pragma once

#include <QString>


namespace StructureSynth {
	namespace JavaScriptSupport {	

		/// Responsible for setting up the JavaScript environment and parsing
		class JavaScriptParser {
		public:
			JavaScriptParser();
			~JavaScriptParser();
			
			void parse(QString input);
		private:
		};

	}
}

