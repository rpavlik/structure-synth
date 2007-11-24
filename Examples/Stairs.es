// Boxes. Plenty of them.
set maxobjects 20000 
set maxdepth 4000
{ sat  0.9 a 0.9 } dispatch


rule dispatch w 0.2 {
    { x 10  h 1  } dispatch
   { s 10 1 1 } sbox
}

rule dispatch w 0.1 {
{ rz 35 } dispatch

}

rule dispatch {
    { y  1 h 1 } dispatch
   { s 10 1 1 } sbox
}

rule dispatch  w 0.05 {
  step 
  dispatch
}

rule step {
   { s 10 1 1 } sbox
   { y 1 z 1 } step
}

rule step w 0.05 {
dispatch 
}

rule step w 0.02 {
}

rule sbox {
 { b 0.4 }  grid
  box
}

