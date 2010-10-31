// 10, 20 , 21, 23, 33 , 104, 106, 118, 119, 127, 128, 133, 136, 138, 139
//1, 66,69,70,73, 79 

set maxdepth 50
set colorpool randomhue
set seed 1
// Camera settings. Place these before first rule call.
set translation [0.469127 6.25771e-10 -20]
set rotation [0.629188 -0.115767 -0.768582 0.347352 -0.842724 0.411288 -0.695318 -0.525747 -0.49002]
set pivot [0 0 0]
set scale 1.26089

{ ry -90 b 0.2 } R1

//{  x 30 y 30 z -3 s 900 900 1 color red } box

Rule R1 {
dbox
{ z 0.6 rx 5  s 1.01 }  R1
}

Rule R1 {
 dbox
{ z 0.5 rx -90 s 0.99 }  R1
}

Rule R1 {
dbox
{ z 0.6 rz 90 s 1.02 }  R1
}

Rule R1 {
 dbox
{ z 0.6 rz -90  s 0.98}  R1
}

Rule R1 weight 0.01 {

} 

Rule dbox {
  { color random   s 0.1 }  box
}

Rule dbox weight 0.5 {
   { ry 90 s 1.01  } R1
}

Rule dbox weight 0.5 {
{ rx 90  s 0.99  } R1
} 