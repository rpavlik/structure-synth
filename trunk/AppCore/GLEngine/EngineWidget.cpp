#include "EngineWidget.h"
#include "../Math/Vector3.h"
#include "../Logging/Logging.h"

#include "Sphere.h"

using namespace AppCore::Math;
using namespace AppCore::Logging;

#include <QWheelEvent>

namespace AppCore {
	namespace GLEngine {


		EngineWidget::EngineWidget(QWidget* parent) : QGLWidget(parent) {

			
			updatePerspective();

			pendingRedraws = 0;
			requiredRedraws = 2; // for double buffering
			startTimer( 10 );
			oldPos = QPoint(0,0);

			// Engine settings
			scale = 0.4;
			minimumScale = 0.01;
			mouseSpeed = 1.0;
			mouseTranslationSpeed = 64.0;
			translation = Vector3f(0,0,-20);

			setMouseTracking(true);

			rotation = Matrix4f::Identity();
			pivot = Vector3f(0,0,0);



			
		}

		EngineWidget::~EngineWidget() {
		}

		void EngineWidget::requireRedraw() {
			pendingRedraws = requiredRedraws;
		}

		void EngineWidget::paintGL() {
			if (pendingRedraws > 0) pendingRedraws--;

			QColor c = QColor (30,30,30);
			qglClearColor(c);
			glMatrixMode(GL_MODELVIEW);

			glLoadIdentity();

			// Calc camera position
			glTranslatef( translation.x(), translation.y(), translation.z() );
			glScalef( scale, scale, scale );
			Vector3f v(1,2,3);
			Vector3f v2 = rotation*v;
			glMultMatrixf(rotation.getArray());
			glTranslatef( -pivot.x(), -pivot.y(), -pivot.z() );
			glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


			for (int i = 0; i < objects.size(); i++) {
				objects[i]->draw();
			}

		};

		void EngineWidget::resizeGL( int /* width */, int /* height */) {
			// When resizing the perspective must be recalculated
			requireRedraw();
			updatePerspective();
		};

		void EngineWidget::updatePerspective() {
			if (height() == 0) return;

			GLfloat w = (float) width() / (float) height();
			GLfloat h = 1.0;

			glViewport( 0, 0, width(), height() );
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();

			gluPerspective(settings.perspectiveAngle, w,  (float)settings.nearClipping, (float)settings.farClipping);
			//glOrtho( -w, w, -h, h, (float)0, (float) 60 );
		}

		void EngineWidget::timerEvent(QTimerEvent*) {
			static bool firstTime = true;
			if (firstTime) {
				firstTime = false;
				updatePerspective(); 
				requireRedraw();
			}
			if (pendingRedraws) updateGL();
		}


		void EngineWidget::initializeGL()
		{
			requireRedraw();

			GLfloat pos[4] = {-55.0f, -25.0f, 20.0f, 1.0f };
			GLfloat pos2[4] = {52.0f, 25.0f, 50.0f, 1.0f };
			GLfloat agreen[4] = {0.0f, 0.8f, 0.2f, 1.0f };

			float ambient = 0.4f;
			float diffuse = 0.6f;
			float specular = 0.1f;
			Vector3f color = Vector3f(1,1,1);
			GLfloat ambientLight[] = { ambient * color.x(),   ambient * color.y(),  ambient * color.z(), 1.0f };
			GLfloat diffuseLight[] = { diffuse * color.x(),   diffuse * color.y(),  diffuse * color.z(), 1.0f };
			GLfloat specularLight[] = {specular * color.x(),  specular * color.y(), specular* color.z(), 1.0f };
			glLightfv(0, GL_AMBIENT, ambientLight);
			glLightfv(0, GL_DIFFUSE, diffuseLight);
			glLightfv(0, GL_SPECULAR, specularLight);
			glLightfv(0, GL_POSITION, pos );
			glEnable( GL_LIGHT0 ); 



			glEnable( GL_CULL_FACE );
			glEnable( GL_LIGHTING );
			glEnable( GL_DEPTH_TEST );
			glEnable( GL_NORMALIZE );
			glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, agreen );

			glEnable(GL_LINE_SMOOTH);
			glEnable(GL_POINT_SMOOTH);
			glEnable(GL_POLYGON_SMOOTH);
			glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
		}


