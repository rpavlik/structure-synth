#pragma once

#include <QString>
#include "../../../SyntopiaCore/Math/Vector3.h"

namespace StructureSynth {
	namespace Model {	
		namespace Rendering {

		/// Abstract base class for implementing a renderer
		class Renderer {
			public:
				Renderer() {};
				virtual ~Renderer() {};

				/// Flow
				virtual void begin() {};
				virtual void end() {};

				/// The primitives
				virtual void drawBox(SyntopiaCore::Math::Vector3f base, 
								SyntopiaCore::Math::Vector3f dir1, 
								SyntopiaCore::Math::Vector3f dir2, 
								SyntopiaCore::Math::Vector3f dir3) = 0;

				virtual void drawGrid(SyntopiaCore::Math::Vector3f base, 
								SyntopiaCore::Math::Vector3f dir1, 
								SyntopiaCore::Math::Vector3f dir2, 
								SyntopiaCore::Math::Vector3f dir3) = 0;

				virtual void drawLine(SyntopiaCore::Math::Vector3f from, 
										SyntopiaCore::Math::Vector3f to) = 0;

				virtual void drawDot(SyntopiaCore::Math::Vector3f pos) = 0;
				
				virtual void drawSphere(SyntopiaCore::Math::Vector3f center, float radius) = 0;

				// Color
				// RGB in [0;1] intervals.
				virtual void setColor(SyntopiaCore::Math::Vector3f rgb) = 0;
				virtual void setBackgroundColor(SyntopiaCore::Math::Vector3f rgb) = 0;
				virtual void setAlpha(double alpha) = 0;
		};

		}
	}
}

