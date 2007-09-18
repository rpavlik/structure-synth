#include "RuleSet.h"
#include "RuleRef.h"
#include "CustomRule.h"
#include "AmbiguousRule.h"
#include "PrimitiveRule.h"

#include <QStringList>
#include <QMap>

#include "../../AppCore/Exceptions/Exception.h"
#include "../../AppCore/Logging/Logging.h"

using namespace AppCore::Exceptions;
using namespace AppCore::Logging;

namespace StructureSynth {
	namespace Model {	

		/// Constructor. Automatically adds built-in rules.
		RuleSet::RuleSet() {
			topLevelRule = new CustomRule("TopLevelRule");

			/// Add instances of predefined built-in types.
			rules.append(new PrimitiveRule(PrimitiveRule::Box));
			rules.append(new PrimitiveRule(PrimitiveRule::Sphere));
			rules.append(new PrimitiveRule(PrimitiveRule::Cylinder));
			rules.append(new PrimitiveRule(PrimitiveRule::Line));
			rules.append(new PrimitiveRule(PrimitiveRule::Dot));
			rules.append(new PrimitiveRule(PrimitiveRule::Grid));
			rules.append(topLevelRule);
		};


		/// Deletes rules
		RuleSet::~RuleSet() {
			for (int i = 0; i < rules.size(); i++) delete(rules[i]);
		}

		void RuleSet::addRule(Rule* rule) { 
			

			// Check if the rule name is already used...
			QString name = rule->getName();
			
			/*
			if (startRule == 0) {
				startRule = rule;
				INFO("Added as start rule: "+name);

			}*/

			for (int i = 0; i < rules.size(); i++) {

				if (rules[i]->getName() == name) {
					if (typeid(*rules[i]) == typeid(CustomRule)) {
						// A Custom rule already exists with the same name.
						// Now we must remove the existing rule, and create a new ambiguous rule hosting them both.
						//Debug("Encountered custom rule with name: "+ name);
						Rule* r = rules[i];
						int count = rules.removeAll(r);
						//Debug(QString("Removed %1 custom rule").arg(count));
						CustomRule* cr1 = dynamic_cast<CustomRule*>(r);
					
						AmbiguousRule* ar = new AmbiguousRule(name);
						ar->appendRule(cr1);

						CustomRule* cr2 = dynamic_cast<CustomRule*>(rule);
						if (!cr2) throw Exception("Trying to add non-custom rule to ambiguous rule: '%1'. "+name);
						ar->appendRule(cr2);

						rules.append(ar);

						return;
					} else if (typeid(*rules[i]) == typeid(PrimitiveRule)) {
						// A primitive rule already exists with the same name. This is not acceptable.
						//Debug("Encountered primitive rule with name: "+ name);
						throw Exception(QString("A primitive rule already exists with the name: '%1'. New definitions can not merged.").arg(name));
					} else if (typeid(*rules[i]) == typeid(AmbiguousRule)) {
						// A ambiguous rule already exists with the same name. We will add to it.
						//Debug("Encountered ambiguous rule with name: "+ name);
						AmbiguousRule* ar = dynamic_cast<AmbiguousRule*>(rules[i]);
						CustomRule* cr = dynamic_cast<CustomRule*>(rule);
						if (!cr) throw Exception("Trying to add non-custom rule to ambiguous rule: '%1'. "+name);
						ar->appendRule(cr);
						return;
					} else {
						WARNING("Unknown typeid");
					}
				}

			}

			rules.append(rule);
		}


		/// Resolve symbolic names into pointers
		void RuleSet::resolveNames() {

			// build map
			QMap<QString, Rule*> map;
			for (int i = 0; i < rules.size(); i++) map[rules[i]->getName()] = rules[i];



			// resolve rules.
			for (int i = 0; i < rules.size(); i++) {

				QList<RuleRef*> refs = rules[i]->getRuleRefs();

				for (int j = 0; j < refs.size(); j++) {
					QString name = refs[j]->getReference();
					if (!map.contains(name)) {
						throw Exception(QString("Unable to resolve rule: %1").arg(name));
					}
					refs[j]->setRef(map[name]);
				}

			}

		}

		///
		QStringList RuleSet::getUnreferencedNames() {

			WARNING("RuleSet::getUnreferencedNames(): Not implemented yet!");
			return QStringList();

		};

		Rule* RuleSet::getStartRule() {
			return topLevelRule;
		};

		/// For debug
		void  RuleSet::dumpInfo() {
			int custom = 0;
			int ambi = 0;
			int primitive = 0;
			int rulesCount = 0;

			for (int i = 0; i < rules.size(); i++) {
				rulesCount++; 

				CustomRule* cr = dynamic_cast<CustomRule*>(rules[i]);
				if (cr) custom++;

				AmbiguousRule* ar = dynamic_cast<AmbiguousRule*>(rules[i]);
				if (ar) ambi++;

				PrimitiveRule* pr = dynamic_cast<PrimitiveRule*>(rules[i]);
				if (pr) primitive++;
			}

			Debug(QString("Loaded %1 user rules: %2 Custom Rules, %3 Ambiguous Rules")
				.arg(rulesCount-primitive).arg(custom).arg(ambi));
			Debug(QString("Loaded %1 built-in rules.").arg(primitive));
		}

	}
}