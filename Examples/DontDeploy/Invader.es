/*
	The mother of all sprites
*/

set maxdepth 400

{ x 00 y 00 z -30.9  s 140 140 1  } box 
1 * { y -3 x 2 } 1 * { x 1 } r
1 * { y -3 x 8 } 1 * { x 1 } r
1 * { y -2 x 3 } 1 * { x 1 } r
1 * { y -2 x 7 } 1 * { x 1 } r
1 * { y -1 x 2 } 7 * { x 1 } r
1 * { y 0 x 1 } 2 * { x 1 } r
1 * { y 0 x 4 } 3 * { x 1 } r
1 * { y 0 x 8 } 2 * { x 1 } r
1 * { y 1 } 11 * { x 1 } r
1 * { y 2 } 1 * { x 1 } r
1 * { y 2  x 2 } 7 * { x 1 } r
1 * { y 2  x 10 } 1 * { x 1 } r
1 * { y 3 } 1 * { x 1 } r
1 * { y 3  x 2 } 1 * { x 1 } r
1 * { y 3  x 8 } 1 * { x 1 } r
1 * { y 3  x 10 } 1 * { x 1 } r
1 * { y 4  x 3 } 2 * { x 1 } r
1 * { y 4  x 6 } 2 * { x 1 } r

rule r maxdepth 20 {
   { s 0.98 } box
   { z -1 ry 3  s 0.98 } r 
}

rule r maxdepth 20 {
{ s 0.98 } box
{ z -1 ry -3  s 0.95 } r 
}

rule r maxdepth 20 {
{ s 0.98 } box
{ z -1 rx 3 s 0.99 } r 
}

rule r maxdepth 20 {
{ s 0.98 } box
{ z -1 rx -3 s 0.99 } r 
}