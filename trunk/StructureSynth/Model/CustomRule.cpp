#include "CustomRule.h"

#include "../../AppCore/Logging/Logging.h"
#include "Builder.h"

using namespace AppCore::Logging;

namespace StructureSynth {
	namespace Model {	

		CustomRule::CustomRule(QString name) : Rule(name) {
			weight = 1.0;
		}

		void CustomRule::apply(Builder* b) {
			/// Apply all actions.
			//INFO(QString("Applying Rule '%1' with %2 actions...").arg(getName()).arg(actions.size()));
			for (int i = 0; i < actions.size(); i++) {
				actions[i].apply(b);
			}
		}

		QList<RuleRef*> CustomRule::getRuleRefs() {
			QList<RuleRef*>  list;
			for (int i = 0; i < actions.size(); i++) {
				RuleRef* a = actions[i].getRuleRef();
				if (a) list.append(a);
			}
			return list;
		}
	

	}
}