

set maxdepth 200
set background #000
1 * { hue 60 sat 0 a 1.5 } 20 * { ry 90 x 6 } 10 * { rx 36 hue 10  } R1


rule R1 {
  { z 1 rx -4 ry 6 s 0.99 hue 0.1 s 1.1 } R1
   box
}

rule R1 {
{ z 1 rx 4 rz 6 s 0.99 s 0.92 } R1
 box
}
