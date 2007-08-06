#pragma once

#include <QVector>
#include <QGLWidget>
#include <QPoint>
#include <QList>

#include "AppCore/Math/Vector3.h"
#include "AppCore/Math/Matrix4.h"

#include "Object3D.h"

namespace AppCore {
	namespace GLEngine {	

		
		/// Settings for the GLEngine
		class Settings {
		public:

			// Default constructor
			Settings() {
				perspectiveAngle = 22.0;
				nearClipping = 5.0;
				farClipping = 60.0;
			}

			// Projection settings
			double perspectiveAngle;
			double nearClipping;
			double farClipping;
		};


		/// Widget for the mini OpenGL engine.
		class EngineWidget : public QGLWidget {
		public:
			/// Constructor
			EngineWidget(QWidget* parent);

			/// Destructor
			~EngineWidget();

			/// Use this whenever the an redraw is required.
			/// Calling this function multiple times will still only result in one redraw
			void requireRedraw();

			void clearWorld();
			void addObject(Object3D* object);
			int objectCount() { return objects.size(); }

		protected:
			void initializeGL();
			void timerEvent( QTimerEvent * );

			/// Actual drawing is implemented here
			void paintGL();

			/// Triggers a perspective update and a redraw
			void resizeGL(int w, int h);
			void wheelEvent(QWheelEvent* e);
			void mouseMoveEvent(QMouseEvent* e);
			void rotateWorldXY(double x, double y);
			void rotateWorldZ(double z);
			void translateWorld(double x, double y, double z);

		

		private:
			// Creates the appropriate GL_PROJECTION matrix
			void updatePerspective();	
			AppCore::Math::Vector3f screenTo3D(int sx, int sy);
		

			int pendingRedraws; // the number of times we must redraw 
			// (when a redraw is requested we must draw two times, when double buffering)
			int requiredRedraws;
			Settings settings;
			double scale;
			double mouseSpeed;
			double mouseTranslationSpeed;
			double minimumScale;
			QPoint oldPos;

			AppCore::Math::Vector3f translation;
			AppCore::Math::Vector3f pivot;
			AppCore::Math::Matrix4f rotation;

			QList<Object3D*> objects;
		};
	};

};

