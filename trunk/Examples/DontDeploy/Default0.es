/*
	A simple demonstration of Structure Synth.
#define sxx 0.97
#define srz 2
*/

set maxdepth 400
R1
R2

rule R1 {
  { x 1 rz srz ry srz s sxx} R1
  { s 1 } sphere
}

rule R2 {
  { x -1 rz srz ry srz s sxx } R2
  { s 1 } sphere
} 