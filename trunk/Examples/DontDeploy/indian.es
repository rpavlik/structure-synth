/*
	A simple demonstration of Structure Synth.
*/

set maxdepth 400
R1


rule R1 {
  { x 1 rz 6 ry 7   s 0.99 } R1 
  { s 1 } x3 
}

rule R2 {
  { x -1 rz 6 ry 6 s 0.99 } R2
  { s 1 } x3
} 

rule x3 maxdepth 20 {
  { rz 10 rx 6 s 0.9 z 1 h 10} x3
  box
 { color black } grid
}


rule x3 maxdepth 20 {
{ rz -10 rx -6 ry 3  s 0.9 z 1 h 1} x3
 box
  { color black }  grid
}