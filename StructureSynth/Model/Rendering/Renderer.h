#pragma once

#include <QString>
#include "../../../AppCore/Math/Vector3.h"

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
				virtual void drawBox(AppCore::Math::Vector3f base, 
								AppCore::Math::Vector3f dir1, 
								AppCore::Math::Vector3f dir2, 
								AppCore::Math::Vector3f dir3) = 0;

				virtual void drawGrid(AppCore::Math::Vector3f base, 
								AppCore::Math::Vector3f dir1, 
								AppCore::Math::Vector3f dir2, 
								AppCore::Math::Vector3f dir3) = 0;

				virtual void drawLine(AppCore::Math::Vector3f from, 
										AppCore::Math::Vector3f to) = 0;

				virtual void drawDot(AppCore::Math::Vector3f pos) = 0;
				
				virtual void drawSphere(AppCore::Math::Vector3f center, float radius) = 0;
		};

		}
	}
}