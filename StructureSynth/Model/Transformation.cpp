#include "Transformation.h"

#include "../../AppCore/Math/Matrix4.h"

#include "../../AppCore/Exceptions/Exception.h"
#include "../../AppCore/Logging/Logging.h"

using namespace AppCore::Exceptions;
using namespace AppCore::Logging;

using namespace AppCore::Math;

namespace StructureSynth {
	namespace Model {	

		Transformation::Transformation() {
			matrix = Matrix4f::Identity();
		}

		Transformation::~Transformation() {
		};

		State Transformation::apply(const State& s) const {
			State s2(s);
			s2.matrix = s.matrix*matrix; // TODO: Check order
			return s2;
		}


			
		void Transformation::append(const Transformation& t) {
			this->matrix = this->matrix * t.matrix; // TODO: Check order
		}

		// The predefined operators
		// Translations
		Transformation Transformation::createX(double offset) {
			Transformation t;
			t.matrix(0,3) = offset;
			INFO("CreateX:" + t.matrix.toString() + " " + QString("%1").arg(offset));
			return t;
		}
		
		Transformation Transformation::createY(double offset) {
			Transformation t;
			t.matrix(1,3) = offset;
			return t;
		}

		Transformation Transformation::createZ(double offset) {
			Transformation t;
			t.matrix(2,3) = offset;
			return t;
		}
			
		// Rotations
		Transformation Transformation::createRX(double angle) {
			Transformation t;
			t.matrix = 
				Matrix4f::Translation(0,0.5,0.5)*
				Matrix4f::Rotation(Vector3f(1,0,0), angle)*
				Matrix4f::Translation(0,-0.5,-0.5);
			return t;
		}

		Transformation Transformation::createRY(double angle) {
			Transformation t;
			t.matrix = 
				Matrix4f::Translation(0.5,0,0.5)*
				Matrix4f::Rotation(Vector3f(0,1,0), angle)*
				Matrix4f::Translation(-0.5,0,-0.5)
				
				; 
			return t;
		}

		Transformation Transformation::createRZ(double angle) {
			Transformation t;
			t.matrix = 
				Matrix4f::Translation(0.5,0.5,0)*
				Matrix4f::Rotation(Vector3f(0,0,1), angle)*
				Matrix4f::Translation(-0.5,-0.5,0);
			return t;
		}

		Transformation Transformation::createScale(double x, double y, double z) {
			Transformation t;
			t.matrix(0,0) = x;
			t.matrix(1,1) = y;
			t.matrix(2,2) = z;
			t.matrix = 
				Matrix4f::Translation(0.5,0.5,0.5)*
				t.matrix*
				Matrix4f::Translation(-0.5,-0.5,-0.5);
			return t;
		}
	}
}