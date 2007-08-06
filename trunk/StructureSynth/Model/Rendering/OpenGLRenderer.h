#pragma once

#include <QString>
#include "../../../AppCore/GLEngine/EngineWidget.h"
#include "Renderer.h"

#include "../../../AppCore/Math/Vector3.h"


namespace StructureSynth {
	namespace Model {	
		namespace Rendering {

			/// A renderer implementation based on the AppCore openGL widget.
			class OpenGLRenderer : public Renderer {
			public:
				OpenGLRenderer(AppCore::GLEngine::EngineWidget* engine) : engine(engine) {};
				virtual ~OpenGLRenderer() {};

				/// The primitives
				virtual void drawBox(AppCore::Math::Vector3f base, 
					          AppCore::Math::Vector3f dir1 , 
							  AppCore::Math::Vector3f dir2, 
							  AppCore::Math::Vector3f dir3);

				virtual void drawSphere(AppCore::Math::Vector3f center, float radius);

				virtual void begin();
				virtual void end();
				

			private:
				AppCore::GLEngine::EngineWidget* engine;
			};

		}
	}
}