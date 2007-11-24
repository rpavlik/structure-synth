set background white
set maxdepth 400
set maxobjects 50000
{ a 0.05 } R3

rule R1 {
{ x 1 rz  6 ry -18 s 1.01 s  0.99 hue 2 } R3
{ s 1 } sbox
}

rule R2 {
{ x -1 rz 6 ry 18 s 0.99 hue  -7 } R3
{ s 1 } sbox
} 

rule R3 {
R1
}

rule R3  {
R2
}

rule R3 w 0.1  {
R2
R1
}

rule sbox {
 { b 0.1  }   grid
 { a 2 } box 
 
}