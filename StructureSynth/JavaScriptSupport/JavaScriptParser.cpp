#include "JavaScriptParser.h"

#include <QScriptEngine>
#include <QMetaType>
#include "SyntopiaCore/Logging/Logging.h"

#include "Debug.h"

using namespace SyntopiaCore::Logging;

Q_DECLARE_METATYPE(StructureSynth::JavaScriptSupport::Vector3)


namespace StructureSynth {
	namespace JavaScriptSupport {	

		namespace {
			QScriptValue constructVector3(QScriptContext * context, QScriptEngine *engine)
			{
				StructureSynth::JavaScriptSupport::Vector3 bar;
				// initialize from arguments in context, if desired
				QScriptValue v =  engine->toScriptValue(bar);
				if (context->argumentCount() == 3) {
					v.setProperty("x", QScriptValue(engine,context->argument(0).toNumber()));
					v.setProperty("y", QScriptValue(engine,context->argument(1).toNumber()));
					v.setProperty("z", QScriptValue(engine,context->argument(2).toNumber()));
				}
				return v;
			}

			QScriptValue vector3ToScriptValue(QScriptEngine *engine, const StructureSynth::JavaScriptSupport::Vector3 &s)
			{
				INFO("<-toScriptValue: From " + s.toString());
				QScriptValue obj = engine->newObject();
				Vector3 v;
				v.writeX(12);
				v.writeY(22);
				v.writeZ(32);
				obj.setPrototype(engine->newQObject(&v));
			
				obj.setProperty("x", QScriptValue(engine, s.readX()));
				obj.setProperty("y", QScriptValue(engine, s.readY()));
				obj.setProperty("z", QScriptValue(engine, s.readZ()));
				return obj;
			}

			void vector3FromScriptValue(const QScriptValue &obj, StructureSynth::JavaScriptSupport::Vector3 &s)
			{
				INFO("<-fromScriptValue");
				s.writeX( obj.property("x").toNumber() );
				s.writeY( obj.property("y").toNumber() );
				s.writeZ( obj.property("z").toNumber() );
			} 

			QScriptValue addSphereStatic(QScriptContext *context, QScriptEngine *engine)
			{
				QScriptValue callee = context->callee();
				if (context->argumentCount() == 2) {
					Vector3 v = qScriptValueToValue<Vector3>(context->argument(0));
					double radius = context->argument(1).toNumber();
					INFO("Got: " + v.toString() + " and " + QString::number(radius));
					//SyntopiaCore::GLEngine::Object3D* o = new SyntopiaCore::GLEngine::Sphere( center.getObj(), radius);
					//engine->addObject(o);
				} else {
					WARNING("addSphere expected two arguments..");
				}
				return QScriptValue();
			}
				
			

		}

		JavaScriptParser::JavaScriptParser(SyntopiaCore::GLEngine::EngineWidget* engine3D) : engine3D(engine3D) {
		}

		JavaScriptParser::~JavaScriptParser() {
		}
			
		void JavaScriptParser::parse(QString input) {

			INFO("Initializing JavaScript environment.");
			QScriptEngine engine;

			// Setup the global objects...
			Debug debugObject;
			engine.globalObject().setProperty("Debug", engine.newQObject(&debugObject)); 

			World world(engine3D);
			QScriptValue w = engine.newQObject(&world);
			w.setProperty("add", engine.newFunction(addSphereStatic));
			engine.globalObject().setProperty("World", w); 
			 

			Vector3 *v3PrototypeObject = new Vector3();
			QScriptValue vProto = engine.newQObject(v3PrototypeObject);
			int id = qMetaTypeId<StructureSynth::JavaScriptSupport::Vector3>();
			engine.setDefaultPrototype(id, vProto);
			QScriptValue vectorCtor = engine.newFunction(constructVector3, vProto);
			engine.globalObject().setProperty("Vector3", vectorCtor); 

			//qScriptRegisterMetaType(&engine, vector3ToScriptValue, vector3FromScriptValue, vProto); 
			//qScriptRegisterMetaType(&engine, vector3ToScriptValue, vector3FromScriptValue); 

			// Execute and catch exceptions.
			QScriptValue result = engine.evaluate(input);
			if (engine.hasUncaughtException()) {
				int line = engine.uncaughtExceptionLineNumber();
				QString error =  QString("Uncaught exception at line %1:%2").arg(line).arg(result.toString());
				WARNING(error);
			} else {
				INFO(result.toString());
			}

		}
		
	}
}
