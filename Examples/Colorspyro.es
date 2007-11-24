set maxobjects 3000
set maxdepth 14000
{ sat 0.5  hue 50 b 0.9  } R1
set background #555

rule R1 {
{ y 0.5  rz 8 rx 1 h 6 s 1.01 1 1 ry 0.1  z 0.1 } R3
{ s 1 } sbox
}

rule R3  {
{ x 0.5  rz 8  rx -1  h 6 } R1
{ s 1 } sbox
}

rule R2 {
{ x -0.5 rz 3 rx 1  s 0.99 } R2
{ s 1 } sbox
} 

rule sbox {
{ b 0.6 }   grid
    box

}

rule sbsox  w 2  {
{ color #555  }   grid


}