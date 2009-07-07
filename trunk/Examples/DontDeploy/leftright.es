// 25, 32, 43, 45 
start
set background white

 rule left w 10  {
  box
  { z 1 rx 4 } left
}

 rule left  {
 right 
}

 rule left  {
 start 
}


 rule start  {
set seed initial
 { s 1 1 0.9  y 1 rz 90  hue 1 } left
  { s 0.9 1 1  x 1    hue -1 } left 
 { s 1 0.9 1 z 1  hue -1 } left 
}

rule right w 10 {
   box
  { z 1 rx -4 } right
}

rule right  {
  left
}

