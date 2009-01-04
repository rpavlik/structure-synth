#include "Mesh.h"

#include "SyntopiaCore/Math/Vector3.h"

#include "../Logging/Logging.h"

using namespace SyntopiaCore::Logging;


using namespace SyntopiaCore::Math;

namespace SyntopiaCore {
	namespace GLEngine {


		Mesh::Mesh(SyntopiaCore::Math::Vector3f startBase, 
				SyntopiaCore::Math::Vector3f startDir1 , 
				 SyntopiaCore::Math::Vector3f startDir2, 
				 SyntopiaCore::Math::Vector3f endBase, 
				SyntopiaCore::Math::Vector3f endDir1 , 
				 SyntopiaCore::Math::Vector3f endDir2) : 
				startBase(startBase), startDir1(startDir1), startDir2(startDir2),
				endBase(endBase), endDir1(endDir1), endDir2(endDir2)
		{
			/// Bounding Mesh (not really accurate)
			from = startBase;
			to = endBase;

		};

		Mesh::~Mesh() { };

		void Mesh::draw() const {

			// --- TODO: Rewrite - way to many state changes...

			glPushMatrix();
			glTranslatef( startBase.x(), startBase.y(), startBase.z() );
			

			GLfloat col[4] = {0.0f, 1.0f, 1.0f, 0.1f} ;
			glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, col );
			

			glPolygonMode(GL_FRONT, GL_FILL);
			glPolygonMode(GL_BACK, GL_FILL);
				
			Vector3f O(0,0,0);
			Vector3f end = endBase - startBase;
			
			glEnable (GL_LIGHTING);

			glDisable(GL_CULL_FACE); // TODO: do we need this?
			glEnable (GL_BLEND);
			glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			GLfloat c3[4] = {0.3f, 0.3f, 0.3f, 0.5f};
			glMaterialfv( GL_FRONT, GL_SPECULAR, c3 );
			glMateriali(GL_FRONT, GL_SHININESS, 127);
			glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
			glEnable(GL_COLOR_MATERIAL);
			
					
			glBegin( GL_QUADS );
			glColor4fv(primaryColor);
			//vertex4n(O, startDir1,startDir2+startDir1,startDir2);
			vertex4n(O, startDir1,end+endDir1,end);
			vertex4n(O, startDir2,end+endDir2,end);
			vertex4n(startDir1, startDir1+startDir2, end+endDir1+endDir2, end+endDir1);
			vertex4n(startDir2, startDir1+startDir2, end+endDir1+endDir2, end+endDir2);
			//vertex4n(O+end, endDir1+end,endDir2+endDir1+end,endDir2+end);
			glEnd();
			
			glDisable(GL_COLOR_MATERIAL);
			

			glPopMatrix();			
		};

	}
}

