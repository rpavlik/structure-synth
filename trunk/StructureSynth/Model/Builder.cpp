#include "Builder.h"
#include "../../SyntopiaCore/Logging/Logging.h"
#include "../../SyntopiaCore/Exceptions/Exception.h"
#include "../../SyntopiaCore/Misc/ColorUtils.h"
#include "../../SyntopiaCore/Math/Vector3.h"

#include <QProgressDialog>
#include <QApplication>

using namespace SyntopiaCore::Logging;
using namespace SyntopiaCore::Math;
using namespace SyntopiaCore::Exceptions;

namespace StructureSynth {
	namespace Model {

		Builder::Builder(Rendering::Renderer* renderTarget, RuleSet* ruleSet) : renderTarget(renderTarget), ruleSet(ruleSet) {
			maxGenerations = 1000;
			maxObjects = 100000;
			objects = 0;
			minDim = 0;
			maxDim = 0;
			newSeed = 0;
			hasSeedChanged = false;
			syncRandom = false;
			initialSeed = 0;
		};
			

		void Builder::build() {
			objects = 0;
			INFO("Starting builder...");
			
			/// Push first generation state
			stack.append(RuleState(ruleSet->getStartRule(), State()));
			int generationCounter = 0;
			
			QProgressDialog progressDialog("Building objects...", "Cancel", 0, 100, 0);
			progressDialog.setWindowModality(Qt::WindowModal);
			progressDialog.setMinimumDuration(0);
			progressDialog.show();
			progressDialog.setValue(0);

			int lastValue = 0;
			int maxTerminated = 0;
			int minTerminated = 0;

			int syncSeed = 0;
			if (syncRandom) {
				syncSeed = rand();
			}

			while (generationCounter < maxGenerations && objects < maxObjects && stack.size() < maxObjects) {

				syncSeed = rand();

				double p = 0;
				if (maxObjects>0) {
					p = objects/(double)maxObjects;
				}

				double p2 = 0;
				if (maxGenerations>0) {
					p2 = generationCounter/(double)maxGenerations;
				}

				double progress = p;
				if (p2 > p) progress = p2;

				if (maxObjects<=0 && maxGenerations<=0) {
					progress = (generationCounter%9)/9.0;
				}

				if (lastValue != (int)(progress*100.0)) {
					progressDialog.setValue((int)(progress*100.0));
					progressDialog.setLabelText(
						QString("Building objects...\r\n\r\nGeneration: %1\r\nObjects: %2\r\nPending rules: %3")
						.arg(generationCounter).arg(objects).arg(stack.size()));
					qApp->processEvents();
				}

				lastValue = (int)(progress*100.0);
				
				if (progressDialog.wasCanceled()) {
					break;
				}

				generationCounter++;

				// Now iterate though all RuleState's on stack and create next generation.
				//INFO(QString("Executing generation %1 with %2 individuals").arg(generationCounter).arg(stack.size()));
				nextStack.clear();
				for (int i = 0; i < stack.size(); i++) {
					//	INFO("Executing: " + stack[i].rule->getName());
					currentState = &stack[i].state;
					if (currentState->seed != 0) {
						srand(currentState->seed);
						currentState->seed = rand();	
					}
					state = stack[i].state; 
					

					// if we are synchronizing random numbers every state must get the same rands
					if (syncRandom) {
						srand(syncSeed);
					}

					// Check the dimensions against the min and max limits.
					if (maxDim != 0 || minDim != 0) {
						Vector3f s = state.matrix * Vector3f(1,1,1) - state.matrix * Vector3f(0,0,0);
						double l = s.length();

						if (maxDim && l > maxDim) {
							maxTerminated++; continue;
						}
						if (minDim && l < minDim) {
							minTerminated++; continue;
						}				
					}
					
					

					stack[i].rule->apply(this);
				}
				stack = nextStack;

				if (stack.size() == 0) break; // no need to continue...
			}

			progressDialog.setValue(100); 
			progressDialog.hide();

			if (progressDialog.wasCanceled()) {
				INFO("User terminated.");
			}

			if (objects >= maxObjects) {
				INFO(QString("Terminated because maximum number of objects reached (%1).").arg(maxObjects));
				INFO(QString("Use 'Set MaxObjects' command to increase this number."));
			}

			if (stack.size() >= objects) {
				INFO(QString("Terminated because the number of pending rules reached (%1).").arg(maxObjects));
				INFO(QString("Use 'Set MaxObjects' command to run for longer time."));
			}
			
			if (generationCounter == maxGenerations) {
				INFO(QString("Terminated because maximum number of generations reached (%1).").arg(maxGenerations));
				INFO(QString("Use 'Set Maxdepth' command to increase this number."));
			}

			if (maxTerminated != 0) {
				INFO(QString("Terminated %1 branches, because the dimension was greater than max size (%2)").arg(maxTerminated).arg(maxDim));
			}
			if (minTerminated != 0) {
				INFO(QString("Terminated %1 branches, because the dimension was less than min size (%2)").arg(minTerminated).arg(minDim));
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
			} else if (command == "syncrandom") {
				if (param.toLower() == "true") {
					syncRandom = true;
				} else if (param.toLower() == "false") {
					syncRandom = false;
				} else { 
				  throw Exception(QString("Command 'syncrandom' expected either 'true' or 'false'. Found: %1").arg(param));
				}

			} else if (command == "maxsize") {
				bool succes;
				float f = param.toFloat(&succes);
				if (!succes) throw Exception(QString("Command 'maxsize' expected floating-point parameter. Found: %1").arg(param));
				maxDim = f;
			} else if (command == "minsize") {
				bool succes;
				float f = param.toFloat(&succes);
				if (!succes) throw Exception(QString("Command 'minsize' expected floating-point parameter. Found: %1").arg(param));
				minDim = f;
			} else if (command == "maxobjects") {
				//INFO(QString("Setting 'maxgenerations' to %1").arg(param));
				bool succes;
				int i = param.toInt(&succes);
				if (!succes) throw Exception(QString("Command 'maxobjects' expected integer parameter. Found: %1").arg(param));
				maxObjects = i;
			} else if (command == "seed") {
				
				if (param.toLower() == "initial") {
					if (initialSeed == 0) {
						initialSeed = rand();
					}
					currentState->seed = initialSeed;
					state.seed = initialSeed;
					
				} else {
					bool succes;
					int i = param.toInt(&succes);
					if (!succes) throw Exception(QString("Command 'seed' expected integer parameter or 'same'. Found: %1").arg(param));
					srand(i);
					hasSeedChanged = true;
					newSeed = i;
				}
			} else if (command == "background") {
				QColor c(param);
				if (!c.isValid()) throw Exception(QString("Command 'background' expected a valid color identifier: Found: %1").arg(param));
				renderTarget->setBackgroundColor(Vector3f(c.red() / 255.0, c.green() / 255.0, c.blue() / 255.0) );
			} else if (command == "scale") {
				bool succes;
				double s = param.toDouble(&succes);
				if (!succes) throw Exception(QString("Command 'scale' expected floating point parameter. Found: %1").arg(param));
				renderTarget->setScale(s);
			} else if (command == "translation") {
				bool succes;
				Vector3f v3(param, succes);
				if (!succes) throw Exception(QString("Command 'translation' expected vector (such as [1 3 -10.1]). Found: %1").arg(param));
				renderTarget->setTranslation(v3);
			} else if (command == "pivot") {
				bool succes;
				Vector3f v3(param, succes);
				if (!succes) throw Exception(QString("Command 'pivot' expected vector (such as [1 3 -10.1]). Found: %1").arg(param));
				renderTarget->setPivot(v3);
			} else if (command == "rotation") {
				bool succes;
				Matrix4f m4(param, succes);
				if (!succes) throw Exception(QString("Command 'rotation' expected matrix (such as [1 0 0 0 1 0 0 0 1]). Found: %1").arg(param));
				renderTarget->setRotation(m4);
			} else if (command == "opengl") {
				INFO("Render commands for 'opengl' not impl'ed yet!");
			} else if (command == "template") {
				renderTarget->callCommand(command,param);
			} else {
				throw Exception(QString("Unknown command: %1").arg(command));
			}
		}
		
		ExecutionStack& Builder::getNextStack() {
			return nextStack;
		}

	}
}

