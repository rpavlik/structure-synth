#pragma once

#include "Rule.h"
#include "CustomRule.h"

namespace StructureSynth {
	namespace Model {	


		/// If several definitions for the same rule exists,
		/// an Ambiguous Rule is created which contains the multiple definitions.
		///
		/// When the rule is executed, a random rule is chosen from the multiple definitins,
	    /// taking their weigths into account.
		class AmbiguousRule : public Rule {
		public:
			AmbiguousRule(QString name) : Rule(name) {};

			virtual void apply(Builder* builder) const;
	
			/// Returns a list over rules that this rule references.
			virtual QList<RuleRef*> getRuleRefs() const;

			QList<CustomRule*> getRules() { return rules; };
			
			void appendRule(CustomRule* r) { rules.append(r); }
		private:
			QList<CustomRule*> rules;
		};


	}
}

