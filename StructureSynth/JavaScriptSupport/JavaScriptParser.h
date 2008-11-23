#pragma once

#include <QString>
#include "../../SyntopiaCore/GLEngine/EngineWidget.h"


namespace StructureSynth {
	namespace JavaScriptSupport {	

		/// Responsible for setting up the JavaScript environment and parsing
		class JavaScriptParser {
		public:
			JavaScriptParser(SyntopiaCore::GLEngine::EngineWidget* engine3D);
			~JavaScriptParser();
			
			void parse(QString input);
		private:
			SyntopiaCore::GLEngine::EngineWidget* engine3D;
		};

	}
}

