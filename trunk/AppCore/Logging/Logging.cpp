#include "Logging.h"

/// TODO's
/// - Nested log entris
/// - Time
/// - Setting a log view level


namespace AppCore {
	namespace Logging {	
		QVector<Logger*> Logger::loggers;

		void LOG(QString message, int priority) {
			for (int i = 0; i < Logger::loggers.size(); i++) {
				Logger::loggers[i]->log(message, priority);
			}
		}

		/// Useful aliases
		void Debug(QString text) { LOG(text, DebugLevel); }
		void INFO(QString text) { LOG(text, InfoLevel); }
		void WARNING(QString text) { LOG(text, WarningLevel); }
		void CRITICAL(QString text) { LOG(text, CriticalLevel); }
	}
}