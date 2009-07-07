set maxobjects 16000
10 * { y 1 } 10 * { z 1 }  1 * { a 0.8  sat 0.9  } r1 
set background #fff

// seed 32
// Camera settings. Place these before first rule call.
set translation [-1.97808 -3.30706 -20]
set rotation [0.663812 -0.72778 0.172253 0.394999 0.536744 0.745573 -0.635071 -0.426883 0.643773]
set pivot [0 0 0]
set scale 0.194418

{ z -1 s 400 400 1 color #ddd } box

rule r1   {
  { x 1  ry 4 } r1
  xbox
}

rule r1   {
{ x 1  ry -4  } r1
xbox
}

rule r1   {
{ x 1  rz -8  s 0.95 } r1
xbox
}

rule r1   {
{ x 1  rz 8  s 0.95   } r1
xbox
}



rule r2 maxdepth 36 {
{ ry 1  ry -13 x  1.2 b 0.99 h 12  } r2 
xbox
}

rule xbox {
  { s 1.1   color #000   } grid
  { b 0.7  color #000   a 0.9  }  box
}

rule xbox {
 { s 1.1   color #000     } grid
 { b 0.7  color #fff  a 0.9    } box
}
