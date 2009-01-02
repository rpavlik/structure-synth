#pragma once

#include <QString>
#include <QVector>
#include <QWidget>

/// Classes for the GUI Editor for the preprocessor constant variables.
/// E.g. the line: #define angle 45 (float:0.0-360.0)
///	will make a simple editor widget appear.
namespace StructureSynth {
	namespace GUI {

		/// The supported types of variables.
		enum VariableType { StringVariable, FloatVariable };

		/// A widget editor for a constant variable.
		class VariableWidget {
		public:
			/// FloatVariable constructor.
			VariableWidget(QString name, double defaultValue, double min, double max);
			
			/// StringVariable constructor.
			VariableWidget(QString name, QString defaultValue);
			
			QString getValueAsText();
			QString getName() const { return name; };
			VariableType getType() const { return type; };
			bool isUpdated() const { return updated; };
			void setUpdated(bool value) { updated = value; };
			
		private:
			QString name;
			VariableType type;
			bool updated;
			QWidget* widget;
		};
	
		/// The Variable Editor window.
		class VariableEditor {
		public:
			VariableEditor();

			void beginUpdate();
			void endUpdate();   // If a widget has not been requested, it is removed when this is called. 
			
			void addFloatVariable(QString name, double defaultValue);
			void addStringVariable(QString name, QString defaultValue);

		private:
			QVector<VariableWidget> variables;
		};
	}
}

