/*
	A simple demonstration of Structure Synth.
*/

set maxdepth 400
R1
R2

{ x -10 s 20 1 1 color red } box
{ y -10 s 1 20 1 color green } box
{ z -10 s 1 1 20 color blue } box

rule R1 {
  { x 1 rz 6 ry 6 s 0.99 } R1
  { s 2 } sphere
}

rule R2 {
  { x -1 rz 6 ry 6 s 0.99 } R2
  { s 2 } sphere
} 