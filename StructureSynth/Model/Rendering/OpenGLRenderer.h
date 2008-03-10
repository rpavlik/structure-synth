#pragma once

#include <QString>
#include "../../../SyntopiaCore/GLEngine/EngineWidget.h"
#include "Renderer.h"

#include "../../../SyntopiaCore/Math/Vector3.h"


namespace StructureSynth {
	namespace Model {	
		namespace Rendering {

			/// A renderer implementation based on the SyntopiaCore openGL widget.
			class OpenGLRenderer : public Renderer {
			public:
				OpenGLRenderer(SyntopiaCore::GLEngine::EngineWidget* engine) : engine(engine) {};
				virtual ~OpenGLRenderer() {};

				/// The primitives
				virtual void drawBox(SyntopiaCore::Math::Vector3f base, 
					          SyntopiaCore::Math::Vector3f dir1 , 
							  SyntopiaCore::Math::Vector3f dir2, 
							  SyntopiaCore::Math::Vector3f dir3);

				virtual void drawSphere(SyntopiaCore::Math::Vector3f center, float radius);

				virtual void drawGrid(SyntopiaCore::Math::Vector3f base, 
								SyntopiaCore::Math::Vector3f dir1, 
								SyntopiaCore::Math::Vector3f dir2, 
								SyntopiaCore::Math::Vector3f dir3);

				virtual void drawLine(SyntopiaCore::Math::Vector3f from, 
										SyntopiaCore::Math::Vector3f to);

				virtual void drawDot(SyntopiaCore::Math::Vector3f pos);

				virtual void begin();
				virtual void end();
				
				virtual void setColor(SyntopiaCore::Math::Vector3f rgb) { this->rgb = rgb; }
				virtual void setBackgroundColor(SyntopiaCore::Math::Vector3f rgb);
				virtual void setAlpha(double alpha) { this->alpha = alpha; }

				// Issues a command for a specific renderclass such as 'template' or 'opengl'
				virtual void callCommand(const QString& renderClass, const QString& command);
			private:
				SyntopiaCore::GLEngine::EngineWidget* engine;
				SyntopiaCore::Math::Vector3f rgb;
				double alpha;
			};

		}
	}
}

