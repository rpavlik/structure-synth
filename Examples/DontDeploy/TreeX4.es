set maxdepth 20



1 * { rz 90   x 10  y 12 s 9.8 } 5 * { ry 1 } 50 * { ry 10 rx 1 z 0.8 } 1 *  { s 0.01 1 10 } box


{  x 30 y 30 z 0 s 900 900 1 h 90 } box

Rule R1 {
dbox
{ z 0.6 rx 5  s 0.9 }  R1
}

Rule R1 {
 dbox
{ z 0.6 rx -5  s 0.9 }  R1
}

Rule R1 {
dbox
{ z 0.6 rz 5 s 1.1 }  R1
}

Rule R1 {
 dbox
{ z 0.6 rz -5 }  R1
}

Rule R1 weight 0.1 {

} 

Rule dbox {
  { s 1.5 1.6  1.2  }  box

}


