//  Produced by Structure Synth V 0.4
//  (http://structuresynth.sf.net/)

set maxdepth 15 
set background #252
{ h 30 sat 0.7  h 67 b 0.8 a 0.55  } s2

rule s2 w 2  { 
  sphere
  { z 5  s 0.2 0.2 10  }  box
  { z 5  sat 0.8 s 0.9   }  s2
}

rule s2  { 
sphere
{ z 5  s 0.2 0.2 10  }  box
{ z 5  sat 0.8 s 1.1    }  s2
}



rule s2  w 2  { 
sphere
{ z 5  s 0.2 0.2 10  }  box
{ z 5 rx 90 b 1.2  }  s2
{ z 5 ry -90 b 0.8  }  s2
}
