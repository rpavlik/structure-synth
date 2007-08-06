#include "Builder.h"
#include "../../AppCore/Logging/Logging.h"
#include "../../AppCore/Exceptions/Exception.h"

using namespace AppCore::Logging;
using namespace AppCore::Exceptions;

namespace StructureSynth {
	namespace Model {

		Builder::Builder(Rendering::Renderer* renderTarget, RuleSet* ruleSet) : renderTarget(renderTarget), ruleSet(ruleSet) {
			maxGenerations = 5;
		};
			

		void Builder::build() {
			INFO("Starting build...");
			
			/// Push first generation state
			stack.currentStack.append(RuleState(ruleSet->getStartRule(), State()));

			INFO("Rule set first rule: " + ruleSet->getStartRule()->getName());
					

			int generationCounter = 0;
			
			while (generationCounter < maxGenerations) {
				generationCounter++;

				// Now iterate though all RuleState's on stack and create next generation.
				//INFO(QString("Executing generation %1 with %2 individuals").arg(generationCounter).arg(stack.currentStack.size()));
				nextStack.currentStack.clear();
				for (int i = 0; i < stack.currentStack.size(); i++) {
				//	INFO("Executing: " + stack.currentStack[i].rule->getName());
					state = stack.currentStack[i].state;
					stack.currentStack[i].rule->apply(this);
				}
				stack = nextStack;
			}

			INFO("Done building...");
		}

		void Builder::setCommand(QString command, QString param) {
			if (command == "maxgenerations") {
				//INFO(QString("Setting 'maxgenerations' to %1").arg(param));
				bool succes;
				int i = param.toInt(&succes);
				if (!succes) throw Exception(QString("Command 'maxgenerations' expected integer parameter. Found: %1").arg(param));
				maxGenerations = i;
			} else {
				throw Exception(QString("Unknown command: %1").arg(command));
			}
		}
		
		ExecutionStack& Builder::getExecutionStack() {
			return nextStack;
		}

	}
}