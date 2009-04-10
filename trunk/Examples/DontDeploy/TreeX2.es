set maxdepth 30

5 * { y 3 } 1 * { ry 0 } R1

{  x 30 y 30 z 0 s 90 90 1 h 90 } box

Rule R1 {
dbox
{ z 0.6 rx 5   }  R1
}

Rule R1 {
 dbox
{ z 0.6 rx -5 }  R1
}

Rule R1 {
dbox
{ z 0.6 rz 5 }  R1
}

Rule R1 {
 dbox
{ z 0.6 rz -5 }  R1
}

Rule R1 weight 0.01 {

} 

Rule dbox {
  { s 1.5 1.6  0.2  }  box
 {s 0.9 rz 11 s 1.5 1.6  0.2  }  box
{ s 0.9 ry 12 rz -11 s 1.5 1.6  0.2  }  box
    {z 1 s 0.5 s 1.5 1.6  0.5  }  box
}


