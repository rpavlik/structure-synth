#include "ListWidgetLogger.h"

#ifdef WIN32
#include "windows.h"
#endif

namespace AppCore {
	namespace Logging {

		ListWidgetLogger::ListWidgetLogger(QWidget* parent) { 	
			listWidget = new QListWidget(parent);
		}

		ListWidgetLogger::~ListWidgetLogger() { 
		}

		void ListWidgetLogger::log(QString message, LogLevel priority) {
			QListWidgetItem* i = new QListWidgetItem(message, listWidget);

			// Levels: NoneLevel, DebugLevel, TimingLevel, InfoLevel, WarningLevel, CriticalLevel, AllLevel

			if ( priority == InfoLevel ) {
				i->setBackgroundColor(QColor(255,255,255));
			} else if ( priority == WarningLevel ) {
				i->setBackgroundColor(QColor(255,243,73));
			} else if ( priority == CriticalLevel ) {
				i->setBackgroundColor(QColor(255,2,0));
			} else {
				i->setBackgroundColor(QColor(220,220,220));
			}
			listWidget->scrollToItem(i); 

#ifdef WIN32
			OutputDebugString((LPCWSTR) (message+"\r\n").utf16());
#endif
		}

	}
}
