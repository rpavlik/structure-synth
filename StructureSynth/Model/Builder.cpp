#include "Builder.h"
#include "../../AppCore/Logging/Logging.h"
#include "../../AppCore/Exceptions/Exception.h"

using namespace AppCore::Logging;
using namespace AppCore::Exceptions;

namespace StructureSynth {
	namespace Model {

		Builder::Builder(Rendering::Renderer* renderTarget, RuleSet* ruleSet) : renderTarget(renderTarget), ruleSet(ruleSet) {
			maxGenerations = 1000;
			maxObjects = 100000;
			objects = 0;
		};
			

		void Builder::build() {
			objects = 0;
			INFO("Starting build...");
			
			/// Push first generation state
			stack.currentStack.append(RuleState(ruleSet->getStartRule(), State()));

			INFO("Rule set first rule: " + ruleSet->getStartRule()->getName());
					

			int generationCounter = 0;
			
			while (generationCounter < maxGenerations && objects < maxObjects) {
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

			if (objects == maxObjects) {
				INFO(QString("Terminated because maximum number of objects reached (%1).").arg(maxObjects));
				INFO(QString("Use 'Set MaxObjects' command to increase this number."));
			}
			if (generationCounter == maxGenerations) {
				INFO(QString("Terminated because maximum number of generations reached (%1).").arg(maxGenerations));
				INFO(QString("Use 'Set MaxGenerations' command to increase this number."));
			}
			INFO("Done building...");
		}

		void Builder::setCommand(QString command, QString param) {
			if (command == "maxdepth") {
				//INFO(QString("Setting 'maxgenerations' to %1").arg(param));
				bool succes;
				int i = param.toInt(&succes);
				if (!succes) throw Exception(QString("Command 'maxdepth' expected integer parameter. Found: %1").arg(param));
				maxGenerations = i;
			} else if (command == "maxobjects") {
				//INFO(QString("Setting 'maxgenerations' to %1").arg(param));
				bool succes;
				int i = param.toInt(&succes);
				if (!succes) throw Exception(QString("Command 'maxobjects' expected integer parameter. Found: %1").arg(param));
				maxObjects = i;
			} else {
				throw Exception(QString("Unknown command: %1").arg(command));
			}
		}
		
		ExecutionStack& Builder::getExecutionStack() {
			return nextStack;
		}

	}
}