#include "Action.h"
#include "ExecutionStack.h"
#include "Builder.h"

#include "../../AppCore/Logging/Logging.h"

using namespace AppCore::Logging;

namespace StructureSynth {
	namespace Model {	
		void  Action::apply(Builder* b, Rule* callingRule, int ruleDepth) {

			
			if (set != 0) {
				b->setCommand(set->key, set->value);
				return;
			}

			//INFO(QString("Applying action -> %1").arg(rule->rule()->getName()));

			State s = b->getState();

			QList<int> counters;
			for (int i = 0; i < loops.size(); i++) counters.append(1);

			if (counters.size() == 0) {
				if (callingRule) {
					s.maxDepths[callingRule] = ruleDepth;
				}
				b->getExecutionStack().currentStack.append(RuleState(rule->rule(), s));
				return;
			}

			bool done = false;
			while (!done) {

				// create state
				State s0 = s;
				for (int i = 0; i < counters.size(); i++) {
					for (int j = 0; j < counters[i]; j++) {
						s0 = loops[i].transformation.apply(s0);
					}
				}
				if (callingRule) {
					s0.maxDepths[callingRule] = ruleDepth;
				}
				b->getExecutionStack().currentStack.append(RuleState(rule->rule(), s0));

				// increase lowest counter...
				counters[0]++;
				for (int i = 0; i < counters.size(); i++) {
					if (counters[i] > loops[i].repetitions) {
						if (i == counters.size()-1) {
							done = true;
						} else {
							counters[i] = 1;
							counters[i+1]++;
						}
					}
				}

				//if (done) break;

			}

		
/*
			for (int i = 0; i < loops.size(); i++) {
				int rep = loops[i].repetitions;
				for (int j = 0; j < rep; j++) {
					s = loops[i].transformation.apply(s);
					if (callingRule) {
						s.maxDepths[callingRule] = ruleDepth;
					}
					b->getExecutionStack().currentStack.append(RuleState(rule->rule(), s));
				}
			}
*/
			if (loops.size() == 0) {
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


		void Action::addTransformationLoop(TransformationLoop tl) { 
			loops.append(tl); 
		}

		void Action::setRule(QString ruleName) {
			rule = new RuleRef(ruleName);
			set = 0;
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