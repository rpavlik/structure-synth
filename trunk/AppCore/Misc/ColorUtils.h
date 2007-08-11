#pragma once

#include <QString>
#include <QVector>
#include <cmath>
#include "../Math/Vector3.h"

namespace AppCore {
	namespace Misc {	
	
		
		class ColorUtils {
		public:
			static AppCore::Math::Vector3f HSVtoRGB(AppCore::Math::Vector3f hsv);
		};

		
	}
}