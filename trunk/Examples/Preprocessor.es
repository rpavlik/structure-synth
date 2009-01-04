/*
	An example of using preprocessor defines
*/

#define sizeStep 0.98
#define angle 6

set maxdepth 100
set background black

36 * { rx 10  x 0.2 sat 0.95 } R


rule R { R1 R2 }

rule R1 {
  {  x 0.6 rz angle ry angle s sizeStep hue 1 a 0.99 } R1
  { s 2 2 0.1  } sbox
}

rule R2 {
  { x -0.6 rz angle ry angle s sizeStep hue -1  sat 1 a 0.99  } R2
  { s 2 2  0.1 } sbox
} 

rule sbox {
  box
}
