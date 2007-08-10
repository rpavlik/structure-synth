#pragma once

#include <QString>
#include "Rendering/Renderer.h"
#include "RuleSet.h"
#include "State.h"
#include "ExecutionStack.h"

#include "../../../AppCore/Math/Matrix4.h"

namespace StructureSynth {
	namespace Model {	


		/// A Builder executes the rule set on a Renderer object
		class Builder {
		public:
			Builder(Rendering::Renderer* renderTarget, RuleSet* ruleSet);
			void build();

			void setCommand(QString command, QString param);
			ExecutionStack& getExecutionStack();
			State& getState() { return state; };
			Rendering::Renderer* getRenderer() { return renderTarget; };
			void increaseObjectCount() { objects++; };

		private:
			State state;
			ExecutionStack stack;
			ExecutionStack nextStack;
			Rendering::Renderer* renderTarget;
			RuleSet* ruleSet;
			int maxGenerations;
			int maxObjects;
			int objects;
		};

	}
}