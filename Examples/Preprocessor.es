/*
	An example of using preprocessor defines
*/

#define sizeStep 0.98
#define angle 4

set maxdepth 400
R1
R2

rule R1 {
  {  x 1 rz angle ry angle s sizeStep hue 1 a 0.97 } R1
  { s 2 } sphere
}

rule R2 {
  { x -1 rz angle ry angle s sizeStep hue -1  sat 1 a 0.97  } R2
  { s 2 } sphere
} 