/*
	A simple demonstration of Structure Synth.
*/

set maxdepth 400
{ s 100 100 1 } box
50 * { z 0.8  } R
1 * { z 8 x -2.5 } 30 * { z 0.8 rz 55  } 1 * {   x -7  } br 

rule br {
  
  {  ry 54 z -20   s 0.05 0.05 50 } box
}

rule br {
{  ry 44 z -20   s 0.05 0.05 50 } box
} 

rule r {
  r1
  { x -2.8  y 2.8  z 0.4 } r2
}

rule r1 {
  4 * { x -1.1 } r11
}

rule r11 {
  { s 1 5 0.3 } box
}

rule r11 {
{ rz 5 s 1 5 0.3 } box
}

rule r11 {
{ rx 2 s 1 5 0.3 } box
}

rule r11 w 0.5 {

}


rule r2 {
4 * { y -1.1 } r12
}

rule r12 {
{ s 5 1 0.3 } box
}

rule r12 {
{ rz 5 s 5 1 0.3 } box
}


rule r12 {
{ rx 2 s 5 1 0.3 } box
}
rule r12 w 0.5 {
}