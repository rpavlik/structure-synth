#pragma once

#include "Rule.h"

namespace StructureSynth {
	namespace Model {	

		/// These are the built-in primitives,
		/// for drawing boxes, spheres and other simple geometric shapes.
		class PrimitiveRule : public Rule {
			public:
				enum PrimitiveType { Box, Sphere, Point, Cylinder, Line } ;
				
				PrimitiveRule(PrimitiveType type);
				virtual void apply(Builder* builder);
	
				/// Returns a list over rules that this rule references.
				/// (Empty for all PrimitiveRules!)
				virtual QList<RuleRef*> getRuleRefs() { return QList<RuleRef*>(); }

			private:
				PrimitiveType type;
		};

	}
}