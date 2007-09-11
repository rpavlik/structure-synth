set maxobjects 2200
{ color #0a0    } r1 

rule r1 w  10  {
   { rz 15 h 1  s 1.01 0.99 1  y 1  } r1 
r2
}

rule r1 {
{ rx 10  color #0f0   s 0.99 1.01 1 z 1 } r1 
r2

}


rule r2 {
  box
{ s 1.1 0.2 0.2  color #000  } box
grid
}


