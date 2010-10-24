#include "Sampler.h"

namespace SyntopiaCore {
	namespace GLEngine {

		Vector3f StratifiedSampler::getAODirection(int index) {
			if (aoSampleIndex>=aoSamples.count()) throw 1;
			//return aoSamples[aoSampleIndex++];
			return aoSamples[index];
		}

		Vector3f StratifiedSampler::getAASample(int index) {
			if (aaSampleIndex>=aaSamples.count()) throw 1;
			return aaSamples[aaSampleIndex++];
		}

		Vector3f StratifiedSampler::getLensSample(int index) {
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
			aaSamples = QVector<Vector3f>(nAASamplesSqrt*nAASamplesSqrt);
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

			count = 0;
			for (int i = 0; i < nAASamplesSqrt; i++) {
				for (int j = 0; j < nAASamplesSqrt; j++) {
					// we need a uniform number in the interval
					// [i/nSqrt;(i+1)/nSqrt]
					double x = rg->getDouble( ((double)i)/(double)nAASamplesSqrt,((double)(i+1.0))/(double)nAASamplesSqrt);
					double y = rg->getDouble( ((double)j)/(double)nAASamplesSqrt,((double)(j+1.0))/(double)nAASamplesSqrt);
					aaSamples[count++] = Vector3f(x-0.5,y-0.5,1);
				}	
			}

			aoSampleIndex = 0;
			aaSampleIndex = 0;
			lensSampleIndex = 0;

		};


		/// --- Progressive versions...

		Vector3f ProgressiveStratifiedSampler::getAODirection(int index) {
			if (index>=aoSamplesSqrt*aoSamplesSqrt) throw 1;
			int i = index / aoSamplesSqrt;
			int j = index % aoSamplesSqrt;
			double x = rg->getDouble( ((double)i)/(double)aoSamplesSqrt,((double)(i+1.0))/(double)aoSamplesSqrt);
			double y = rg->getDouble( ((double)j)/(double)aoSamplesSqrt,((double)(j+1.0))/(double)aoSamplesSqrt);		
			return sampleSphere(x,y);
		}

		Vector3f ProgressiveStratifiedSampler::getAASample(int index) {
			if (index>=(aaSamplesSqrt*aaSamplesSqrt)) throw 1;
			int i = index / aaSamplesSqrt;
			int j = index % aaSamplesSqrt;
			double x = rg->getDouble( ((double)i)/(double)aaSamplesSqrt,((double)(i+1.0))/(double)aaSamplesSqrt);
			double y = rg->getDouble( ((double)j)/(double)aaSamplesSqrt,((double)(j+1.0))/(double)aaSamplesSqrt);
			return Vector3f(x-0.5,y-0.5,1);
		}

		Vector3f ProgressiveStratifiedSampler::getLensSample(int index) {
			return rg->getUniform2D();
		}

		// Based on the Physical Based Rendering book
		Vector3f ProgressiveStratifiedSampler::sampleSphere(double u1, double u2) {
			double z = 1.0 - 2.0*u1;
			double r = 0;
			if (1.0-z*z > 0) r = sqrt(1.0-z*z);
			double phi = 2.0 * 3.1415926 * u2;
			double x = r * cos(phi);
			double y = r * sin(phi);
			return Vector3f(x,y,z);
		}

		void ProgressiveStratifiedSampler::prepareSamples(int nAASamplesSqrt, int nAOSamplesSqrt) {
			this->aoSamplesSqrt = nAASamplesSqrt*nAOSamplesSqrt;
			this->aaSamplesSqrt = nAASamplesSqrt;
		};


	}
}