		void EngineWidget::wheelEvent(QWheelEvent* e) {
			e->accept();

			// e->delta() is (typically) multipla of 4000
			double interval = (double)e->delta() / 8000.0;

			if ( scale <= minimumScale ) return;
			scale -= mouseSpeed*interval ;
		}

		void EngineWidget::mouseMoveEvent( QMouseEvent *e ) {
			e->accept();

			// store old position

			if (oldPos.x() == 0 && oldPos.y() == 0) {
				// first time
				oldPos = e->pos(); 
				return;
			}
			double dx = e->x() - oldPos.x();
			double dy = e->y() - oldPos.y();

			// normalize wrt screen size
			double rx = dx / width();
			double ry = dy / height();

			oldPos = e->pos();

			if ( (e->buttons() == Qt::LeftButton && e->modifiers() == Qt::ShiftModifier ) 
				|| e->buttons() == (Qt::LeftButton | Qt::RightButton )) 
			{
				// 1) dragging with left mouse button + shift down, or
				// 2) dragging with left and right mouse button down
				//
				// This results in zooming for vertical movement, and Z axis rotation for horizontal movement.

				scale += (ry * 4.0f);

				if (scale < minimumScale) scale = minimumScale;

				rotateWorldZ( rx );
				requireRedraw();
			} else if ( ( e->buttons() == Qt::RightButton ) 
				|| (e->buttons() == Qt::LeftButton && e->modifiers() == Qt::ControlModifier ) 
				|| (e->buttons() == Qt::LeftButton && e->modifiers() == Qt::MetaModifier ) ) 
			{ 
				// 1) dragging with right mouse button, 
				// 2) dragging with left button + control button,
				// 3) dragging with left button + meta button (Mac)
				//
				// results in translation

				translateWorld( mouseSpeed*mouseTranslationSpeed* rx, - mouseSpeed*mouseTranslationSpeed*ry, 0 );
				requireRedraw();
			} else if ( e->buttons() == Qt::LeftButton ) {
				// Dragging with left mouse button.
				// Results in rotation.

				rotateWorldXY( rx, ry );
				requireRedraw();
			}
		}


		Vector3f EngineWidget::screenTo3D(int sx, int sy, int sz) {
			// 2D -> 3D conversion
			GLdouble modelView[16];
			GLdouble projection[16];
			GLint viewPort[16];

			glGetDoublev( GL_MODELVIEW_MATRIX, modelView );
			glGetDoublev( GL_PROJECTION_MATRIX, projection );
			glGetIntegerv( GL_VIEWPORT, viewPort);

			GLdouble x, y, z;
			float h = (float)height(); 
			gluUnProject(sx, h-sy, sz, modelView, projection, viewPort, &x, &y ,&z);
			return Vector3f(x,y,z);
		}

		void EngineWidget::rotateWorldXY(double x, double y) {
			double rotateSpeed = 5.0;

			Vector3f startPoint = screenTo3D(oldPos.x(), oldPos.y(),1);
			Vector3f xDir =       (screenTo3D(oldPos.x()+10, oldPos.y(),1) - startPoint).normalize() ;
			Vector3f yDir =       (screenTo3D(oldPos.x(), oldPos.y()+10,1) - startPoint).normalize() ;

			Matrix4f mx = Matrix4f::Rotation(xDir, y*rotateSpeed);
			Matrix4f my = Matrix4f::Rotation(yDir, -x*rotateSpeed);
			rotation = rotation*my * mx   ; 
			requireRedraw();
		}

		void EngineWidget::rotateWorldZ(double z) {
			double rotateSpeed = 5.0;

			Vector3f startPoint = screenTo3D(oldPos.x(), oldPos.y(), 0);
			Vector3f endPoint =   screenTo3D(oldPos.x(), oldPos.y(), 1);

			Matrix4f mz = Matrix4f::Rotation(startPoint-endPoint, z*rotateSpeed);
			rotation = rotation*mz  ; 
			requireRedraw();
		}

		void EngineWidget::translateWorld(double x, double y, double z) {
			translation = Vector3f(translation.x() + x, translation.y() + y, translation.z() + z);
		}


		void EngineWidget::clearWorld() {
			for (int i = 0; i < objects.size(); i++) delete(objects[i]);
			objects.clear();
		}

		void EngineWidget::addObject(Object3D* object) {
			objects.append(object);
		}

	}
}