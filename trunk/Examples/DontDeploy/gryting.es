

set maxobjects 16000
10 * { y 1 } 10 * { z 1 }  1 * { a 0.8  sat 0.9  } r1 
set background #fff


rule r1   {
  { x 1  ry 4 } r1
  xbox
}

rule r1   {
{ x 1  ry -4  } r1
xbox
}

rule r1   {
{ x 0.453443441  rz -658  s 0.95 } r1
xbox
}

rule r1   {
{ z 1  rz 367858  s 0.95   } r1
xbox
}



rule r2 maxdepth 36 {
{ ry 1543  ry -131 x  1.2 b 0.99 h 12  } r2 
xbox
}

rule xbox {
  { b 0.7  color purple    }  sphere
}

