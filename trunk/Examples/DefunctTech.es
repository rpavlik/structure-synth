rule start { 
set maxgenerations 400
R1

} 

rule R1 w 100 { 
{ x 1 rz 6 ry 6 s 0.99 } R1
{ s 2 } sphere 
} 

rule R1 w 10 { 
{ x 1 rz -6 ry 6 s 0.99 } R1
{ s 2 } sphere 
} 

rule R1 w 1  { 
  R1
  { x -1 rz 180 } R1
} 
