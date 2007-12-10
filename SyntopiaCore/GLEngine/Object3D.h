#pragma once

#include "SyntopiaCore/Math/Vector3.h"
#include <QGLWidget>
#include <QtOpenGL>

namespace SyntopiaCore {
	namespace GLEngine {	

		class Object3D {
		public:
			Object3D() {};
			virtual ~Object3D() {};

			virtual void draw() const = 0;

			void setColor(SyntopiaCore::Math::Vector3f rgb, float alpha);
			const GLfloat (&getColor() const)[4] { return primaryColor; }

			void getBoundingBox(SyntopiaCore::Math::Vector3f& from, SyntopiaCore::Math::Vector3f& to) const;
			void expandBoundingBox(SyntopiaCore::Math::Vector3f& from, SyntopiaCore::Math::Vector3f& to) const;

		protected:

			void vertex(SyntopiaCore::Math::Vector3f v) const { glVertex3f(v.x(), v.y(), v.z()); }
			void normal(SyntopiaCore::Math::Vector3f v) const { glNormal3f(v.x(), v.y(), v.z()); }
			void vertex4(SyntopiaCore::Math::Vector3f v1,SyntopiaCore::Math::Vector3f v2,SyntopiaCore::Math::Vector3f v3,SyntopiaCore::Math::Vector3f v4) const { vertex(v1); vertex(v2); vertex(v3); vertex(v4); }
			void vertex4r(SyntopiaCore::Math::Vector3f v1,SyntopiaCore::Math::Vector3f v2,SyntopiaCore::Math::Vector3f v3,SyntopiaCore::Math::Vector3f v4) const { vertex(v4); vertex(v3); vertex(v2); vertex(v1); }
			void vertex4n(SyntopiaCore::Math::Vector3f v1,SyntopiaCore::Math::Vector3f v2,SyntopiaCore::Math::Vector3f v3,SyntopiaCore::Math::Vector3f v4) const;
			void vertex4rn(SyntopiaCore::Math::Vector3f v1,SyntopiaCore::Math::Vector3f v2,SyntopiaCore::Math::Vector3f v3,SyntopiaCore::Math::Vector3f v4) const ;
			void vertex4nc(SyntopiaCore::Math::Vector3f v1,SyntopiaCore::Math::Vector3f v2,SyntopiaCore::Math::Vector3f v3,SyntopiaCore::Math::Vector3f v4,SyntopiaCore::Math::Vector3f center) const ;
			void vertex4rnc(SyntopiaCore::Math::Vector3f v1,SyntopiaCore::Math::Vector3f v2,SyntopiaCore::Math::Vector3f v3,SyntopiaCore::Math::Vector3f v4,SyntopiaCore::Math::Vector3f center) const;

			GLfloat primaryColor[4];

			SyntopiaCore::Math::Vector3f from;
			SyntopiaCore::Math::Vector3f to;

		};

	}
}

