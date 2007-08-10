rule start { 
set maxgenerations 2000
R1

} 

rule R1 w 10 { 
{ x 1  rz 3 ry 5  } R1
 box
} 

rule R1 w 10 { 
{ x 1  rz -3 ry 5  } R1
box
} 

