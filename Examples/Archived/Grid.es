//  Produced by Structure Synth V 0.4
//  (http://structuresynth.sf.net/)

set maxdepth 25 
set background #252
{ h 30 sat 0.7  h 67 b 0.8 a 1  } s2

rule s2 w 2  { 
  {  z 5  s 0.4 0.4 10  }  sbox
  { rz 3 z 5  sat 0.98 s 0.9 h 10   }  s2
}

rule s2  { 
{ rz 1 z 5  s 0.4 0.4 10  }  sbox
{ rz 4 z 5  sat 0.98 s 1.1    }  s2
}



rule s2  w 2  { 
{ z 5  s 0.4  0.4 10  } sbox
{ rz 1 z 5 rx 90 b 1.2  }  s2
{ rz 1  z 5 ry -90 b 0.8  }  s2
}


rule sbox {
box
{ color #110 } grid
}

