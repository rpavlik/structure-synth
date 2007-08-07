#pragma once

#include "Rule.h"
#include "Action.h"

namespace StructureSynth {
	namespace Model {	

		/// A custom rule is a user defined rule.
		/// It consist of a number of actions, 
		/// and a weight that is used if the rule definition is ambiguous (see 'AmbiguousRule').
		class CustomRule : public Rule {
		public:
			CustomRule(QString name);
			virtual void  apply(Builder* builder);

			/// Returns a list over rules that this rule references.
			virtual QList<RuleRef*> getRuleRefs();

			void appendAction(Action a) { actions.append(a); }

			double getWeight() { return weight; }
			void setWeight(double w) { weight = w; }

		private:
			QList<Action> actions;
			double weight;
		};

	}
}