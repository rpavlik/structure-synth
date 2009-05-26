#pragma once

#include <QString>
#include <QVector>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QFrame>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QTabWidget>
#include <QtGui/QTableWidget>
#include <QtGui/QTextBrowser>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

namespace StructureSynth {
	namespace GUI {


		/// The Variable Editor window.
		class TemplateExportDialog : public QDialog {
			Q_OBJECT
		public:
			TemplateExportDialog(QWidget* parent);
			~TemplateExportDialog();


			void setTemplatePath(QString templatePath);

		public slots:
			void templateChanged(const QString &);


		protected:
			void retranslateUi();

			void setupUi();

		private:
			QString path;
			QVBoxLayout *verticalLayout;
			QHBoxLayout *horizontalLayout;
			QLabel *label;
			QComboBox *templateComboBox;
			QPushButton *templatePathButton;
			QTabWidget *tabWidget;
			QWidget *settingstab;
			QVBoxLayout *verticalLayout_3;
			QLabel *descriptionLabel;
			QTextBrowser *descriptionTextBrowser;
			QLabel *primitivesLabel;
			QTableWidget *primitivesTableWidget;
			QGroupBox *templateOutputGroupBox;
			QVBoxLayout *verticalLayout_2;
			QHBoxLayout *horizontalLayout_2;
			QRadioButton *fileRadioButton;
			QLineEdit *fileNameLineEdit;
			QPushButton *filePushButton;
			QHBoxLayout *horizontalLayout_3;
			QSpacerItem *horizontalSpacer;
			QCheckBox *uniqueCheckBox;
			QRadioButton *clipboardRadioButton;
			QGroupBox *postProcessingGroupBox;
			QVBoxLayout *verticalLayout_4;
			QCheckBox *runAfterCheckBox;
			QHBoxLayout *horizontalLayout_4;
			QSpacerItem *horizontalSpacer_2;
			QLineEdit *afterCommandLineEdit;
			QWidget *advancedTab;
			QVBoxLayout *verticalLayout_5;
			QLabel *outputSizeLabel;
			QHBoxLayout *horizontalLayout_5;
			QLabel *label_4;
			QSpinBox *widthSpinBox;
			QLabel *label_5;
			QSpinBox *heightSpinBox;
			QFrame *line;
			QCheckBox *lockAspectRatioCheckBox;
			QSpacerItem *horizontalSpacer_4;
			QCheckBox *modifyTemplateCheckBox;
			QTextEdit *templateTextEdit;
			QHBoxLayout *horizontalLayout_6;
			QSpacerItem *horizontalSpacer_3;
			QPushButton *pushButton_3;
			QPushButton *pushButton_4;
			QCheckBox *modifyOutputCheckBox;
			QDialogButtonBox *buttonBox;
		};


	}
}

