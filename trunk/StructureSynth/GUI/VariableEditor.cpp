#include "VariableEditor.h"

#include <QPushButton>
#include <QSlider>
#include <QDoubleSpinBox>
#include <QHBoxLayout>
#include <QLabel>

#include "../../SyntopiaCore/Logging/ListWidgetLogger.h"

using namespace SyntopiaCore::Logging;


namespace StructureSynth {
	namespace GUI {
		namespace {
		
			/// The supported types of variables.
			enum VariableType { StringVariable, FloatVariable };

			/// A widget editor for a constant variable.
			class VariableWidget : public QWidget {
			public:
				/// FloatVariable constructor.
				VariableWidget(QWidget* parent, QString name) : QWidget(parent) {};

				virtual QString getValueAsText() { return ""; };
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

			/// A widget editor for a constant variable.
			class FloatWidget : public VariableWidget {
			public:
				/// FloatVariable constructor.
				FloatWidget(QWidget* parent, QString name, double defaultValue, double min, double max) 
					: VariableWidget(parent, name), defaultValue(defaultValue), min(min), max(max) {
						QHBoxLayout* l = new QHBoxLayout(this);
						//setLayout(l);
						QLabel* label = new QLabel(this);
						label->setText(name);
						l->addWidget(label);
						slider = new QSlider(Qt::Horizontal,this);
						slider->setRange(0,1000);
						slider->setValue(500);
						spinner = new QDoubleSpinBox(this);
						spinner->setMaximum(max);
						spinner->setMinimum(min);
						spinner->setValue(defaultValue);
						l->addWidget(slider);
						l->addWidget(spinner);
				};

				virtual QString getValueAsText() { return ""; };
				
			private:
				QSlider* slider;
				QDoubleSpinBox* spinner;
				double defaultValue;
				double min;
				double max;
			};

		}

			

		VariableEditor::VariableEditor(QWidget* parent) : QWidget(parent) {
			QVBoxLayout* l = new QVBoxLayout(this);
			l->addWidget(new FloatWidget(this, "alpha", 0.5, 0.0, 1.0));
			l->addWidget(new FloatWidget(this, "beta", 0.5, 0.0, 1.0));
			l->addWidget(new FloatWidget(this, "gamma", 0.5, 0.0, 1.0));
			l->addItem(new QSpacerItem(1,1, QSizePolicy::Expanding,  QSizePolicy::Expanding));
		};

		void VariableEditor::addFloatVariable(QString /*name*/, double /*defaultValue*/) {};
		void VariableEditor::addStringVariable(QString /*name*/, QString /*defaultValue*/) {};

	}
}

