#include "AmbiguousRule.h"

#include "Builder.h"

namespace StructureSynth {
	namespace Model {	

		QList<RuleRef*> AmbiguousRule::getRuleRefs() {
			QList<RuleRef*>  list;
			for (int i = 0; i < rules.size(); i++) {
				for (int j = 0; j < rules[i]->getRuleRefs().size(); j++) {
					list.append(rules[i]->getRuleRefs()[j]);
				}
			}
			return list;
		}
	}
}