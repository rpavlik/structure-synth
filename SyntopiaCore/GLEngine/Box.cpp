#include "Box.h"

#include "SyntopiaCore/Math/Vector3.h"

using namespace SyntopiaCore::Math;

namespace SyntopiaCore {
	namespace GLEngine {


		Box::Box(SyntopiaCore::Math::Vector3f base, 
				SyntopiaCore::Math::Vector3f dir1 , 
				 SyntopiaCore::Math::Vector3f dir2, 
				 SyntopiaCore::Math::Vector3f dir3) : base(base), v1(dir1), v2(dir2), v3(dir3) 
		{
			/// Bounding box
			from = base;
			to = base;

			if (dir1.x()+base.x() < from.x()) from.x() = (base.x()+dir1.x());
			if (dir2.x()+base.x() < from.x()) from.x() = (base.x()+dir2.x());
			if (dir3.x()+base.x() < from.x()) from.x() = (base.x()+dir3.x());
			
			if (dir1.y()+base.y() < from.y()) from.y() = (base.y()+dir1.y());
			if (dir2.y()+base.y() < from.y()) from.y() = (base.y()+dir2.y());
			if (dir3.y()+base.y() < from.y()) from.y() = (base.y()+dir3.y());
			
			if (dir1.z()+base.z() < from.z()) from.z() = (base.z()+dir1.z());
			if (dir2.z()+base.z() < from.z()) from.z() = (base.z()+dir2.z());
			if (dir3.z()+base.z() < from.z()) from.z() = (base.z()+dir3.z());

			if (dir1.x()+base.x() > to.x()) to.x() = (base.x()+dir1.x());
			if (dir2.x()+base.x() > to.x()) to.x() = (base.x()+dir2.x());
			if (dir3.x()+base.x() > to.x()) to.x() = (base.x()+dir3.x());
			
			if (dir1.y()+base.y() > to.y()) to.y() = (base.y()+dir1.y());
			if (dir2.y()+base.y() > to.y()) to.y() = (base.y()+dir2.y());
			if (dir3.y()+base.y() > to.y()) to.y() = (base.y()+dir3.y());
			
			if (dir1.z()+base.z() > to.z()) to.z() = (base.z()+dir1.z());
			if (dir2.z()+base.z() > to.z()) to.z() = (base.z()+dir2.z());
			if (dir3.z()+base.z() > to.z()) to.z() = (base.z()+dir3.z());
			
			
			n21 = Vector3f::cross(v2,v1).normalized();
			n32 = Vector3f::cross(v3,v2).normalized();
			n13 = Vector3f::cross(v1,v3).normalized();

			
		};

		Box::~Box() { };

		

		void Box::draw() const {
			glPushMatrix();
			glTranslatef( base.x(), base.y(), base.z() );
			
			glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, primaryColor );
			glPolygonMode(GL_FRONT, GL_FILL);
			glPolygonMode(GL_BACK, GL_FILL);
			
			glEnable (GL_LIGHTING);
			glEnable(GL_CULL_FACE); // TODO: do we need this?
			
			glEnable (GL_BLEND);
			glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glMateriali( GL_FRONT, GL_SPECULAR, 30 );
		
			glMateriali(GL_FRONT, GL_SHININESS, 127);
			
			glBegin( GL_QUADS );
			Vector3f O(0,0,0);
			vertex4n(O, v2,v2+v1,v1);
			vertex4rn(O+v3, v2+v3, v2+v1+v3, v1+v3);
			vertex4n(O, v3, v3+v2,v2);
			vertex4rn(O+v1, v3+v1, v3+v2+v1, v2+v1);
			vertex4n(O, v1, v3+v1, v3);
			vertex4rn(O+v2, v1+v2, v3+v2+v1, v3+v2);
			glEnd();
			
			glPopMatrix();			
		};

