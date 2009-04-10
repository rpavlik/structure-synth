/*

	A simple demonstration of Structure Synth.
*/

set maxdepth 700
R1 


rule R1 {
  { x 1 rz 12  ry -12   s 0.99 } R1 
  { s 1 } x3 
}

rule R2 {
  { x -1 rx 7 rx -6 s 0.99 } R2
  { s 1 } x3
} 

rule x3 maxdepth 20 {
set seed initial
  { rz 10 rz 6 s 1 y 1 h 10} x3
  cbox
 
}


rule x3 maxdepth 20 {
{ rz -10 rx -6 ry 3  s 0.9 z 1 h 1} x3
cbox
  
}

rule cbox {
   { s 0.1 1 1 } box
{  s 0.1 1 1 color black }  grid
}