
set maxdepth 10

  {  x  -4.25819396973 y -4.8784570694 z  5.70054674149 s 0.1 } sphere
   {   x -5.13696432114 y -5.61583280563 z 4.06224298477 s 0.1 } sphere
   {   x -6.422539711 y -4.08374404907 z 4.06224298477 s 0.1 } sphere
  {    x -5.54376888275 y -3.34636831284 z 5.70054721832 s 0.1 } sphere

6 * { x 3 } 6  * { y 3 }  R1
{ x 30 y 30 s 90 90 1 color white } box

Rule R1 {
   { s 1.5 1.6  0.5  } dbox
  { z 0.6 rx 5 }  R1
}

Rule R1 {
{ s 1.5 1.6  0.5  } dbox
{ z 0.6 rx -5 }  R1
}

Rule R1 {
{ s 1.5 1.6  0.5  } dbox
{ z 0.6 rz 5 }  R1
}

Rule R1 {
{ s 1.5 1.6  0.5  } dbox
{ z 0.6 rz -5 }  R1
}

Rule R1 weight 0.1 {

}

rule dbox {
  { color white } box 
}

rule dbox w 0.3 {
  { color orange } box 
}