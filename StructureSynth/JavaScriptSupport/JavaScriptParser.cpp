#include "JavaScriptParser.h"

#include <QScriptEngine>
#include "SyntopiaCore/Logging/Logging.h"

#include "Debug.h"

using namespace SyntopiaCore::Logging;


namespace StructureSynth {
	namespace JavaScriptSupport {	

		JavaScriptParser::JavaScriptParser(){
		}

		JavaScriptParser::~JavaScriptParser() {
		}
			
		void JavaScriptParser::parse(QString input) {

			INFO("Initializing JavaScript environment.");
			QScriptEngine engine;

			// Setup the global objects...
			Debug debugObject;
			engine.globalObject().setProperty("Debug", engine.newQObject(&debugObject)); 

			// Execute and catch exceptions.
			QScriptValue result = engine.evaluate(input);
			if (engine.hasUncaughtException()) {
				int line = engine.uncaughtExceptionLineNumber();
				QString error =  QString("Uncaught exception at line %1:%2").arg(line).arg(result.toString());
				WARNING(error);
			} else {
				INFO(result.toString());
			}

		}
		
	}
}
