#pragma once

#include <QMutex>
#include <QImage>
#include "SyntopiaCore/Math/Vector3.h"

namespace SyntopiaCore {
	namespace GLEngine {	

		using namespace SyntopiaCore::Math;
	
		class ProgressiveOutput {
		public:
			ProgressiveOutput(int w, int h) : w(w), h(h) {
				colors = new Vector3f[w*h];
				weights = new double[w*h];
				for (int x = 0; x < w; x++) {	
					for (int y = 0; y < h; y++) {
						colors[x+y*w] = Vector3f(0,0,0);
						weights[x+y*w] = 0.0;
					}
				}
			}

			~ProgressiveOutput() {
				delete[] colors;
				delete[] weights;
			}

			void addIteration(Vector3f* newColors, double* newWeights) {
				mutex.lock();
				for (int x = 0; x < w; x++) {	
					for (int y = 0; y < h; y++) {	
						colors[x+y*w] = colors[x+y*w] + newColors[x+y*w];
						weights[x+y*w] = weights[x+y*w] + newWeights[x+y*w];
					}
				}
				mutex.unlock();
			};

			QImage getImage() {
				QImage im(w,h, QImage::Format_RGB32);

				mutex.lock();
				for (int x = 0; x < w; x++) {	
					for (int y = 0; y < h; y++) {	
						Vector3f c = colors[x+y*w]/weights[x+y*w];
						im.setPixel(x,y,qRgb(c.x()*255, c.y()*255, c.z()*255));
					}
				}
				mutex.unlock();
				return im;
			}

		private:
			int w;
			int h;
			Vector3f* colors;
			double* weights;
			QMutex mutex;
		};

	}
}

