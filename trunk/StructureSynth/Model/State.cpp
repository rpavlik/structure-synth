#include "State.h"


namespace StructureSynth {
	namespace Model {	

		State::State() : 
			matrix(SyntopiaCore::Math::Matrix4f::Identity()), 
			hsv(SyntopiaCore::Math::Vector3f(0,1.0f,1.0f)), 
			alpha(1.0f), prevMatrix(0)  { 
			
		}

		State& State::operator=(const State& rhs){
			this->matrix = rhs.matrix;
			this->hsv = rhs.hsv;
			this->alpha = rhs.alpha;
			this->maxDepths = rhs.maxDepths;
			if (rhs.prevMatrix) {
				this->prevMatrix = new SyntopiaCore::Math::Matrix4f();
				*(this->prevMatrix) = *rhs.prevMatrix;
			} else {
				this->prevMatrix = 0;
			}
			return *this;
		}

		void State::setPrevMatrix(SyntopiaCore::Math::Matrix4f matrix) {
			if (prevMatrix) {delete (prevMatrix); }

			this->prevMatrix = new SyntopiaCore::Math::Matrix4f();
			*(this->prevMatrix) = matrix;
		}

		
		State::State(const State& rhs) : matrix(rhs.matrix), 
			hsv(rhs.hsv), 
			alpha(rhs.alpha), prevMatrix(0), maxDepths(rhs.maxDepths) {
			if (rhs.prevMatrix) {
				this->prevMatrix = new SyntopiaCore::Math::Matrix4f();
				*(this->prevMatrix) = *rhs.prevMatrix;
			} else {
				this->prevMatrix = 0;
			}
		}
		


		State::~State() { 
			delete(prevMatrix);
		}
	}
}

