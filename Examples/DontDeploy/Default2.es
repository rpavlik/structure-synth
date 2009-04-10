/*
	A simple demonstration of Structure Synth.
*/

set maxdepth 20
R1

rule R1 maxdepth 5 {
  { x 1 rz 6 ry 6 s 0.99 } R1
  { s 2 } sphere
}

