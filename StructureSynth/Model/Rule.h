#pragma once

#include "Rendering/Renderer.h"
#include "State.h"

namespace StructureSynth {
	namespace Model {	

		struct ExecutionStack; // forward declaration.
		class RuleRef; // forward decl.
		class Builder; // forward decl.

		/// (Abstract) Base class for rules.
		class Rule {
		public:
			/// Every rule must have a name.
			Rule(QString name) : name(name) {};
			Rule() {};

			QString getName() { return name; }

			/// When applied the rule will add new pending rules to the ExecutionStack for execution.
			/// Only PrimitiveRules will make use of the renderer.
			virtual void apply(Builder* builder) = 0;

			/// Returns a list over rules that this rule references.
			virtual QList<RuleRef*> getRuleRefs() { return QList<RuleRef*>(); }
			
		protected:
			QString name;
		};
	}
}