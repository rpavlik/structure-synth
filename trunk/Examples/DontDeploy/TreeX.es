set maxdepth 30

{ ry -90 } R1
{rx -90 } R1
{  x 30 y 30 z -3 s 90 90 1 h 90 } box

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
  { s 1.5 1.6  0.5  }  box
}

Rule dbox weight 0.5 {
   { ry 90 s 0.5 } R1
}

Rule dbox weight 0.5 {
{ rx 90 s 0.5 } R1
}