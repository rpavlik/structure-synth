// Master sphere
set maxobjects  30400
set background #454

15 * { s 0.9 z -2 y 0.5   } 1 * {  color #5a5 s 0.92  rz 0  }  r1
15 * { s 0.9 z -2 y 0.5    } 1 * {  color #aaa  s 0.87   rz 0 }  r1
15 * { s 0.9 z -2 y 0.5   } 1 * {  color #fff }  r1


rule r1 w 40  {
  ubox
  dbox
  { x 1 ry 3 } r1 
}

rule r1  w 14 {  r2 }


rule r2 w 10 {  
  { x 1 ry 3 } r2
}

rule r2  {  r1 }

rule dbox w 8 maxdepth 15 {
   { y -1 rx 2.9  } dbox
   sbox
}

rule dbox { }


rule ubox w 8  maxdepth 15 {
 { y 1 rx -2.9  }  ubox
sbox
}

rule ubox { }


rule sbox   {
  { s 1.2  1.2 0.6  ry 5 }  box
}