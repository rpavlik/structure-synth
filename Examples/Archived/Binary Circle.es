
72 * {  x 1 rz 5  } 1 * { b 1.2 sat 1.0  hue 176   y 1 z 1  } R1

rule R1 maxdepth 2 > void {
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

rule void  {
}

rule r2 {
{ hue 90 s 0.9 }  frame 
}

rule r2 {
{ hue 45 s 0.9 sat 0.5  }  frame
}


rule r2 {
{ hue 12    }  r1
}


rule r2 {
{ hue 73   }  r1
}


rule frame  {
/*
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
*/
{ s 0.99 h 180 b 0.9 a 1  } box
}