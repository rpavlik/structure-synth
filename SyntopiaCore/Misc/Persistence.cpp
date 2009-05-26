#include "Persistence.h"

#include "../Logging/Logging.h"

#include <QMap>
#include <QLineEdit>
#include <QVariant>

using namespace SyntopiaCore::Logging;

namespace SyntopiaCore {
	namespace Misc {	

		namespace {
			class PersistenceStore {
			public:
				QMap< QString, QVariant > map;
			};
		}

		void Persistence::Store(QWidget* widget, QString storageName) {
			if (storageName.isEmpty()) storageName = widget->objectName();
			 Manage(widget, storageName, true);
		}

		void Persistence::Restore(QWidget* widget, QString storageName) {
			 if (storageName.isEmpty()) storageName = widget->objectName();
			Manage(widget, storageName, false);
		}

		void Persistence::Manage(QWidget* widget, QString storageName, bool doStore) {
			static PersistenceStore store;

			if (doStore) {
				INFO("Storing " + storageName);
				if (qobject_cast<QLineEdit*>(widget)) {
					QLineEdit* lineEdit = qobject_cast<QLineEdit*>(widget); 
					store.map[storageName] = QVariant(lineEdit->text());
					INFO("--stored " + lineEdit->text());
				}
			} else {
				INFO("Restoring " + storageName);
				if (qobject_cast<QLineEdit*>(widget)) {
					QLineEdit* lineEdit = qobject_cast<QLineEdit*>(widget); 
					if (store.map.contains(storageName)) {
						lineEdit->setText(store.map[storageName].toString());
					}
					INFO("--restored " + store.map[storageName].toString());
				}
			}
		}


	}
}

