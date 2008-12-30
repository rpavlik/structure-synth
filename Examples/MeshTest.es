/*
	A simple demonstration of Structure Synth.
*/

set maxobjects 4200
set background #fff
R1
{ x 3 s 0.9  } R1
{ y 3 s 0.7 } R1
{ y -3 s 0.6 } R1
{ x -3 s 0.5 } R1

rule R1 {
  { z 1 ry 6   s 0.91  hue 5  y 0.01 } R1
 mesh
}


rule R1 w 0.2 {
{ z 1 ry 6   s 0.99  hue 1 } R1
{ z 1 rz 90 ry 6   s 0.99  hue 1 } R1
mesh
}


