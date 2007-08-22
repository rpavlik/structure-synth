Structure Synth Road Map
------------------------

Retirement :
  
  // Rule retirement
  Rule R1 maxdepth 10 { ...  }

  // Rule substitution retirement
  Rule R1 weigth 10 maxdepth 10 > R2 { ...  }
  
  // Rule retirement with surrection.
  Rule R1 w 10 md 10 >> R2 {
  }
  

Version 0.5 (Kolberger Heide)
-----------------------------
- Version number
- Menu item 'Examples' which autoscans some dir.
- SET command (where should it be stored...) (COMMAND instead? Strange shortcut symbol (i.e. <Background #234>?)
- Log framework (at least coloring)
- Color support: HSV?
- Transformations: s (size), m (matrix), color transforms
- Command to limit recursion.
- Support for primitives: box, cylinder, line, point, tube
- Zoom to fit
- 3D Engine mouse nav improvements.

- Restructure: 
	-'const correctness'
	-TODO's
	-'AppCore'->'Syntopia Core'
	-GUI files the proper place
	-No local filesystem reference i VS project files.
	-Release build should be fixed
	-Check directory structure.
	-Comments
				

Version 0.8 ()
--------------
- Preprocessor (Support for including files)
- Support for customization of OpenGL engine (Fog, Lightning, Polygons count, projection, background, anti-alias)
- Drag'n'drop of eisenscript
- Setting of Random Seed.
- Tabs in UI
- Alpha support
- Alpha depth sorting
- Complete Syntax highlightning
- Full Screen
- Splash screen

- Installer

Version 1.0 (Optimus)
---------------------
- Check for updates on internet
- Linux (and Mac) support
  - Makefiles.
- PovRay export
- Documentation.



  
SynCore (Framework for rapid application development)
------------------------------------------------
- Logging Framework.
- Math/Matrix mini classes.
- Small OpenGL visualization engine.
- Version / Auto check for update 








TODO:


Clean up development a little.

'For Context Free / CFDG users'

The EisenScript syntax in Structure Synth has a lot in common with CFDG.
There are however a few differences:

- The 'startrule' statement: in CFDG an startrule has to be explicitly specified. In EisenScript, a more generic approach is used: statements which can be used in a rule definition, can also be used at the top-level scope, so in order to specify a start-rule, just write the name of the rule.

20 * { x 1 } tree   // Start out with 20 tree objects

rule tree { ... } 

- Termination criteria: in CFDG the recursion automatically terminates when the object produced no longer are visible. This is a very elegant solution, but it is not easy to do in a dynamic 3D world, where the user can reposition or zoom with the camera. Several options exists in Structure Synth for termination the rendering:

- Transformation order: in CFDG transformations (which CFDG refers to as adjustments) in curly brackets are not applied in the order of appearence, and if multiple transformations of the same type are applied, only the last one is actually carried out. For transformations in square brackets in CFDG the order on the other hand is significant. In Structure Synth the transformation order is always significant: transformations are applied starting from the right-most one. 

- Whitespace handling: In structure synth every <i>token</i> (e.g. a number, a keyword, a symbolic name) must be separated by whitespace (ordinary spaces, newlines or tabs). This means that statements like " rule name{ ... }" will not parse - the correct form is " rule name { ... } ", with a whitespace after the rule name. This is mainly due to lazyness from my side (since this makes it very easy to write a tokenizer for the grammar), and in future version I might make the parser more forgiving.

- Transformation loops: 

'Acknowledgements' (icons, Qt, Context Free, CFDG).


A EisenScript program consists of a number of actions and rule definitions.

An action can be either:
- A rule call:
  - either a single call.
  - a transformation list followed by a rule call.
  - a number of transformation loops followed by a rule call.
- A SET statement.
