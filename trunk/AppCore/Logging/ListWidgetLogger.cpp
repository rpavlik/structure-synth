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

		void ListWidgetLogger::log(QString message, int priority) {
			QListWidgetItem* i = new QListWidgetItem(message, listWidget);
			i->setBackgroundColor(QColor(220,220,220));
			listWidget->scrollToItem(i); 

#ifdef WIN32
			OutputDebugString((LPCWSTR) (message+"\r\n").utf16());
#endif
		}

	}
}
