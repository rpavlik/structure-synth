#define DISTANCE0 OFF
#define DISTANCE1 ON
#define DISTANCE2 ON
#define DISTANCE3 OFF
#define WIDTH  0.05
#define STEP 9

// Camera settings. Place these before first rule call.
set translation [0.566876 -3.52063 -20]
set rotation [0.596494 -0.797386 0.0913825 0.253351 0.295103 0.921269 -0.761576 -0.526383 0.378044]
set pivot [0 0 0]
set scale 5.35875

set background #fff

ON

rule ON maxdepth 3 > MyBox {
       { s 1/3 } DISTANCE0	

       { s 1/3 x -1  }  DISTANCE1
	{ s 1/3 x +1  }  DISTANCE1 
       { s 1/3 y -1  }  DISTANCE1
	{ s 1/3 y +1  }  DISTANCE1
       { s 1/3 z -1  }  DISTANCE1
	{ s 1/3 z +1  }  DISTANCE1

	{ s 1/3 x -1 y -1 }   DISTANCE2 
	{ s 1/3 x 1 y -1 }    DISTANCE2 
	{ s 1/3  y -1  z -1 } DISTANCE2  
	{ s 1/3  y -1  z +1 } DISTANCE2 
	{ s 1/3 x -1 y 1 }    DISTANCE2 
	{ s 1/3 x 1 y 1 }     DISTANCE2 
	{ s 1/3  y 1 z -1 }   DISTANCE2  
	{ s 1/3  y 1 z +1 }   DISTANCE2 
	{ s 1/3 x -1 z -1 }   DISTANCE2 
	{ s 1/3 x -1 z +1 }   DISTANCE2 
	{ s 1/3 x 1 z -1 }    DISTANCE2 
	{ s 1/3 x 1 z +1 }    DISTANCE2 
	
	{ s 1/3 x 1 y -1  z -1 }  DISTANCE3 
	{ s 1/3 x -1 y -1  z -1}  DISTANCE3 
	{ s 1/3 x -1 y -1  z +1}  DISTANCE3 
	{ s 1/3 x 1 y -1  z +1 }  DISTANCE3 	
	{ s 1/3 x -1 y 1  z -1 }  DISTANCE3 
	{ s 1/3 x -1 y 1  z +1 }  DISTANCE3 
	{ s 1/3 x 1 y 1  z -1 }   DISTANCE3 
	{ s 1/3 x 1 y 1  z +1 }  DISTANCE3   
}

rule MyBox {
	{ s 0.9 color white } box
  { color black } frame
	
}

rule frame  {
{ s WIDTH 1 WIDTH x STEP  z STEP } box
{ s WIDTH 1 WIDTH x STEP  z -STEP } box
{ s WIDTH 1 WIDTH x -STEP  z STEP } box
{ s WIDTH 1 WIDTH x -STEP  z -STEP } box

{ s 1 WIDTH WIDTH y STEP  z STEP } box
{ s 1 WIDTH WIDTH y STEP  z -STEP } box
{ s 1 WIDTH WIDTH y -STEP  z STEP } box
{ s 1 WIDTH WIDTH y -STEP  z -STEP } box

{ s WIDTH WIDTH 1 y STEP  x STEP } box
{ s WIDTH WIDTH 1 y STEP  x -STEP } box
{ s WIDTH WIDTH 1 y -STEP  x STEP } box
{ s WIDTH WIDTH 1 y -STEP  x -STEP } box

}

rule Off {
}