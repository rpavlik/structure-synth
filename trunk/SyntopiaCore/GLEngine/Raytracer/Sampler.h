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
			virtual Vector3f getAASample(int /*index*/) { return Vector3f(rg->getDouble(-0.5,0.5), rg->getDouble(-0.5,0.5),1.0); }
			virtual Vector3f getAODirection(int /*index*/) { return rg->getUniform3D(); }
			virtual Vector3f getLensSample(int /*index*/) { return rg->getUniform2D(); }
			virtual void prepareSamples(int /*nSamplesSqrt*/, int /*nAOSamplesSqrt*/) {};
			virtual Sampler* clone(Math::RandomNumberGenerator* rg) { return new Sampler(rg); }
		protected:
			Math::RandomNumberGenerator* rg;
		};

		// Stratified sampling
		class StratifiedSampler : public Sampler {
		public:
			StratifiedSampler(Math::RandomNumberGenerator* rg) : Sampler(rg) {};
			virtual Vector3f getAASample(int index);
			virtual Vector3f getAODirection(int index);
			virtual Vector3f getLensSample(int index);
			Vector3f sampleSphere(double u1, double u2);
			virtual void prepareSamples(int nSamplesSqrt, int nAOSamplesSqrt);
			virtual Sampler* clone(Math::RandomNumberGenerator* rg) { return new StratifiedSampler(rg); }

		private:
			QVector<Vector3f> aoSamples;
			QVector<Vector3f> aaSamples;
			QVector<Vector3f> lensSamples;
		};

		// Stratified sampling
		class ProgressiveStratifiedSampler : public Sampler {
		public:
			ProgressiveStratifiedSampler(Math::RandomNumberGenerator* rg) : 
			  Sampler(rg) {}
			virtual Vector3f getAASample(int index);
			virtual Vector3f getAODirection(int index);
			virtual Vector3f getLensSample(int index);
			Vector3f sampleSphere(double u1, double u2);
			virtual void prepareSamples(int nSamplesSqrt, int nAOSamplesSqrt);
			virtual Sampler* clone(Math::RandomNumberGenerator* rg);
			void setAAOrder(QVector<int> aaOrder) { this->aaOrder = aaOrder; }
		private:
			int aoSamplesSqrt;
			int aaSamplesSqrt;
			QVector<int> aaOrder;
			
		};


	}
}

