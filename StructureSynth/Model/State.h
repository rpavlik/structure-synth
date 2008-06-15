#pragma once

#include <QString>
#include <QMap>
#include "../../SyntopiaCore/Math/Matrix4.h"

namespace StructureSynth {
	namespace Model {	

		class Rule; // Forward

		/// A state represent the current rendering projection matrix and other rendering settings.
		struct State {
			State();
			State(const State& rhs);
			~State();

			State& operator=(const State& rhs);

			SyntopiaCore::Math::Matrix4f* prevMatrix; // previous matrix (if stored)
			SyntopiaCore::Math::Matrix4f matrix; // Transformation matrix (4x4 homogenous representation)
			void setPrevMatrix(SyntopiaCore::Math::Matrix4f matrix);

			QMap<const Rule*, int> maxDepths;    // Rules may have a max. recursion depth before they are retired. 
												 // We need to keep track of this in the state.
			SyntopiaCore::Math::Vector3f hsv;    // Hue, Saturation, Value colorspace state
			float alpha;                         // Transparency 
		};


	}
}

