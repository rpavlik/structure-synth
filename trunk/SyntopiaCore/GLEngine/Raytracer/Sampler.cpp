#include "Sampler.h"

namespace SyntopiaCore {
	namespace GLEngine {

		Vector3f StratifiedSampler::getAODirection() {
			if (aoSampleIndex>=aoSamples.count()) throw 1;
			return aoSamples[aoSampleIndex++];
		}

		Vector3f StratifiedSampler::getLensSample() {
			return rg->getUniform2D();
		}

		// Based on the Physical Based Rendering book
		Vector3f StratifiedSampler::sampleSphere(double u1, double u2) {
			double z = 1.0 - 2.0*u1;
			double r = 0;
			if (1.0-z*z > 0) r = sqrt(1.0-z*z);
			double phi = 2.0 * 3.1415926 * u2;
			double x = r * cos(phi);
			double y = r * sin(phi);
			return Vector3f(x,y,z);
		}

		void StratifiedSampler::prepareSamples(int nAASamplesSqrt, int nAOSamplesSqrt) {
			int nSqrt = nAASamplesSqrt*nAOSamplesSqrt;
			aoSamples = QVector<Vector3f>(nSqrt*nSqrt);
			int count = 0;
			for (int i = 0; i < nSqrt; i++) {
				for (int j = 0; j < nSqrt; j++) {
					// we need a uniform number in the interval
					// [i/nSqrt;(i+1)/nSqrt]
					double x = rg->getDouble( ((double)i)/(double)nSqrt,((double)(i+1.0))/(double)nSqrt);
					double y = rg->getDouble( ((double)j)/(double)nSqrt,((double)(j+1.0))/(double)nSqrt);
					aoSamples[count++] = sampleSphere(x,y);
				}	
			}
			aoSampleIndex = 0;
		};

	}
}

