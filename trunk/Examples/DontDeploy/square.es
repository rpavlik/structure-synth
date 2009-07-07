#define $count 30

square 

rule square {
$count * { x 1 } 1 * { s 1 1 0.1 } strip
}

rule strip w 5 {
   { s 1 } vstrip 
}

rule strip {
}

rule vstrip  {
  set seed initial 
  dbox
}

rule dbox md 20 {
  { y 1 }  dbox
  dbox2
}

rule dbox2 w 0.1 {
 
}

rule dbox2 w 0.7  {
   {s  1 1 10 } tower
  { color grey } box 
}

rule dbox2 w 0.3  {

}

rule tower {
 { z 5 s 0.9 0.9 10 } box
}

rule tower {
 { z 2  s 0.9 0.9 4 } box
}

rule tower w 2 {
 { z 3 s 0.9 0.9 6 } box
}

rule tower  w 0.3 {
 { z 15/2 s 0.9 0.9 15} box
}

rule tower  w 0.1 {
 { z 25/2 s 0.9 0.9 25} box
}