		bool Box::intersectsRay(RayInfo* ri) {
/*
			Vector3f n21 = Vector3f::cross(v2,v1).normalized();
			Vector3f n32 = Vector3f::cross(v3,v2).normalized();
			Vector3f n13 = Vector3f::cross(v1,v3).normalized();
*/
			if (Vector3f::dot(n21, ri->lineDirection) < 0) {
				double is  = Vector3f::dot(n21, base-ri->startPoint)/Vector3f::dot(n21, ri->lineDirection);
				if (is > 0) {
					Vector3f ip = ri->startPoint + ri->lineDirection * is;
					float t1 = Vector3f::dot(ip-base, v1);
					if (t1 > 0 && t1 < v1.sqrLength()) {
						float t2 = Vector3f::dot(ip-base, v2);
						if ( t2 > 0 && t2 < v2.sqrLength()) {
							for (int i = 0; i < 4; i++) ri->color[i] = primaryColor[i];
							ri->intersection = is;
							ri->normal = n21;
							return true;
						}
					}
				}
			} else {
				double is  = Vector3f::dot(n21, (base+v3)-ri->startPoint)/Vector3f::dot(n21, ri->lineDirection);
				if (is > 0) {
					Vector3f ip = ri->startPoint + ri->lineDirection * is;
					float t1 = Vector3f::dot(ip-(base+v3), v1);
					if (t1 > 0 && t1 < v1.sqrLength()) {
						float t2 = Vector3f::dot(ip-(base+v3), v2);
						if ( t2 > 0 && t2 < v2.sqrLength()) {
							for (int i = 0; i < 4; i++) ri->color[i] = primaryColor[i];
							ri->intersection = is;
							ri->normal = -n21;
							return true;
						}
					}
				}
			}

			if (Vector3f::dot(n32, ri->lineDirection) < 0) {
				double is  = Vector3f::dot(n32, base-ri->startPoint)/Vector3f::dot(n32, ri->lineDirection);
				if (is > 0) {
					Vector3f ip = ri->startPoint + ri->lineDirection * is;
					float t1 = Vector3f::dot(ip-base, v2);
					if (t1 > 0 && t1 < v2.sqrLength()) {
						float t2 = Vector3f::dot(ip-base, v3);
						if ( t2 > 0 && t2 < v3.sqrLength()) {
							for (int i = 0; i < 4; i++) ri->color[i] = primaryColor[i];
							ri->intersection = is;
							ri->normal = n32;
							return true;
						}
					}
				}
			} else {
				double is  = Vector3f::dot(n32, (base+v1)-ri->startPoint)/Vector3f::dot(n32, ri->lineDirection);
				if (is > 0) {
					Vector3f ip = ri->startPoint + ri->lineDirection * is;
					float t1 = Vector3f::dot(ip-(base+v1), v2);
					if (t1 > 0 && t1 < v2.sqrLength()) {
						float t2 = Vector3f::dot(ip-(base+v1), v3);
						if ( t2 > 0 && t2 < v3.sqrLength()) {
							for (int i = 0; i < 4; i++) ri->color[i] = primaryColor[i];
							ri->intersection = is;
							ri->normal = -n32;
							return true;
						}
					}
				}
			}


			if (Vector3f::dot(n13, ri->lineDirection) < 0) {
				double is  = Vector3f::dot(n13, base-ri->startPoint)/Vector3f::dot(n13, ri->lineDirection);
				if (is > 0) {
					Vector3f ip = ri->startPoint + ri->lineDirection * is;
					float t1 = Vector3f::dot(ip-base, v3);
					if (t1 > 0 && t1 < v3.sqrLength()) {
						float t2 = Vector3f::dot(ip-base, v1);
						if ( t2 > 0 && t2 < v1.sqrLength()) {
							for (int i = 0; i < 4; i++) ri->color[i] = primaryColor[i];
							ri->intersection = is;
							ri->normal = n13;
							return true;
						}
					}
				}
			} else {
				double is  = Vector3f::dot(n13, (base+v2)-ri->startPoint)/Vector3f::dot(n13, ri->lineDirection);
				if (is > 0) {
					Vector3f ip = ri->startPoint + ri->lineDirection * is;
					float t1 = Vector3f::dot(ip-(base+v2), v3);
					if (t1 > 0 && t1 < v3.sqrLength()) {
						float t2 = Vector3f::dot(ip-(base+v2), v1);
						if ( t2 > 0 && t2 < v1.sqrLength()) {
							for (int i = 0; i < 4; i++) ri->color[i] = primaryColor[i];
							ri->intersection = is;
							ri->normal = -n13;
							return true;
						}
					}
				}
			}


			return false;
			
		}

		bool Box::intersectsAABB(SyntopiaCore::Math::Vector3f from2, SyntopiaCore::Math::Vector3f to2) {
			return
					    (from.x() < to2.x()) && (to.x() > from2.x()) &&
						(from.y() < to2.y()) && (to.y() > from2.y()) &&
						(from.z() < to2.z()) && (to.z() > from2.z());
		}
			
	}
}

