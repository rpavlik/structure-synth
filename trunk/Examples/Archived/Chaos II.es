set maxdepth 400
{ b 0.9 h 45  } R3

rule r3  maxdepth 3 > R1 {
  { sat 0.9  a 0.9 hue 120  } r3
 r1
}

rule R1 w 100 { 
{ x 1 rz 6 ry 6 s 0.99 h 0.9 } R1
{ s 1 } box 
} 

rule R1 w 100 { 
{ x 1 rz -6 ry 6 s 0.99 h -0.8  } R1
{ s 1 } box 
} 

rule R1 w 10 { 
{ x 1 rz -6 ry 6 s 0.99 a 0.9 h 1 } R1
{ s 1 } box 
} 

rule R1 w 3  { 
  R1
  { x -1 rx 70 } R1
} 

rule R1 w 3  { 
R1
{ x -1 ry 70 } R1
} 



