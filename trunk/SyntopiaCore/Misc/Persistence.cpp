#include "Persistence.h"

#include "../Logging/Logging.h"

#include <QMap>
#include <QComboBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QSpinBox>
#include <QRadioButton>
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
				//INFO("Storing " + storageName);
				if (qobject_cast<QLineEdit*>(widget)) {
					QLineEdit* lineEdit = qobject_cast<QLineEdit*>(widget); 
					store.map[storageName] = QVariant(lineEdit->text());
					//INFO("--stored " + lineEdit->text());
				} else if (qobject_cast<QCheckBox*>(widget)) {
					QCheckBox* cb = qobject_cast<QCheckBox*>(widget); 
					store.map[storageName] = QVariant(cb->isChecked());
					//INFO("--stored " + store.map[storageName].toString());
				} else if (qobject_cast<QSpinBox*>(widget)) {
					QSpinBox* sb = qobject_cast<QSpinBox*>(widget); 
					store.map[storageName] = QVariant(sb->value());
					INFO("--stored " + store.map[storageName].toString());
				} else if (qobject_cast<QRadioButton*>(widget)) {
					QRadioButton* rb = qobject_cast<QRadioButton*>(widget); 
					store.map[storageName] = QVariant(rb->isChecked());
					//INFO("--stored " + store.map[storageName].toString());
				} else if (qobject_cast<QComboBox*>(widget)) {
					QComboBox* cb = qobject_cast<QComboBox*>(widget); 
					store.map[storageName] = QVariant(cb->currentText());
					INFO("--stored " + store.map[storageName].toString());
				} else {
					WARNING("Unsupported widget: " + widget->objectName());
				}
			} else {
				//INFO("Restoring " + storageName);
				if (qobject_cast<QLineEdit*>(widget)) {
					QLineEdit* lineEdit = qobject_cast<QLineEdit*>(widget); 
					if (store.map.contains(storageName)) {
						lineEdit->setText(store.map[storageName].toString());
						//INFO("--restored " + store.map[storageName].toString());
					}
					
				} else if (qobject_cast<QCheckBox*>(widget)) {
					QCheckBox* cb = qobject_cast<QCheckBox*>(widget); 
					if (store.map.contains(storageName)) {
						cb->setChecked(store.map[storageName].toBool());
						//INFO("--restored " + store.map[storageName].toString());
					}
					
				} else if (qobject_cast<QSpinBox*>(widget)) {
					QSpinBox* sb = qobject_cast<QSpinBox*>(widget); 
					if (store.map.contains(storageName)) {
						sb->setValue(store.map[storageName].toInt());
						INFO("--restored " + store.map[storageName].toString());
					}
					
				} else if (qobject_cast<QRadioButton*>(widget)) {
					QRadioButton* rb = qobject_cast<QRadioButton*>(widget); 
					if (store.map.contains(storageName)) {
						rb->setChecked(store.map[storageName].toInt());
						//INFO("--restored " + store.map[storageName].toString());
					}
					
				} else if (qobject_cast<QComboBox*>(widget)) {
					QComboBox* rb = qobject_cast<QComboBox*>(widget); 
					if (store.map.contains(storageName)) {
						int i = rb->findText(store.map[storageName].toString());
						if (i!=-1) {
							rb->setCurrentIndex(i);
						   INFO("--restored " + store.map[storageName].toString());
						} else {
							 INFO("-- failed restore " + store.map[storageName].toString());
						}
					}
					
				} else {
					WARNING("Unsupported widget: " + widget->objectName());
				}
			}
		}


	}
}

