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

				virtual void drawGrid(AppCore::Math::Vector3f base, 
								AppCore::Math::Vector3f dir1, 
								AppCore::Math::Vector3f dir2, 
								AppCore::Math::Vector3f dir3);

				virtual void drawLine(AppCore::Math::Vector3f from, 
										AppCore::Math::Vector3f to);

				virtual void drawDot(AppCore::Math::Vector3f pos);

				virtual void begin();
				virtual void end();
				
				virtual void setColor(AppCore::Math::Vector3f rgb) { this->rgb = rgb; }
				virtual void setBackgroundColor(AppCore::Math::Vector3f rgb);
				virtual void setAlpha(double alpha) { this->alpha = alpha; }
			private:
				AppCore::GLEngine::EngineWidget* engine;
				AppCore::Math::Vector3f rgb;
				double alpha;
			};

		}
	}
}