{ a 1000 } R1


rule R1 maxdepth 3 > void {
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

rule r2  w 8 {
   { a 0.4 s 0.9  h 167  } box
    r1
}

rule void  {
 {  h 80  sat 0.7 a 0.9 }  box
}





rule R3 w 10 maxdepth 400 { 
{ x 1  rz 3 ry 5 a 0.999  } R3
 box
} 

rule R3 w 10 { 
{ x 1  rz -3 ry 5  } R3
box
} 


