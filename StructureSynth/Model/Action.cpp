#include "Action.h"
#include "ExecutionStack.h"
#include "Builder.h"


namespace StructureSynth {
	namespace Model {	
		void  Action::apply(Builder* b) {

			if (set != 0) {
				b->setCommand(set->key, set->value);
				return;
			}

			State s = b->getState();

			for (int i = 0; i < loops.size(); i++) {
				int rep = loops[i].repetitions;
				for (int j = 0; j < rep; j++) {
					s = loops[i].transformation.apply(s);
					b->getExecutionStack().currentStack.append(RuleState(rule->rule(), s));
				}
			}

			if (loops.size() == 0) {
				b->getExecutionStack().currentStack.append(RuleState(rule->rule(), s));
			}
		}

		Action::Action(QString key, QString value) {
			set = new SetAction();
			set->key = key;
			set->value = value;
			rule = 0;
		}

		Action::~Action() {
			// TODO: Handle leaks (Actions are treated as value types, and hence rule,set ptrs are duped)
			//delete(rule);
			//delete(set);
		}
			

		Action::Action(Transformation t, QString ruleName) {
			loops.append(TransformationLoop(1, t));
			rule = new RuleRef(ruleName);
			set = 0;
		}

		Action::Action(QString ruleName) {
			rule = new RuleRef(ruleName);
			set = 0;
		}

	}
}