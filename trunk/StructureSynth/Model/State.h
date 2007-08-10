#pragma once

#include <QString>
#include <QMap>
#include "../../AppCore/Math/Matrix4.h"

namespace StructureSynth {
	namespace Model {	

		class Rule; // Forward

		/// A state represent the current rendering projection matrix and other rendering settings.
		struct State {
			State();

			AppCore::Math::Matrix4f matrix;

			// TODO:
			// - Alpha , Coloring state

			QMap<Rule*, int> maxDepths;
		};


	}
}