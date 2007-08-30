{ b 0.8 sat 0.8 hue 76 } R1


rule R1 maxdepth 3 > c {
	{ s 1/3 x -1 y -1  } R2 
	{ s 1/3 x -1 y -1  z -1 } R2 
	{ s 1/3 x -1 y -1  z +1 } R2 
	{ s 1/3 x 1 y -1  } R2 
	{ s 1/3 x 1 y -1  z -1 } R2 
	{ s 1/3 x 1 y -1  z +1 } R2 
	{ s 1/3  y -1  z -1 } R2  
	{ s 1/3  y -1  z +1 } R2 
	{ s 1/3 x -1 y 1  } R2 
	{ s 1/3 x -1 y 1  z -1 } R2 
	{ s 1/3 x -1 y 1  z +1 } R2 
	{ s 1/3 x 1 y 1  } R2 
	{ s 1/3 x 1 y 1  z -1 } R2 
	{ s 1/3 x 1 y 1  z +1 } R2 
	{ s 1/3  y 1  z -1 } R2  
	{ s 1/3  y 1  z +1 } R2 
	{ s 1/3 x -1   z -1 } R2 
	{ s 1/3 x -1   z +1 } R2 
	{ s 1/3 x 1    z -1 } R2 
	{ s 1/3 x 1    z +1 } R2 
}

rule c  {
}

rule r2 {
  { hue 37 s 0.9 }  frame 
}

rule r2 {
{ hue 8 s 0.9  }  frame
}


rule r2 {
  { hue 19    }  r1
}


rule r2 {
  { hue 20   }  r1
}


rule frame  {
  { s 0.1 1.1 0.1 x 5  z 5 } box
  { s 0.1 1.1 0.1 x 5  z -5 } box
  { s 0.1 1.1 0.1 x -5  z 5 } box
  { s 0.1 1.1 0.1 x -5  z -5 } box

  { s 1 0.1 0.1 y 5  z 5 } box
 { s 1 0.1 0.1 y 5  z -5 } box
 { s 1 0.1 0.1 y -5  z 5 } box
 { s 1 0.1 0.1 y -5  z -5 } box

  { s 0.1 0.1 1 y 5  x 5 } box
{ s 0.1 0.1 1 y 5  x -5 } box
{ s 0.1 0.1 1 y -5  x 5 } box
{ s 0.1 0.1 1 y -5  x -5 } box
 { s 0.99 h 180 b 0.5 a 1  } box
}