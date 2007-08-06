#pragma once

#include "Rule.h"

namespace StructureSynth {
	namespace Model {	

		/// Container for all rules.
		class RuleSet {
			public:
				/// Constructor. Automatically adds built-in rules.
				RuleSet();

				/// Deletes rules
				~RuleSet();

				/// Added rules belong to the RuleSet and will be deleted by the RuleSet destructor.
				void addRule(Rule* rule);

				/// Resolve symbolic names into pointers
				void resolveNames();

				///
				QStringList getUnreferencedNames();

				/// The first rule parsed will be the start-rule.
				/// Notice that rule loaded as part of a preprocessor '#include' statement will not be start-rules
				Rule* getStartRule();

				/// For debug
				void dumpInfo();

		private:
			 QList<Rule*> rules;
			 Rule* startRule;
		};

	}
}