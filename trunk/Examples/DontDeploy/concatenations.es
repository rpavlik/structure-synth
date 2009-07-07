r1

rule r1  md 9  {

{ x 1 s 0.7 rz -1 hue 5 }  r1
{ s 1 1 0.1 } box
1 * { x 0.4 y -0.3 rx 90 ry -40 s 0.9 } 1 * { s 1 1 0.1 } box 

1 * { x -0.4 y -0.8  rz -25 s 0.6 sat 0.95 }  r1 
1 * { x -0.4 y 0.8 rz 25 s 0.6 sat 0.95 }  r1 
//1 * { s 0.9 x 0.9 rx 90  } 1 * { s 1 1 0.1 } box

//1 * { s 0.8  x -1.2 y 0.8 rz -225  }  r1
//1 * { s 0.8  x -1.2 y -0.8 rz 225  }   r1
}
