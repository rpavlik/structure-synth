#pragma once

#include <QString>
#include <QStatusBar>
#include "../../SyntopiaCore/GLEngine/EngineWidget.h"


namespace StructureSynth {
	namespace JavaScriptSupport {	

		/// Responsible for setting up the JavaScript environment and parsing
		class JavaScriptParser {
		public:
			JavaScriptParser(SyntopiaCore::GLEngine::EngineWidget* engine3D, QStatusBar* statusBar);
			~JavaScriptParser();
			
			void parse(QString input);
		private:
			SyntopiaCore::GLEngine::EngineWidget* engine3D;
			QStatusBar* statusBar;
		};

	}
}

