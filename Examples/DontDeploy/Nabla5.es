// 31 ,47, 51, 59

// City: seed 70

set maxdepth 40
//set maxsize 10
set colorpool randomrgb
//set colorpool colorlist:orange,white,white,white,white,white,white,grey
// Camera settings. Place these before first rule call.
set translation [-25.9557 -3.34381 -20]
set rotation [0.653426 0.755741 0.0415757 0.531119 -0.418698 -0.736492 -0.539223 0.503358 -0.675026]
set pivot [0 0 0]
set scale 0.319395

10 * { y 10 } 10 * { x 10 }  1 * { ry -90 b 0.2 } R1
set background black
{  x 30 y 30 z 3 s 200 200 1 color random } box

Rule R1 {
dbox
//set seed initial
{ z 0.6 rx 5   }  R1
}

Rule R1 {
 dbox
{ z 0.5 rx -90 }  R1
}

Rule R1 {
dbox
{ z 0.6 rz 90 }  R1
}

Rule R1 {
 dbox
{ z 0.6 rz -90 }  R1
}

Rule R1 weight 0.01 {

} 

Rule dbox {
  { color random s 2 2  0.5  }  box
}

Rule dbox weight 0.5 {
   { ry 90 s 0.5  1 1 } R1
}

Rule dbox weight 0.5 {
{ rx 90 s 0.5 2 1 } R1
} 