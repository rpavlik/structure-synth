#include "State.h"


namespace StructureSynth {
	namespace Model {	

		State::State() { 
			// Default statae
			matrix = SyntopiaCore::Math::Matrix4f::Identity();
			hsv =  SyntopiaCore::Math::Vector3f(0,1.0f,1.0f);
			alpha = 1.0f;
		}
	}
}

