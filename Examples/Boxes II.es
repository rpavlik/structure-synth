// Boxes. Plenty of them.

60 * { y 1 } 60 * { z 1 } 1 * { sat 0.7 b 0.7 h 0   }   r1

rule r1 {   { s 4 1 1 h 0 } r2 }
rule r1 {  { s 1 1 1 h 50 } r2 }
rule r1 {   { s 2 1 1 h 90 } r2 }
rule r1 {  { s 3 1 1 h 220  } r2  }

rule r2 {   { rx 14 } box  } 
rule r2 {   { rx 24 } box  }
rule r2 {    box  }
