// Double spirals [MHC 2007]
set maxobjects 40000
R2


rule R2 w 10 { 
{ x -1  rz 6 ry 6  h 2 b 0.99  s 0.99 } R2
{ s 1 } boxen 
} 


rule R2 { 
{ x -1  rx 180 rz 6 ry 6 s 0.9  color #f00  } R2
{ s 1 } boxen 
R2
} 

rule R2 { 
{ x -1  rx 90 rz 6 ry 6 s 0.9 color #f00 } R2
{ s 1 } boxen 
R2
}

rule r2 { }  





rule boxen {
 { s 0.3   s 1.0  0.3 0.3 }   grid
 { s 1 0.3 0.3 a 0.4  }   box

}