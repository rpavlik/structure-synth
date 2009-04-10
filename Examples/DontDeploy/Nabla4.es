// 31 ,47, 51, 59

set maxdepth 30
set maxsize 10
//set colorpool picture:001.JPG
set colorpool colorlist:orange,red,yellow,white,blue,cyan


10 * { y 10 } 10 * { x 10 }  1 * { ry -90 b 0.2 } R1
set background black
{  x 30 y 30 z -3 s 200 200 1 color white } box

Rule R1 {
dbox
set seed initial
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