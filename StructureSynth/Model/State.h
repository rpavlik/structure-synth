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

			SyntopiaCore::Math::Matrix4f matrix;

			QMap<const Rule*, int> maxDepths;
			SyntopiaCore::Math::Vector3f hsv;
			float alpha;
		};


	}
}