#pragma once

#include "SyntopiaCore/Math/Random.h"
#include "SyntopiaCore/Math/Vector3.h"

namespace SyntopiaCore {
	namespace GLEngine {	

		using namespace SyntopiaCore::Math;

		// A simple sampler. Draws uniform numbers, but no stratification.
		class Sampler {
		public:
			Sampler(Math::RandomNumberGenerator* rg) : rg(rg) {}
			virtual Vector3f getAODirection() { return rg->getUniform3D(); }
			virtual Vector3f getLensSample() { return rg->getUniform2D(); }
			virtual void prepareSamples(int /*nSamplesSqrt*/, int /*nAOSamplesSqrt*/) {};
		protected:
			Math::RandomNumberGenerator* rg;
		};

		// Stratified sampling
		class StratifiedSampler : public Sampler {
		public:
			StratifiedSampler(Math::RandomNumberGenerator* rg) : Sampler(rg), aoSampleIndex(0) {}

			virtual Vector3f getAODirection();
			virtual Vector3f getLensSample();
			Vector3f sampleSphere(double u1, double u2);
			virtual void prepareSamples(int nSamplesSqrt, int nAOSamplesSqrt);

		private:
			int aoSampleIndex;
			QVector<Vector3f> aoSamples;
			QVector<Vector3f> lensSamples;
		};


	}
}

