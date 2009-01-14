// --- SKELETON CODE - not used yet!

#pragma once

#include <QString>
#include <QVector>
#include <QWidget>

/// Classes for the GUI Editor for the preprocessor constant variables.
/// E.g. the line: #define angle 45 (float:0.0-360.0)
///	will make a simple editor widget appear.
namespace StructureSynth {
	namespace GUI {
	
		class VariableWidget; // Forward decl...

		/// The Variable Editor window.
		class VariableEditor : public QWidget {
		public:
			VariableEditor(QWidget* parent);

			void beginUpdate() {};
			void endUpdate() {};   // If a widget has not been requested, it is removed when this is called. 
			
			void addFloatVariable(QString name, double defaultValue);
			void addStringVariable(QString name, QString defaultValue);

		private:
			QVector<VariableWidget*> variables;
		};
	}
}

