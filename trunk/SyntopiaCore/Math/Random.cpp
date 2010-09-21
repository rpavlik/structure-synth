  #include "Random.h"


namespace SyntopiaCore {
	namespace Math {	

		Vector3f RandomNumberGenerator::getUniform2D() {
			Vector3f v;
			do {
				v = Vector3f(getDouble(-1,1),getDouble(-1,1),0);
			} while (v.sqrLength()>1.0);
			return v;
		}
		
		Vector3f RandomNumberGenerator::getUniform3D() {
			Vector3f v;
			do {
				v = Vector3f(getDouble(-1,1),getDouble(-1,1),getDouble(-1,1));
			} while (v.sqrLength()>1.0);
			return v;
		}

		namespace {
			const int UniformTableSize = 10000; // size of precalculated tables.					
		}

		// Uses precalculated tables.
		// Initialized on first use (so init before using in multithreaded code)
		Vector3f RandomNumberGenerator::getUniform2DFromTable() {
			static QVector<Vector3f> uniform2D;
			if (uniform2D.count() == 0) {
				for (int i = 0; i < UniformTableSize; i++) uniform2D.append(getUniform2D());
			}
			uniformCounter2D++;
			if (uniformCounter2D>=uniform2D.count()) uniformCounter2D = 0;
			return uniform2D[uniformCounter2D];
		}

		Vector3f RandomNumberGenerator::getUniform3DFromTable() {
			static QVector<Vector3f> uniform3D;
			if (uniform3D.count() == 0) {
				for (int i = 0; i < UniformTableSize; i++) uniform3D.append(getUniform3D());
			}
			uniformCounter3D++;
			if (uniformCounter3D>=uniform3D.count()) uniformCounter3D = 0;
			return uniform3D[uniformCounter3D];
		}

		void RandomNumberGenerator::randomizeUniformCounter(){
			uniformCounter2D = getInt(UniformTableSize);
			uniformCounter3D = getInt(UniformTableSize);
		}

	}
}

