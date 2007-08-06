#include "State.h"


namespace StructureSynth {
	namespace Model {	

		State::State() { 
			// Default statae
			matrix = AppCore::Math::Matrix4f::Identity();
		}
	}
}