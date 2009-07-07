


set background #fff

5 * { z 10 } s2

rule s2  {
{ s 20 20 0.3 } box
10  * { x 2 } 10  * { y 2 } 1 * { x -11 y -11 }  d1
 10  * { x 2 } 10  * { y 2 } 1 * { x -11 y -11 rx 180 }  d1
 }

rule d1  md 10 {
 { s 0.2 0.1 1 } box
{ z 1 ry 5 s 0.9  } d1
}

rule d1  md 10 {
 { s 0.2 0.1 1 } box
{ z 1 rx 5 s 0.9 } d1
}

rule d1  md 10 {
 { s 0.2 0.1 1 } box
{ z 1 rz 5 } d1
}

rule d1  md 10 {
 { s 0.2 0.1 1 } box
{ z 1 ry -5 s 0.9  } d1
}

rule d1  md 10 {
 { s 0.2 0.1 1 } box
{ z 1 rx -5 s 0.9 } d1
}

rule d1  md 10 {
 { s 0.2 0.1 1 } box
{ z 1 rz -5 } d1
}