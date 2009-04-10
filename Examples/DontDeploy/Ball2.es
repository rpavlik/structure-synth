// Camera settings. Place these before first rule call.
set translation [0 0 -20]
set rotation [0.993621 0.0416075 -0.104809 -0.046835 0.997752 -0.0479189 0.102579 0.0525221 0.993337]
set pivot [0 0 0]
set scale 0.316143

set seed 3
set background white

set maxdepth 2000
{ a 0.9 hue 30 } R1 

rule R1 w 10 { 
{ x 1  rz 3 ry 5 } R1
{ s 1 1 0.1 sat 0.9 color black  } box
} 

rule R1 w 10 { 
{ x 1  rz -3 ry 5   } R1
{ s 1 1 0.1 } box
} 

rule R1 w 1 { 
{ rx 180 } R2
R1
} 

rule R2  maxdepth 120  { 
{ x 1  rz -3 rx 5 ry 5  sat 0.98  s 0.97 } R2
{ s 1 1 0.1 } box
} 

rule R2  maxdepth 120 w 0.05 { 
  
} 