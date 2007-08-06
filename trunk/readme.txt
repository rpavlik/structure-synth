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







